import shlex
import serial
import serial.tools.list_ports
import argparse
import sys

from command_handlers import command_map

def process_command(line, hSerial, row_number):
    line = line.lstrip()
    
    # Skip empty lines and comments        
    if not line or line.startswith('#'):
        return True
    
    # Split parts
    parts = shlex.split(line)
    command = parts[0].lower()
    params = parts[1:]

    if command in command_map:
        return command_map[command](params, hSerial)
    else:
        print(f"[!] Row {row_number}: Unknown Command: '{command}'")
        return False

def detectPorts():
    ports_list = list(serial.tools.list_ports.comports())
    if len(ports_list) <= 0:
        print("No serial port found!")
        sys.exit(1)
    
    selected_port = ports_list[0]
    print(f"[+] Device found: {selected_port.device}")
    return selected_port.device

def main():
    parser = argparse.ArgumentParser(description='Parse commands from file and send to serial port')
    parser.add_argument('-f', '--file', default="./testCMDs", help='Path to file')
    parser.add_argument('-p', '--port', default=detectPorts(), help='Port to use')
    parser.add_argument('-b', '--baudrate', type=int, default=115200, help='Baudrate')
    
    args = parser.parse_args()
    
    # 打开串口
    ser = serial.Serial(args.port, args.baudrate, timeout=0.5)
    print("[-] Status:",  "Connected" if ser.is_open else "Error")
    
    # 读取并处理文件
    try:
        with open(args.file, 'r') as f:
            lines = f.readlines()
        
        success_count = 0
        total_count = 0
        
        print(f"\n[-] Processing: {args.file}")
        print("=" * 40)
        
        for i, line in enumerate(lines, 1):
            if process_command(line, ser, i):
                success_count += 1
            total_count += 1
        
        print("=" * 40)
        print(f"[-] Completed: {success_count}/{total_count} commands sent")
        
    except FileNotFoundError:
        print(f"[!] No such file: {args.file}")
    except Exception as e:
        print(f"[!] Errors occured while parsing: {e}")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
