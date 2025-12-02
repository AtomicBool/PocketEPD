import time
import qrcode

from utils.qr import generate_qr_matrix 

def waitACK(hSerial, timeout=30.0):
    start_time = time.time()
    while time.time() - start_time < timeout:
        if hSerial.in_waiting > 0:
            response = hSerial.read(1)
            if response == b'\x67':  # 0x67 ACK
                return True
        time.sleep(0.01)
    
    print("[!] Failed to receive ACK from STM32 within timeout period.")
    return False

def parseCoordinate(x, y):
    x_low = x & 0xFF
    x_high = (x >> 8) & 0xFF
    y_low = y & 0xFF
    y_high = (y >> 8) & 0xFF
    return x_high, x_low, y_high, y_low

def drawPixel(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    color = int(params[2])

    # 0x02 XX XX YY YY COLOR
    packet = bytes([
        0x02,
        *parseCoordinate(x0, y0),
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawByte(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    mask = int(params[2], 0)
    color = int(params[3])

    # 0x03 XX XX YY YY MASK CLOR
    packet = bytes([
        0x03,
        *parseCoordinate(x0, y0),
        mask,
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawChar(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    char = int(params[2].encode('ascii', errors='ignore')[0])
    color = int(params[3])
    
    # 0x04 XX XX YY YY CHAR CLOR
    packet = bytes([
        0x04,
        *parseCoordinate(x0, y0),
        char,
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawPie(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    r_out = int(params[2])
    r_in = int(params[3])
    percentage = int(params[4])
    color = int(params[5])

    # 0x05 XX XX YY YY RO RI PT CLOR
    packet = bytes([
        0x05,
        *parseCoordinate(x0, y0),
        r_out,
        r_in,
        percentage,
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawLine(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    x1 = int(params[2])
    y1 = int(params[3])
    color = int(params[4])

    # 0x06 XX XX YY YY XX XX YY YY CLOR
    packet = bytes([
        0x06,
        *parseCoordinate(x0, y0),
        *parseCoordinate(x1, y1),
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawRectangle(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    x1 = int(params[2])
    y1 = int(params[3])
    color = int(params[4])

    # 0x07 XX XX YY YY XX XX YY YY CLOR
    packet = bytes([
        0x07,
        *parseCoordinate(x0, y0),
        *parseCoordinate(x1, y1),
        color
    ])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def drawString(params, hSerial):
    x0 = int(params[0])
    y0 = int(params[1])
    color = int(params[2])

    text_bytes = params[3].encode('ascii', errors='ignore')[:16]  # 只取前16个字符，忽略非ASCII字符
    text_bytes = text_bytes.ljust(16, b'\x00')  # 用0x00填充到16字节

    # 0x08 XX XX YY YY CLOR CHAR*16
    packet = bytes([
        0x08,
        *parseCoordinate(x0, y0),
        color
    ]) + text_bytes

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def send_qr_data(x0, y0, matrix, color, hSerial):
    height = len(matrix)
    width = len(matrix[0])
    
    bytes_per_row = width // 8
    
    success_count = 0
    total_commands = 0

    for y in range(height):
        for byte_col in range(bytes_per_row):
            start_bit = byte_col * 8
            end_bit = start_bit + 8
            
            byte_mask = 0
            
            for bit_pos in range(start_bit, end_bit):
                if matrix[y][bit_pos]: # if color is set
                    pos_in_byte = 7 - (bit_pos - start_bit)
                    byte_mask |= (1 << pos_in_byte)
                    
            x_pos = x0 + start_bit
            y_pos = y0 + y

            drawByte_params = [str(x_pos), str(y_pos), f"0x{byte_mask:02x}", str(color)]
            if drawByte(drawByte_params, hSerial):
                success_count += 1
            total_commands += 1
    
    print(f"[+] QR bytes sent: {success_count}/{total_commands} successfully")
    return success_count == total_commands

def drawQR(params, hSerial):
    # x y color border size data [error_correction] [version]
    x0 = int(params[0])
    y0 = int(params[1])
    color = int(params[2])
    border = int(params[3])
    size = int(params[4])
    text = params[5]
    
    error_correction_map = {
            "L": qrcode.constants.ERROR_CORRECT_L,
            "M": qrcode.constants.ERROR_CORRECT_M,
            "Q": qrcode.constants.ERROR_CORRECT_Q,
            "H": qrcode.constants.ERROR_CORRECT_H
        }
    ec = error_correction_map.get(
        params[6],
        qrcode.constants.ERROR_CORRECT_L
    ) if len(params) > 6 else qrcode.constants.ERROR_CORRECT_L
    
    mat = generate_qr_matrix(
        data = text,
        qr_border = border,
        qr_ec = ec,
        qr_version = int(params[7]) if len(params) > 7 else None,
        qr_size = size
    )

    send_qr_data(x0, y0, mat, color, hSerial)
    
    return True

def clearScreen(params, hSerial):
    packet = bytes([0x00])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

def updateDisplay(params, hSerial):
    packet = bytes([0x01])

    print(packet.hex("_"))
    hSerial.write(packet)
    return waitACK(hSerial)

command_map = {
    "clear": clearScreen,
    "end": updateDisplay,
    "pixel": drawPixel,
    "byte": drawByte,
    "char": drawChar,
    "pie": drawPie,
    "line": drawLine,
    "rect": drawRectangle,
    "str": drawString,
    "qr": drawQR
}