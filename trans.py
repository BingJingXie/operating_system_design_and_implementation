import struct
import time
import serial
import sys
import os

def send_kernel(port, kernel_path):
    try:
        # 檢查檔案是否存在
        if not os.path.exists(kernel_path):
            print(f"Error: Kernel file '{kernel_path}' not found!")
            sys.exit(1)
            
        with open(kernel_path, 'rb') as f:
            kernel_data = f.read()
        
        print(f"Opening serial port {port}...")
        with serial.Serial(port, 115200, timeout=5) as ser:
            # 清空串口緩衝區
            ser.reset_input_buffer()
            ser.reset_output_buffer()
            
            # 發送kernel大小
            size = len(kernel_data)
            print(f"Sending kernel size: 0x{size:08x}")
            # 使用大端序發送大小
            size_bytes = size.to_bytes(4, byteorder='big')
            ser.write(size_bytes)
            ser.flush()  # 確保資料被送出
            
            # 等待 bootloader 確認收到大小
            time.sleep(1)
            
            # 發送kernel資料
            print(f"Sending kernel data ({size} bytes)...")
            # 逐位元組發送資料
            total_sent = 0
            for byte in kernel_data:
                ser.write(bytes([byte]))
                ser.flush()
                total_sent += 1
                if total_sent % 100 == 0:  # 每100位元組顯示一次進度
                    print(f"\rProgress: {total_sent}/{size} bytes ({total_sent*100//size}%)", end='', flush=True)
                time.sleep(0.001)  # 短暫延遲確保資料被處理
            
            print("\nWaiting for bootloader response...")
            # 讀取剩餘的輸出
            response = ""
            start_time = time.time()
            while time.time() - start_time < 10:  # 等待最多10秒
                if ser.in_waiting:
                    data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
                    response += data
                    print(data, end='', flush=True)
                time.sleep(0.1)
            
            if "Kernel loaded" not in response:
                print("\nWarning: Did not receive kernel loaded confirmation!")
                print("Debug information:")
                print(f"Expected size: 0x{size:08x}")
                print(f"Size bytes sent: {[hex(b) for b in size_bytes]}")
                print(f"Total bytes sent: {total_sent}")
                print(f"Expected bytes: {size}")
                if total_sent != size:
                    print("Error: Not all bytes were sent!")
                print("\nTroubleshooting tips:")
                print("1. Check if the USB-to-TTL adapter is properly connected")
                print("2. Verify the correct port name (try: ls -l /dev/ttyUSB*)")
                print("3. Make sure you have permission to access the port (try: sudo usermod -a -G dialout $USER)")
                print("4. Try increasing the delay between bytes")
                
    except serial.SerialException as e:
        print(f"Error: Could not open serial port {port}")
        print(f"Details: {e}")
        print("\nTroubleshooting tips:")
        print("1. Check if the USB-to-TTL adapter is properly connected")
        print("2. Verify the correct port name (try: ls -l /dev/ttyUSB*)")
        print("3. Make sure you have permission to access the port (try: sudo usermod -a -G dialout $USER)")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 trans.py <kernel_path>")
        print("Example: python3 trans.py kernel8.img")
        sys.exit(1)
        
    kernel_path = sys.argv[1]
    send_kernel('/dev/ttyUSB0', kernel_path)