# ----------------------------------------------------------------------------------------------------
#
#   @   DLIP final project code  :   HADA CAM rubber cone detect
#   @   Update     				 :   2023.6.15
#   @   Purpose    			     :   Cmaera code using yolov8
#
# ----------------------------------------------------------------------------------------------------

import cv2
import torch
from ultralytics import YOLO
import numpy as np
import time
import tensorrt
from multiprocessing import shared_memory
from   datetime    import datetime

# # 내 경로 설정을 해야 현재 내가 다운로드를 받은 ultralytics를 받아올 수 있다
# import sys
# sys.path.append('D:\HADA\HADA_3rd\Camera\yolov8')
# model = YOLO('seg_0514.pt')
# weight_path = 'D:\HADA\HADA_3rd\Camera\yolov8\\runs\segment\\train7\weights\\best.engine'
weight_path = 'runs\segment\\best.pt'
model = YOLO(weight_path)

my_cam_index = 0 
# 이 파트가 카메라를 여는데 가장 중요한 부분
cap = cv2.VideoCapture(my_cam_index, cv2.CAP_DSHOW) 
# print('cap: ', str(cap))

cap.set(cv2.CAP_PROP_FPS, 60.0) 
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M','J','P','G'))
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.75) 
# cap.set(cv2.CAP_PROP_EXPOSURE, -11.0)
print(cap.get(cv2.CAP_PROP_EXPOSURE))

# ------------------------------------------------------------------- #

width   = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height  = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps     = int(cap.get(cv2.CAP_PROP_FPS))

print('get cam fps: ', fps)
user_font    = {0   : cv2.FONT_HERSHEY_COMPLEX,
                1   : cv2.FONT_ITALIC,
                2   : cv2.FONT_HERSHEY_DUPLEX,
                }

sm_name = "HADA3_CAM"
print("start...")
shared_memory_name = "HADA3_CAM"
shm_size = 6 * 2 * 4  # (int int) * 6개 
shm = shared_memory.SharedMemory(name=shared_memory_name, create=True, size=shm_size)
print("sm memory is open...")

# 영상 저장 initialize

fourcc = cv2.VideoWriter_fourcc(*'XVID')
output_filename   : str = datetime.today().strftime(".\output\output_%Y_%m_%d_%H%M.mp4")  # save results to project/name
frame_size = (width, height)
out = cv2.VideoWriter(output_filename, fourcc, fps, frame_size)

with open('counting_result.txt', 'w') as f:
    f.write('')

def key_command(_key):
            # 정지 

        if _key == ord('s') :   cv2.waitKey()
        
        # 대회에서는 터널은 일정할거니까 터널에 해당하는 saturation값을 특정해 놓자
        
        # 카메라 노출 수동 조절
        elif _key == ord('i'):
        # Get current exposure.
            exposure = cap.get(cv2.CAP_PROP_EXPOSURE)
            # Increase exposure by 1.
            cap.set(cv2.CAP_PROP_EXPOSURE, exposure + 1)

        # Decrease exposure on 'd' key press.
        elif _key == ord('d'):
            # Get current exposure.
            exposure = cap.get(cv2.CAP_PROP_EXPOSURE)
            # Decrease exposure by 1.
            cap.set(cv2.CAP_PROP_EXPOSURE, exposure - 1)

# SM memory
def send_data(shm_name, points):
    shm = shared_memory.SharedMemory(name=shm_name)
    shared_data = np.ndarray((6,2), dtype='int', buffer=shm.buf)

    # Check if the middle_points list is empty and return if it is
    if not points:
        shared_data.fill(0)
        return    
    else:

        # Clear the shared_data array with zeros
        n = len(points)

        # parsing 작업
        for i in range(n):
            min_idx = i
            for j in range(i+1, n):
                if points[j][1] < points[min_idx][1]:
                    min_idx = j
            points[i], points[min_idx] = points[min_idx], points[i]

        # Keep only the first 6 sorted middle_points
        points = points[:6]
    
    shared_data.fill(0)

    # shared_data에 현재 측정된 box의 좌표를 넘긴다. 
    for i in range(len(points)):
        shared_data[i] = points[i]


colors = {0: (255, 0, 0),  # Blue for class 0
          1: (40, 188, 249),  # white for class 1
          }

# 이건 자동적으로 어떻게 불러올까.... yaml 파일에서 바로 불러올 수 있으면 좋을듯
cls_name = {0: "b_rub",
            1: "y_rub", }

line_width = 2
font_thick = 1

# Loop through the video frames
while cap.isOpened():

    start_time = time.time()
    prev_time = start_time

    # Read a frame from the video
    ret, frame      = cap.read()

    if ret == True: # Run YOLOv8 inference on the frame

        results = model(frame, imgsz=640)
        result = results[0]
        len_result = len(result)

        if len_result != 0: # 객체가 인식이 된다면 
        
            middle_points = []  # sm data list 초기화

            for idx in range(len_result):

                detection = result[idx]

                # box = detection.boxes.cpu().numpy()[0]
                box = detection.boxes.cpu().numpy()[0]
                cls = int(box.cls[0])                

                xywh    = box.xywh[0].astype(int)
                centerX = xywh[0]
                centerY = xywh[1]
                area    = xywh[2] * xywh[3]
                
                xyxy    = box.xyxy[0].astype(int)
                x1      = xyxy[0]
                y1      = xyxy[1]
                conf    = box.conf[0]
                # print("cX: ", str(centerX))
                # print("cY: ", str(centerY))
                # print("area: ", str(area))

                if area > 3000 and conf > 0.8: 

                    color = colors[cls]
                    # if segment.shape[0] > 0:
                    #     cv2.fillPoly(frame, [segment], color)
                    conf = box.conf[0]
                    r = box.xyxy[0].astype(int) # box
                    
                    # # if size_cut < size: # 특정 위치 아래로 내려가면 주차를 하지 않은것으로 인식
                    cv2.line(frame, (centerX, centerY), (centerX, centerY), (0, 0, 255), 4)
                    
                    # box를 그리는 부분
                    cv2.rectangle(frame, r[:2], r[2:], color, thickness=line_width, lineType=cv2.LINE_AA)
                    label = str(cls_name[cls]) + ' ' +str(f'{ conf:.2f}')
                    text_w, text_h = cv2.getTextSize(label, 0, fontScale=line_width/3,thickness=font_thick)[0]
                    outside = r[:2][1] - text_h >= 3
                    text_p1 = r[:2]
                    text_p2 = r[:2][0] + text_w, r[:2][1] - text_h - 3 if outside else  r[:2][1] + text_h + 3
                    cv2.rectangle(frame, text_p1, text_p2, color, -1, cv2.LINE_AA)  # filled

                    cv2.putText(frame, label ,
                                (text_p1[0], text_p1[1] - 2 if outside else text_p1[1] + text_h + 2), 
                                0, 
                                line_width/3, 
                                (255,255,255), 
                                thickness=font_thick, 
                                lineType=cv2.LINE_AA)
                    
                    # shared memory에 append를 해서 넘기는 부분
                    middle_points.append((centerX, centerY))

            send_data(sm_name, middle_points)
        else:
            middle_points = []  # 0으로
            send_data(sm_name, middle_points)

        diff_time = time.time() - prev_time
        
        if diff_time > 0:
            fps = 1 / diff_time

        cv2.putText(frame, f'FPS : {fps:.2f}', (20, 40), user_font[2], 1, (0, 255, 0), 2)

        out.write(frame)
        # # Display the annotated frame
        # cv2.imshow("result", frame)
        cv2.imshow("mask", frame)

        key = cv2.waitKey(1) & 0xFF
        if key == ord('q')   :   break
        key_command(key)

    else:
        print("Camera is Disconnected ...!")
        break

# Release the video capture object and close the display window
shm.close()
print("sm memory close...") 

cap.release()
cv2.destroyAllWindows()