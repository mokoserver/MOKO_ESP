@echo off
python -m esptool --port COM10 --baud 921600 write_flash --flash_mode dio --flash_size 4MB 0x0 .pio\build\nodemcuv2\firmware.bin
pause