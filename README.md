# ToDo

- [ ] Connect scale sensor.
- [x] Add some JSON parser or library to the project.
- [x] Make standalone USB serial Hello World program.

# Compiling

```code
mkdir build
cd build
cmake ../src/
make proj
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
