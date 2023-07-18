#!/usr/bin/env python
# coding: utf-8

# In[11]:


# In[14]:


import cv2
import mediapipe as mp
import numpy as np


# In[ ]:





# In[41]:


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


# In[42]:





# In[49]:


wCam, hCam = 640, 480
cap = cv2.VideoCapture(0)
cap.set(3, wCam)
cap.set(4, hCam)

counter = 0
stage = None

with mp_pose.Pose(min_detection_confidence = 0.7, min_tracking_confidence = 0.7) as pose:
    while cap.isOpened():
        ret, frame = cap.read()
        #Изменяем цвет фрейма
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False
        #Обучение модели
        results = pose.process(image)
        #Возвращаем изначальный цвет
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        
        #Извлекаем координаты положения точек
        try:
            landmarks = results.pose_landmarks.landmark
            
            l_shoulder = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x,landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y] 
            l_elbow = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].x,landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y] 
            l_wrist = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].x,landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y] 
            
            angle = calculate_angle(l_shoulder, l_elbow, l_wrist)
            
            cv2.putText(image, str(angle),
                           tuple(np.multiply(l_elbow, [640, 480]).astype(int)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
            #PutText - визуализация значения угла на камере. Угол будет отражаться в данном случае возле точки локтя
            
            if angle > 160:
                stage = "Down"
            if angle < 30 and stage == "Down":
                stage = "Up"
                counter += 1
                print(counter)
            
            #print(landmarks)#Отражает координаты каждой точки
        except:
            pass
        
        
        cv2.rectangle(image, (0,0), (225, 73), (245, 117, 16), -1)
        
        cv2.putText(image, "Count", (15, 12), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0) ,1, cv2.LINE_AA)
        cv2.putText(image, str(counter), (10, 60), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv2.LINE_AA)
        
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                                 mp_drawing.DrawingSpec(color = (245,117, 66), thickness = 2, circle_radius = 2),
                                 mp_drawing.DrawingSpec(color = (245,66,280), thickness = 2, circle_radius = 2))
        
        cv2.imshow('Mediapipe Feed', image)
        
        if cv2.waitKey(1) == 27:
            break
    cap.release()
    cv2.destroyAllWindows()


# In[33]:





# In[34]:


l_shoulder, l_elbow, l_wrist


# In[37]:


calculate_angle(l_shoulder, l_elbow, l_wrist)


# In[27]:


for lndmrk in mp_pose.PoseLandmark:
    print(lndmrk)


# In[28]:


landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value]


# In[ ]:




