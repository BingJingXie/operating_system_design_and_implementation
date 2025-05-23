import struct
import time
import serial

def send_kernel(port, kernel_path):
    with open(kernel_path, 'rb') as f:
        kernel_data = f.read()
    
    with serial.Serial(port, 115200) as ser:
        # 發送kernel大小
        size = len(kernel_data)
        ser.write(struct.pack('>I', size))
        time.sleep(0.1)
        
        # 發送kernel資料
        ser.write(kernel_data)

# 使用方式
send_kernel('/dev/ttyUSB0', 'kernel8.img')