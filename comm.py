import serial

import serial
import serial.tools.list_ports
 
# 获取所有串口设备
ports_list = list(serial.tools.list_ports.comports())
if len(ports_list) <= 0:
    print("[!] No serial port found!")
else:
    print("[+] Will be using:", ports_list[0][1])

ser = serial.Serial(ports_list[0][0], 115200, timeout=0.5)
print("[-] Status:",  "Success" if ser.is_open else "Failed")


