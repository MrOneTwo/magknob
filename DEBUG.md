This document describes how I debug, trace and profile this
firmware.

The firmware uses [ITM of ARM Cortex-M](https://developer.arm.com/documentation/ddi0337/e/BABCCDFD).
When developing, I was using a debug adapter without SWO pin
which meant I had to push the ITM data in a UART format, via
a UART to USB converter.

## Debugging

In order to debug, trace or profile you have to start OpenOCD
server.

```sh
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c 'tpiu config external uart off 72000000 2000000'
```

If you want to step through your program then you should use
GDB as OpenOCD's client.

```sh
./toolchain/gcc-arm-none-eabi-9-2020-q2-update/binl/arm-none-eabi-gdb-py \
  -ex 'target extended-remote localhost:3333' src/magknob.elf
```

`target extended-remote localhost:3333` means you've connected
to the OpenOCD. [Commands specific for OpenOCD](https://openocd.org/doc/html/General-Commands.html)
(plus [these](https://openocd.org/doc/html/Architecture-and-Core-Commands.html))
can be run from GDB by starting the command with the `monitor`
keyword.

## Profiling

Executing OpenOCD's commands in the *telnet* session don't
require the `monitor` keyword.
In order to sample PC for performance measurements:

```sh
telnet 0.0.0.0 4444
(telnet) tpiu config external uart off 72000000 2000000
(telnet) mww 0xE0001000 0x1207 0x103FF
(telnet) itm port 0 on
(telnet) reset
```

Save the output to a file

```sh
picocom /dev/ttyUSB0 -b 2000000 -g itm.uart
```

You can use [itm-tool](https://github.com/japaric/itm-tools) to
read that data:

```sh
itm-tools/target/debug/pcsampl itm.uart -e magknob/src/magknob.elf
```

The output for this project might look like:

```
MalformedPacket { header: 0, len: 1 }
    % FUNCTION
 0.00 *SLEEP*
75.12 st_usbfs_poll
13.79 main
 9.81 usbd_poll
 1.14 i2c_transfer7
 0.06 _vsnprintf
 0.02 snprintf_
 0.02 _out_rev
 0.02 sys_tick_handler
 0.01 _ntoa_format
 0.01 st_usbfs_copy_to_pm
 0.01 i2c_send_start
 0.00 usbd_ep_write_packet
 0.00 _out_buffer
 0.00 usb_standard_get_descriptor
 0.00 as5601_set_watchdog
 0.00 usb_control_send_chunk
 0.00 st_usbfs_ep_read_packet
-----
100% 606129 samples
```
