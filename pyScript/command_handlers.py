import time

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

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
    waitACK(hSerial)
    return True

def clearScreen(params, hSerial):
    packet = bytes([0x00])

    print(packet.hex("_"))
    hSerial.write(packet)
    waitACK(hSerial)
    return True

def updateDisplay(params, hSerial):
    packet = bytes([0x01])

    print(packet.hex("_"))
    hSerial.write(packet)
    waitACK(hSerial)
    return True

command_map = {
    "clear": clearScreen,
    "end": updateDisplay,
    "pixel": drawPixel,
    "byte": drawByte,
    "char": drawChar,
    "pie": drawPie,
    "line": drawLine,
    "rect": drawRectangle,
    "str": drawString
}