# This script starts an OpenOCD which configures the ITM port. There are two
# targets of this script: 'internal' and 'external'.
# - internal - the traces are read via the programmer's SWO pin.
#              The traces can be piped into OpenOCD's console or into a file.
# - external - different device than the programmer is connected to the SWO pin.
#              With protocol set to 'uart' you can use a typical UART to USB dongle.

# TRACECLKIN determines the tpiu frequency - 8MHz should be fine.
# tpiu command configures the actual MCU.

if [ "$1" == "internal" ]; then
  # This method needs a programmer with working SWO line. You can change the itm.fifo into '-' to get output in OpenOCD.
  openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c 'tpiu config internal itm.fifo uart off 8000000'
elif [ "$1" == "external" ]; then
  # This method needs an additional UART to USB converter with Rx connected to PB3.
  # Use a baudrate = 2000000 when starting picocom (`picocom /dev/ttyUSB0 -b 2000000`).
  openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c 'tpiu config external uart off 72000000 2000000'
else
  echo "internal or external, darling?"
fi
