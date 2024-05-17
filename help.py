import serial

# Open the serial port
ser = serial.Serial('/dev/tty.usbserial-110', 9600)  # Replace 'XXXXXXXX' with your USB-to-Serial adapter port

try:
    while True:
        # Read a line of serial data
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
except KeyboardInterrupt:
    # Close the serial port when Ctrl+C is pressed
    ser.close()
