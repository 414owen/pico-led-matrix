# Pico LED matrix

Some code to play around with a homemade LED matrix, and a raspberry pi pico.

![demo image](https://github.com/414owen/pico/blob/master/img/pico.jpg?raw=true)

The pico has GPIO pins connected to the rows and columns of the matrix, which
are connected to the LEDs' positives and negative rows.

Because of this configuration, it is impossible to, for example, turn on 0,0
and 1,1 without also turning on 0,1 and 1,0. We get around this by turning
on columns individually, faster than the eye can see, like a scanline.
We can control the brightness of LEDs by turning them on for some sub-frames,
and off for others.

## Environment

### Direnv + nix

```sh
direnv allow # only have to do this once
```

### Pure nix

```sh
nix-shell
```

### Other

Follow the instructions in the [getting started guide](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)

## Compiling

```sh
mkdir build
cd build
cmake ..
make ..
```

## Entering `BOOTSEL` mode

### Hard

Hold down the `BOOTSEL` button on the pico, and plug it in.

### Soft

The code in this repository will enter `BOOTSEL` mode when it reads an `r`
key over serial.

```sh
sudo screen /dev/ttyACM0 115200
r
```

## Running the code

When the pico is in `BOOTSEL` mode, it will mount as a storage device.
To run the compiled program, copy `main.uf2` to the device.

## Recommended workflow

Replace paths with ones that work on your system.

```sh
STORAGE_PATH=/run/media/owen/RPI-RP2
SERIAL_PATH=/dev/ttyACM0
make && while [ ! -d $STORAGE_PATH ]; do sleep 0.1; done && cp main.uf2 $STORAGE_PATH && sleep 2 && sudo screen $SERIAL_PATH 115200
```
