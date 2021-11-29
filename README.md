# MagKnob

This project is a knob for controlling volume (or anything that a USB keyboard can).
It uses a *AS5601* sensor as an encoder. It was developed using
[Bluepill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html).

## Building this project

The quickest way is to use *just*:

```sh
git clone https://git.sr.ht/~mronetwo/magknob
just setup_toolchain
make
bash flash.sh src/magknob.bin
```
