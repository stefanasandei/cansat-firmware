CC = avr-g++
OBJCOPY = avr-objcopy
MCU = atmega328p
PORT = COM6
PROGRAMMER = arduino

CFLAGS = -Wall -Os -mmcu=$(MCU) -std=c++17

SRC_DIR = src
BIN_DIR = bin

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))
OBJ_DIRS := $(sort $(dir $(OBJECTS)))

all: $(BIN_DIR)/firmware.hex

$(BIN_DIR)/firmware.hex: $(BIN_DIR)/firmware.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BIN_DIR)/firmware.out: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIRS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
