ifneq ($(V), 1)
MAKEFLAGS += --no-print-dir
Q := @
endif

PROJECT_NAME := magknob

# Collect all the source files.
SRC :=
SRC += board.c
SRC += printf.c
SRC += main.c
SRC += as5601.c
SRC += profile_trace.c

OBJ = $(SRC:.c=.o)

INCLUDE_PATHS :=
INCLUDE_PATHS += -I../libopencm3/include


#                        #
#        COMPILER        #
#                        #

PREFIX ?= arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
OBJCOPY = $(PREFIX)objcopy

OPT_FLAGS ?= -O3

# -Wall
# -Werror - every warning is an error
# -MD - generate dependencies and compile
# -Wno-incompatible-pointer-types
CFLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS += -Wall -Wextra -Werror -Wno-char-subscripts -Wno-unused-but-set-variable -Wno-unused-function
CFLAGS += -DSTM32F1 -DDISCOVERY_STLINK $(INCLUDE_PATHS)
CFLAGS += -std=gnu11 -fstack-usage
CFLAGS += $(OPT_FLAGS) -g3


#                      #
#        LINKER        #
#                      #

LDFLAGS :=
LDFLAGS += --specs=nano.specs
#LDFLAGS += --specs=nosys.specs
# Libs.
LDFLAGS += -lopencm3_stm32f1
LDFLAGS += -L../libopencm3/lib
# Compiler flags on the linking stage.
LDFLAGS += -mthumb -mcpu=cortex-m3
LDFLAGS += -nostartfiles
LDFLAGS += -lc  # use libc
#LDFLAGS += -nolibc
#LDFLAGS += -nostdlib
# Linker flags
# Stack grows downwards from the end of RAM (0x2000_0000 + 0x5000).
# The RAM size = 20480 B. In hex that's 0x5000. This symbol is also defined in the
# cortex-m-generic.ld.
LDFLAGS += -Wl,--defsym,_stack=0x20005000
LDFLAGS += -Wl,-T,memory.ld
LDFLAGS += -Wl,-Map=mapfile
LDFLAGS += -Wl,-gc-sections
LDFLAGS += -Wl,--print-memory-usage

LDFLAGS += $(OPT_FLAGS)



#                       #
#        TARGETS        #
#                       #

all: $(PROJECT_NAME).bin

host_clean:
	-$(Q)$(RM) $(PROJECT_NAME).bin


$(PROJECT_NAME).elf: version.h $(OBJ)
	@echo "  LD      $@"
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	@echo "  CC      $<"
	$(CC) $(CFLAGS) -c $< -o $@

%.bin: %.elf
	@echo "  OBJCOPY $@"
	$(OBJCOPY) -O binary $^ $@

%.hex: %
	@echo "  OBJCOPY $@"
	$(OBJCOPY) -O ihex $^ $@


.PHONY: clean host_clean FORCE

clean: host_clean
	$(Q)echo "  CLEAN"
	-$(Q)$(RM) -f *.o *.d *~ *.elf *.bin *.su $(HOSTFILES)
	-$(Q)$(RM) -f mapfile version.h

version.h: FORCE
	$(Q)echo "  GIT     version.h"
	$(Q)echo "#define FIRMWARE_VERSION \"`git describe --always --dirty`\"" > $@

#TODO(michalc): this doesn't work yet.
size: FORCE
	$(Q)echo "The size of $(PROJECT_NAME).bin: " `stat -L -c %s $(PROJECT_NAME).bin` "B"

-include *.d
