REPO_PATH := `git rev-parse --show-toplevel`

TOOLCHAIN_URL := 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2?revision=05382cca-1721-44e1-ae19-1e7c3dc96118&la=en&hash=D7C9D18FCA2DD9F894FD9F3C3DC9228498FA281A'
TOOLCHAIN_ARCHIVE_NAME := 'tc.tar.bz2'
TOOLCHAIN_DEST_DIR := REPO_PATH + '/toolchain'

TOOLCHAIN_PATH := REPO_PATH + "/toolchain/gcc-arm-none-eabi-9-2020-q2-update/bin"

# Flash the ELF onto the target board.
flash fw="src/magknob.bin":
  st-flash write {{fw}} 0x08000000

# Fetch the gcc-arm-none-eabi toolchain.
setup_toolchain: _unpack_toolchain

# Unpack the toolchain archive.
_unpack_toolchain: _download_toolchain
  cd {{TOOLCHAIN_DEST_DIR}} && tar xj -f {{TOOLCHAIN_ARCHIVE_NAME}}
  rm {{TOOLCHAIN_DEST_DIR}}/{{TOOLCHAIN_ARCHIVE_NAME}}

# Download the toolchain archive.
_download_toolchain:
  mkdir -p toolchain
  wget -q -O '{{TOOLCHAIN_DEST_DIR}}/{{TOOLCHAIN_ARCHIVE_NAME}}' '{{TOOLCHAIN_URL}}'

TRACECLKIN := '8000000'
CPU_CLOCK := '72000000'
BAUD_RATE := '2000000'

# Start GDB session connected to the OpenOCD.
dbg_client elf_file="src/pawusb.elf":
  {{TOOLCHAIN_PATH}}/arm-none-eabi-gdb-py -ex 'target extended-remote localhost:3333' {{elf_file}}

dbg_server_external:
  openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c 'tpiu config external uart off {{CPU_CLOCK}} {{BAUD_RATE}}'

dbg_server_internal:
  openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c 'tpiu config internal itm.fifo uart off {{TRACECLKIN}}'
