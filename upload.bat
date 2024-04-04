@echo off

:: change these
set AVRDUDE_CONF="C:\Users\Stefan\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf"
set PORT="COM7"

avrdude -C%AVRDUDE_CONF% -v -V -p atmega328p -c arduino "-P%PORT%" -b115200 -D "-Uflash:w:bin\firmware.hex"

