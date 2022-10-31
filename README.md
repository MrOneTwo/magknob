[![builds.sr.ht status](https://builds.sr.ht/~mronetwo/magknob.svg)](https://builds.sr.ht/~mronetwo/magknob?)

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

## Technical information

The linker script is the `src/memory.ld` which includes `cortex-m-generic.ld`, from *libopencm3*.

The startup code lives in the `libopencm3/lib/cm3/vector.c`. It also provides a structure that
provides the stack address and the handlers.

You can check the loaded objects in the `src/mapfile`, example:

```
LOAD board.o
LOAD printf.o
LOAD main.o
LOAD as5601.o
LOAD profile_trace.o
LOAD ../libopencm3/lib/libopencm3_stm32f1.a
LOAD /home/mc/gits/magknob/toolchain/gcc-arm-none-eabi-9-2020-q2-update/bin/../lib/gcc/arm-none-eabi/9.3.1/../../../../arm-none-eabi/lib/thumb/v7-m/nofp/libc_nano.a
```
