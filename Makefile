CFLAGS+=-Wall -ffreestanding -mthumb -march=armv4t -mtune=arm920t -Os \
	-mabi=aapcs -mfloat-abi=softfp -nostartfiles -static -nostdlib \
	-g -Wextra
CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)as
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump
LD=$(CROSS_COMPILE)ld

# Specific TS-7300 options
ifeq ($(DBG_ENABLE),1)
CFLAGS+= -DDEBUG
endif
SDRAM_SIZE_MB?=32
CFLAGS+= -DSDRAM_SIZE_MB=$(SDRAM_SIZE_MB)

drop-sections   = .comment .comments .ARM.exidx
strip-flags     = $(addprefix --remove-section=,$(drop-sections)) \
		  --set-section-flags .bss=alloc,load,contents

all: sdboot.bin sdboot.dis

sdboot: sdboot.c sdboot.lds mbr_defs.h atags_defs.h
	$(CC) $(CFLAGS) -T sdboot.lds -o $@ $<

sdboot.bin: sdboot
	$(OBJCOPY) -O binary $(strip-flags) $^ $@
	if [ "$$(wc -c <$@)" -gt "446" ]; then \
		echo "$@ is too big to fit in MBR!"; \
		exit 1; \
	fi

sdboot.dis: sdboot
	$(OBJDUMP) -S $< > $@

clean:
	rm -fr *.bin sdboot *.o sdboot.dis
