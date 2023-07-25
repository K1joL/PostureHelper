import cv2
import mediapipe as mp
import numpy as np
from flask import Flask, render_template, Response
import socket
import threading
import time
#import keyboard

#globals
self_message = ''
message_to_app = ''
frame_coordin = []
Calibration_stage = False
timer = 0
ex_stage = None
show_stage = False
flag = 0
count = 0
Scount = 0
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def calculate_angle(a, b, c):
    a = np.array(a)#Первая точка
    b = np.array(b)#Вторая точка
    c = np.array(c)#Третья точка
    
    radians = np.arctan2(c[1] - b[1], c[0] - b[0]) - np.arctan2(a[1] - b[1], a[0] - b[0])
    angle = np.abs(radians*180/np.pi)
    
    if angle > 180.0:
        angle = 360 -angle
    return angle

def Sbends(angle_L_hip, angle_R_hip):
    global ex_stage, count
    if (ex_stage == "Up" and (angle_L_hip <= 145.0)):
        ex_stage = "Left"
        count += 1
    elif (ex_stage == "Up" and (angle_R_hip <= 145.0)):
        ex_stage = "Right"
        count += 1 
    if (ex_stage == "Right" and (angle_R_hip >= 170.0)):
        ex_stage = "Up"
    if (ex_stage == "Left" and (angle_L_hip >= 170.0)):
        ex_stage = "Up"
    return 0

def Fbends(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee):
    global ex_stage, count
    if (ex_stage == "Up" and (angle_L_hip and angle_R_hip) <= 110.0):
        if ((angle_L_knee and angle_R_knee) >= 160.0):
            ex_stage = "Down"
            count += 1
    if (ex_stage == "Down" and (angle_L_hip and angle_R_hip) >= 160.0):
        ex_stage = "Up"
    return 0

def Squats(angle_L_knee, angle_R_knee):
    global ex_stage, count
    if (ex_stage == "Up" and (angle_L_knee and angle_R_knee) <= 90.0):
        ex_stage = "Down"
        count += 1
    if (ex_stage == "Down" and (angle_L_knee and angle_R_knee) >= 160.0):
        ex_stage = "Up"
    return 0

def Standup(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee):
    global message_to_app
    message_to_app = "Ex NotReady"
    if (angle_L_hip >= 165.0) or (angle_R_hip >= 165.0):
        if (angle_L_knee >= 165.0) or (angle_R_knee >= 165.0):
            message_to_app = "Ex Ready"
    return 0

# Функция запуска режима выполнения упражнений:
# Если поток получения сообщений получил одно из названий упражнений, то флаг готовности запуска соответсвующего упражнения выставляется на соответсвтующее значение
# Интерфейсу подаётся сообщение о готовности "Ex Start"
# Если поток получение сообщений получает сообщение "Stand" и при этом сообщение, которое мы подали последний раз имеет состояниие "Ex Start", то запускаем функцию проверки 
# Того, что человек встал
# Если человек готов к выполнению упражнения, флаг позиции человека устанавливается в состояние "Up", счётчик повторений обнуляется - запускаеться функция выполнения соответствующего упражнения
# Иначе, флаг готовности запуска соответсвующего упражнения выставляется на 0
# Если человеке выполнил одно повторение, на интерфейс подаётся сообщение "Ex Did"
# Если поток получения сообщений получил сообщение "Pause" - выполнение упражнения прерывается
def Exercise(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee):
    exsercises = {"Squats" : 1, "Fbends" : 2, "Sbends" : 3}
    global self_message, flag, message_to_app, ex_stage, count, client_socket, Scount, show_stage
    if ((self_message in exsercises) and flag == 0):
        flag = exsercises[self_message]
        self_message = ""
        show_stage = False
        message_to_app = "Ex Start"
        client_socket.sendto(message_to_app.encode(), ('localhost', 4444))
    if (message_to_app == "Ex Start" and self_message == "Stand"):
        Standup(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee)
        if (message_to_app == "Ex Ready"):
            client_socket.sendto(message_to_app.encode(), ('localhost', 4444))
            ex_stage = "Up"
            count = 0
            Scount = 0
        else:
            client_socket.sendto(message_to_app.encode(), ('localhost', 4444))
            flag = 0
    if (message_to_app == "Ex Ready"):
        if (flag == 1):
            Squats(angle_L_knee, angle_R_knee)
        elif (flag == 2):
            Fbends(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee)
        elif (flag == 3):
            Sbends(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee)
    if (Scount != count):
        message_to_app = "Ex Did"
        client_socket.sendto(message_to_app.encode(), ('localhost', 4444))
        message_to_app = "Ex Ready"
        Scount += 1
    if (self_message == "Pause"):
        message_to_app = ''
        flag = 0
    return 0

# Функция калибровки:
# Запоминает данные ,которые вычисляются в каждом кадре(углы, длина спины) в массиве до тех пор, пока не прошло 5 секунд
# Когда проходит 5 секунд, каждый набор данных сравнивается друг с другом, определяются максимальные значения среди всех данных
# По завершении сравнения данных, стадия готовности калибровки переводится в True - калибровка завершена
# Сообщение, полученное от интерфейса удаляется, таймер выставляется в ноль, обнуляется массив набора данных за каждый фрейм
def Calibration(angle_L_elbow, angle_sh_cent, angle_R_elbow, angle_L_knee, angle_R_knee, angle_sh, llen):
    global self_message, timer, frame_coordin, Calibration_stage
    fstimer = [0, 0, 0, 0, 0, 0, 0]
    if (time.time() - timer <= 5.0):
        frame_coordin.append([angle_L_elbow, angle_sh_cent, angle_R_elbow, angle_L_knee, angle_R_knee, angle_sh, llen])
    else:
        for i in range(len(frame_coordin)):
            for j in range(len(fstimer)):
                if frame_coordin[i][j] > fstimer[j]:
                    fstimer[j] = frame_coordin[i][j]
        Calibration_stage = True
        self_message = ''
        timer = 0

        frame_coordin = []
    return Calibration_stage, fstimer

# Поток для получения сообщений с интерфейса
# Если поток, получае сообщение "Stop" - работа поток прекращается
# Глобально изменяет только сообщение, адресованное модели
def thread_function1():
    global self_message
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = ('localhost', 12345)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(server_address)
    while True:
        data, address = server_socket.recvfrom(1024)
        self_message = data.decode()
        print(self_message)
        if (self_message == "Stop"):
            return self_message
# Вызов Flask модуля, доп. модулей для обработки кадров моделью и захват камеры
app = Flask(__name__)
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
wCam, hCam = 640, 480
cap = cv2.VideoCapture(0)
cap.set(3, wCam)
cap.set(4, hCam)

# Основная функция обработки кадров моделью, а также обработки сообщений
# Глобально изменяет переменые калибровочных точек человека, таймер, в течении которого проводится калибровка, флаг готовности калибровки, сокет клиента, для подачи сообщений интерфейсу
def gen_frames():
    global frame_coordin, timer, Calibration_stage, client_socket, show_stage
    llen = 0
    Calib = []
    global self_message, message_to_app, ex_stage
    # Запуск потока чтения данных
    thread1 = threading.Thread(target=thread_function1)
    thread1.start()
    with mp_pose.Pose(min_detection_confidence = 0.7, min_tracking_confidence = 0.7) as pose:
        while cap.isOpened():
            # Открытие камеры для отображения кадров
            ret, frame = cap.read()
            # Если не удалось открыть камеру - откат
            if not ret:
                break
            #Изменяем цвет фрейма
            image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image.flags.writeable = False
            #Обучение модели
            results = pose.process(image)
            #Возвращаем изначальный цвет
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            sh_cent = [0, 0]
            ear_cent = [0, 0]
            hip_cent = [0, 0]
            #Извлекаем координаты положения точек
            try:
                landmarks = results.pose_landmarks.landmark
                l_shoulder = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x,landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y] 
                l_elbow = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].x,landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y] 
                l_wrist = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].x,landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y]
                r_shoulder = [landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].y] 
                r_elbow = [landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].y] 
                r_wrist = [landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].y]
                sh_cent[0] =  (landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x + landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].x) / 2
                sh_cent[1] =  (landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y + landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].y) / 2
                l_ankle = [landmarks[mp_pose.PoseLandmark.LEFT_ANKLE.value].x,landmarks[mp_pose.PoseLandmark.LEFT_ANKLE.value].y] 
                l_knee = [landmarks[mp_pose.PoseLandmark.LEFT_KNEE.value].x,landmarks[mp_pose.PoseLandmark.LEFT_KNEE.value].y] 
                l_hip = [landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x,landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y]
                r_ankle = [landmarks[mp_pose.PoseLandmark.RIGHT_ANKLE.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_ANKLE.value].y] 
                r_knee = [landmarks[mp_pose.PoseLandmark.RIGHT_KNEE.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_KNEE.value].y]
                r_hip = [landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y]
                hip_cent[0] = (landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x + landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x) / 2
                hip_cent[1] = (landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y + landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y) / 2
                #l_ear = [landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].x,landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].y]
                #r_ear = [landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].y]
                ear_cent[0] = (landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].x + landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].x) / 2
                ear_cent[1] = (landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].y + landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].y) / 2
                nose = [landmarks[mp_pose.PoseLandmark.NOSE.value].x,landmarks[mp_pose.PoseLandmark.NOSE.value].y]

                
                # Расчёт всех необходимых углов
                angle_L_elbow = calculate_angle(l_shoulder, l_elbow, l_wrist)
                
                angle_sh_cent = calculate_angle(nose, sh_cent, hip_cent)
                
                angle_R_elbow = calculate_angle(r_shoulder, r_elbow, r_wrist)

                angle_L_knee = calculate_angle(l_ankle, l_knee, l_hip)
            
                angle_R_knee = calculate_angle(r_ankle, r_knee, r_hip)
            
                angle_sh = calculate_angle(ear_cent, sh_cent, hip_cent)
                
                angle_L_hip = calculate_angle(l_shoulder, l_hip, l_knee)
                
                angle_R_hip = calculate_angle(r_shoulder, r_hip, r_knee)
            
                # Расчёт длины спины
                llen = int(((sh_cent[0]*480 - hip_cent[0]*480)**2 + (sh_cent[1]*640 - hip_cent[1]*640)**2)**0.5)

            # [angle_L_elbow, angle_sh_cent, angle_R_elbow, angle_L_knee, angle_R_knee, angle_sh, llen]
            # Если поток получения сообщений получит сообщение о калибровке, будет произведена проверка на запуск таймера.
            # Если таймер не запущен - калибровка не начата
            # Для выполнения калибровки, необходимо установить флаг готовности калибровки в состояние False
                if (self_message == 'Calibration'):
                    if not(timer):
                        timer = time.time()
                        Calibration_stage = False
            # Если калибровка ещё не была выполнена или произошел сброс калибровки и таймер запущен, запускается функция калибровки
                if (not(Calibration_stage) and timer):
                    Calibration_stage, Calib = Calibration(angle_L_elbow, angle_sh_cent, angle_R_elbow, angle_L_knee, angle_R_knee, angle_sh, llen)
            # Если поток получения сообщений получит сообщение "Show" и при этом калибровка будет уже выполнена, необходимо отображать на каждом кадре 
            # Скелет человека
            # Для этого, флаг показа скелета выставляется в состояние True, полученное сообщение обнуляется
                if (self_message == "Show" and Calibration_stage):
                    show_stage = True
                    self_message = "" 
            # Если поток получения сообщения получает сообщение "Hide" необходимо прекратить отображение скелета в каждом кадре
            # Для этого, флаг показа скелета выставляется в состояние False, полученное сообщение обнуляется
                if (self_message == "Hide"):
                    show_stage = False
                    self_message = ""
                # Если калибровка завершена, необходимо следить за осанкой
                # Причём, если включен режим отображения скелета, необходимо также отображать угол или длину в месте соответсвующей ошибки
                # Если ошибок нет, посылаем сообщение "Error None"
                if (Calibration_stage):
                    Error_message = "Error"
                    if not(abs(angle_R_knee  - Calib[4]) <= Calib[4]*0.02):
                        Error_message = Error_message + ' RightLeg' 
                        if (show_stage):
                            cv2.putText(image, str(angle_R_knee),
                                    tuple(np.multiply(r_knee, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)

                    if not(abs(angle_L_knee - Calib[3]) <= Calib[3]*0.02):
                        Error_message = Error_message + ' LeftLeg'
                        if (show_stage):
                            cv2.putText(image, str(angle_L_knee),
                                    tuple(np.multiply(l_knee, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)
                        
                    if not(abs(angle_sh - Calib[5]) <= Calib[5]*0.1) and (abs(angle_sh_cent - Calib[1]) <= Calib[1]*0.21):
                        Error_message = Error_message + ' Neck'
                        if (show_stage):
                            cv2.putText(image, str(angle_sh),
                                    tuple(np.multiply(sh_cent, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)
                    if not(abs(llen - Calib[-1]) <= Calib[-1]*0.008):
                        Error_message = Error_message + ' Back'
                        if (show_stage):
                            cv2.putText(image, str(llen),
                                    tuple(np.multiply(hip_cent, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)
                    if (len(Error_message) == 5):
                        Error_message = Error_message + ' None'
                    # В конце каждого цикла, если калибровка завершена, передаём сообщение об ошибках или их отсутствии
                    client_socket.sendto(Error_message.encode(), ('localhost', 4444))
                # Запускаем функцию упражнения в конце каждого цикла
                Exercise(angle_L_hip, angle_R_hip, angle_L_knee, angle_R_knee)
            except:
                pass
            # Если запущен режим отображения скелета, на каждом кадре рисуется скелет
            if (show_stage):
                mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                                          mp_drawing.DrawingSpec(color = (245,117, 66), thickness = 2, circle_radius = 2),
                                          mp_drawing.DrawingSpec(color = (245,66,280), thickness = 2, circle_radius = 2))
            success, buffer = cv2.imencode('.jpg', image)
            fframe = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + fframe + b'\r\n')  # concat frame one by one and show result
            #cv2.imshow('Mediapipe Feed', image)
        
            if (self_message == "Stop"):
                break
        cap.release()
        #cv2.destroyAllWindows()
        client_socket.close()
        thread1.join()
        #keyboard.press_and_release('ctrl + c')
        return 0
@app.route('/video_feed')
def video_feed():
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')


if __name__ == '__main__':
    app.run(debug=True)



