[![builds.sr.ht status](https://builds.sr.ht/~mronetwo.svg)](https://builds.sr.ht/~mronetwo?)

# MagKnob

This project is a knob for controlling volume (or anything that a USB keyboard can).
It uses a *AS5601* sensor as an encoder. It was developed using
[Bluepill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html).

You can download [OpenSCAD designed models for 3D printing](https://git.sr.ht/~mronetwo/magknob-oscad).

## Building this project

Dependencies:

- wget
- [just](https://github.com/casey/just)
- [stlink](https://github.com/stlink-org/stlink)
- [OpenOCD](https://openocd.org/)

The quickest way is to use *just*:

```sh
git clone https://git.sr.ht/~mronetwo/magknob
just setup_toolchain
make
bash flash.sh src/magknob.bin
```
