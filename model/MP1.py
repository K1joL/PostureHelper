#!/usr/bin/env python
# coding: utf-8

# In[1]:





# In[1]:


import cv2
import mediapipe as mp
import numpy as np
from flask import Flask, render_template, Response


# In[2]:


app = Flask(__name__)
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
def calculate_angle(a, b, c):
    a = np.array(a)#Первая точка
    b = np.array(b)#Вторая точка
    c = np.array(c)#Третья точка
    
    radians = np.arctan2(c[1] - b[1], c[0] - b[0]) - np.arctan2(a[1] - b[1], a[0] - b[0])
    angle = np.abs(radians*180/np.pi)
    
    if angle > 180.0:
        angle = 360 -angle
    return angle


# In[3]:


wCam, hCam = 640, 480
cap = cv2.VideoCapture(0)
cap.set(3, wCam)
cap.set(4, hCam)

counter = 0
stage = None
def gen_frames():
    global counter, stage
    with mp_pose.Pose(min_detection_confidence = 0.7, min_tracking_confidence = 0.7) as pose:
        while cap.isOpened():
            ret, frame = cap.read()
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
                l_ear = [landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].x,landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].y]
                r_ear = [landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].y]
                ear_cent[0] = (landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].x + landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].x) / 2
                ear_cent[1] = (landmarks[mp_pose.PoseLandmark.LEFT_EAR.value].y + landmarks[mp_pose.PoseLandmark.RIGHT_EAR.value].y) / 2
                angleLU = calculate_angle(l_shoulder, l_elbow, l_wrist)
            
                angleRU = calculate_angle(r_shoulder, r_elbow, r_wrist)

                angleLD = calculate_angle(l_ankle, l_knee, l_hip)
            
                angleRD = calculate_angle(r_ankle, r_knee, r_hip)
            
                angle_sh = calculate_angle(ear_cent, sh_cent, hip_cent)
            
                angle = calculate_angle(l_ear, sh_cent, r_ear)
            
                cv2.putText(image, str(angleLU),
                               tuple(np.multiply(l_elbow, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
                cv2.putText(image, str(angleRU),
                               tuple(np.multiply(r_elbow, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
                #cv2.putText(image, str(angleLD),
                #               tuple(np.multiply(l_knee, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
                #cv2.putText(image, str(angleRD),
                #               tuple(np.multiply(r_knee, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
                cv2.putText(image, str(angle_sh),
                               tuple(np.multiply(ear_cent, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2, cv2.LINE_AA)
                cv2.putText(image, str(angle),
                               tuple(np.multiply(sh_cent, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)
            
            #PutText - визуализация значения угла на камере. Угол будет отражаться в данном случае возле точки локтя
            
                if angleLU > 160:
                    stage = "Down"
                if angleLU < 30 and stage == "Down":
                    stage = "Up"
                    counter += 1
                    #print(counter)
            
                print(landmarks)#Отражает координаты каждой точки
            except:
                pass
        
        
            cv2.rectangle(image, (0,0), (225, 73), (245, 117, 16), -1)
            #cv2.circle(image, (sh_cent[0], sh_cent[1]), 2, (245, 117, 66), 2)
        
            cv2.putText(image, "Count", (15, 12), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0) ,1, cv2.LINE_AA)
            cv2.putText(image, str(counter), (10, 60), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
        
            mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                                     mp_drawing.DrawingSpec(color = (245,117, 66), thickness = 2, circle_radius = 2),
                                     mp_drawing.DrawingSpec(color = (245,66,280), thickness = 2, circle_radius = 2))
            success, buffer = cv2.imencode('.jpg', image)
            fframe = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + fframe + b'\r\n')  # concat frame one by one and show result
            cv2.imshow('Mediapipe Feed', image)
        
            if cv2.waitKey(1) == 27:
                break
        cap.release()
        cv2.destroyAllWindows()
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


# In[4]:



# In[34]:


#l_shoulder, l_elbow, l_wrist


# In[37]:


#calculate_angle(l_shoulder, l_elbow, l_wrist)


# In[27]:


#for lndmrk in mp_pose.PoseLandmark:
#    print(lndmrk)


# In[28]:


#landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value]


# In[ ]:




