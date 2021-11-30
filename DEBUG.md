The firmware uses ITM of Cortex-M. When developing I was using a
debug adapter without SWO pin which means I have to push the ITM
data to a UART->USB.


Start OpenOCD:

```sh
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c 'tpiu config external uart off 72000000 2000000'
```

## Profiling

All the *telnet* commands can be done in the GDB by prepending the
commands with `monitor`.
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
