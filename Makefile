CC = avr-g++
OBJCOPY = avr-objcopy
MCU = atmega328p
F_CPU = 16000000UL
PORT = COM6
PROGRAMMER = arduino

CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=c++17

SRC_DIR = src
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

all: $(BIN_DIR)/firmware.hex

$(BIN_DIR)/firmware.hex: $(BIN_DIR)/firmware.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BIN_DIR)/firmware.out: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
