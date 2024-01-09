# Cansat Firmware

This is the firmare, of the Cansat of the team Lunateeks, for the 2023-2024 edition of the Romanian Cansat and Rocketry Competition.

## Usage

This project uses GNU Make as a primary build system. It requires the AVR toolchain to build the project.

1. Compile the project

This will generate a `./bin/firmware.hex` file.

```
$ make
```

2. Upload the firmare to the Arduino

Modify the `upload.bat` script first to update the avrdude config path and the port.

```
$ upload
```

Make sure the Arduino UNO is connected to the PC when running this command!

## License

This work is under the MIT license and such it requires the preservation of copyright and license notices.
