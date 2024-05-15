#!/bin/bash

# Arduino settings
MCU="atmega328p"
PROGRAMMER="arduino"

# Paths
BIN_DIR="bin"
FIRMWARE="$BIN_DIR/firmware.hex"

# AVRDUDE settings
AVRDUDE="avrdude" 

# Function to find Arduino port
find_arduino_port() {
    port=$(ls /dev/cu.* | grep -E 'usb|ACM|ttyUSB|ttyACM' | head -n 1)
    echo "$port"
}

# Upload firmware
echo "Finding Arduino port..."
PORT=$(find_arduino_port)
if [ -z "$PORT" ]; then
    echo "Error: Arduino port not found."
    exit 1
fi

echo "Arduino found on port: $PORT"
echo "Uploading firmware to Arduino..."
$AVRDUDE -v -p $MCU -c $PROGRAMMER -P $PORT -D -U flash:w:$FIRMWARE:i
