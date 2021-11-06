ifneq ($(V), 1)
MFLAGS += --no-print-dir
Q := @
endif

_TOOLCHAIN_PATH ?= $(shell pwd)/toolchain/gcc-arm-none-eabi-9-2020-q2-update/bin/

_PREFIX := arm-none-eabi-
ifneq ($(_TOOLCHAIN_PATH),)
  _PREFIX := $(_TOOLCHAIN_PATH)/$(_PREFIX)
endif

firmware: lib
	#
	# Building firmware.
	#
	$(Q)$(MAKE) $(MFLAGS) -C src PREFIX=$(_PREFIX)
	# Verify the size of the resulting binary.
	$(Q)$(MAKE) $(MFLAGS) -C src size PREFIX=$(_PREFIX)

lib: submodules
	#
	# Building libopencm3 lib!
	#
	$(Q)$(MAKE) $(MFLAGS) -C libopencm3 V=0 lib PREFIX=$(_PREFIX)

submodules:
	$(Q)if [ ! -f libopencm3/Makefile ]; then \
		echo "Initialising git submodules..." ;\
		git submodule init ;\
		git submodule update ;\
	fi

clean:
	$(Q)$(MAKE) $(MFLAGS) -C src $@
	
clean_all: clean
	$(Q)$(MAKE) $(MFLAGS) -C libopencm3 clean
