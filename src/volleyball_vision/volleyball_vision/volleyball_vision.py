import cv2
import numpy as np
import math

def process_frame(frame):
    height, width = frame.shape[:2]
    img_center = (width//2, height//2)

    # HSV颜色空间转换
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # 黄色范围（可根据实际环境调整）
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])
    
    # 创建掩膜
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    
    # 形态学操作
    kernel = np.ones((5,5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    
    # 查找轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if contours:
        max_contour = max(contours, key=cv2.contourArea)
        (x, y), radius = cv2.minEnclosingCircle(max_contour)
        ball_center = (int(x), int(y))
        
        # 计算距离
        distance = math.hypot(x - img_center[0], y - img_center[1])
        
        # 确定方位
        position = []
        if x < img_center[0]:
            position.append("左侧")
        elif x > img_center[0]:
            position.append("右侧")
            
        if y < img_center[1]:
            position.append("上方")
        elif y > img_center[1]:
            position.append("下方")
            
        position_str = "".join(position) if position else "正中心"
        
        # 绘制图形
        cv2.circle(frame, ball_center, 5, (0,0,255), -1)
        cv2.circle(frame, img_center, 5, (255,0,0), -1)
        cv2.line(frame, ball_center, img_center, (0,255,0), 2)
        
        # 显示文字信息
        cv2.putText(frame, f"Distance: {distance:.1f}px", (10,30), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)
        cv2.putText(frame, f"Position: {position_str}", (10,60), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)
        
    return frame

# 初始化摄像头
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    processed_frame = process_frame(frame)
    
    cv2.imshow('Volleyball Tracking', processed_frame)
    
    # 按q退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()