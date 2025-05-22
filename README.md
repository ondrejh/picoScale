# ToDo

- [x] Connect scale sensor.
- [x] Create global JSON data structure.
- [x] Test scale sensor, make it non-blocking.
- [ ] Filter sensor data (either temp or scale)
- [ ] Connect temperature sensor.
- [x] Add sensors to json response.
- [x] Use WS2812 led, instead of pico buildin.
- [x] Add some JSON parser or library to the project.
- [x] Make standalone USB serial Hello World program.

- [x] Build simulation for PC.


# Schematic

RP2040 Zero (WaveShare) | Signal
--- | ---
GP2 | HX711 SCK
GP3 | HX711 DT (connect with voltage divider 5V / 3.3V)
GP16 | WS2812 RGB LED DIN
GP29 | DS18B20 DATA


# Notes

## Prepare

```code
git submodule update --init
```

## Compiling

```code
mkdir build
cd build
cmake ../src/
make
```

## Compiling simulation

```code
mkdir simul
cd simul
cmake ../src -DBUILD_FOR_PICO=OFF
make
```

## Installing picotool locally

To get rid of annoyng message "it is recommended to build and install picotool separately", we can install it separately (it become a submodule of project).

```code
cd ext/picotool
git submodule update --init
sudo apt update
sudo apt install build-essential pkg-config libusb-1.0-0-dev cmake
mkdir build
cd build
cmake .. -DPICO_SDK_PATH=../../pico-sdk
sudo make install
picotool version
```
