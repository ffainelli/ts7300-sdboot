CFLAGS+=-Wall -ffreestanding -mthumb -march=armv4t -mtune=arm920t -Os \
	-mabi=aapcs -mfloat-abi=softfp -nostartfiles -static -nostdlib
CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)as
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump
LD=$(CROSS_COMPILE)ld

drop-sections   = .comment .comments .ARM.exidx
strip-flags     = $(addprefix --remove-section=,$(drop-sections)) \
		  --set-section-flags .bss=alloc,load,contents

all: sdboot.bin sdboot.dis

sdboot: sdboot.c sdboot.lds
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
