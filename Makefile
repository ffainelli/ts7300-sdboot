CFLAGS+=-Wall -ffreestanding -mthumb -march=armv4t -mtune=arm920t -Os \
	-mabi=aapcs -mfloat-abi=softfp -nostartfiles -static -nostdlib
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump
LD=$(CROSS_COMPILE)ld

drop-sections   = .comment .comments .ARM.exidx
strip-flags     = $(addprefix --remove-section=,$(drop-sections))

all: sdboot.bin sdboot.dis

sdboot: sdboot.c
	$(CC) $(CFLAGS) -T sdboot.lds -o $@ $<

sdboot.bin: sdboot
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents \
		$(strip-flags) $^ $@

sdboot.dis: sdboot
	$(OBJDUMP) -S $< > $@

clean:
	rm -fr *.bin sdboot *.o sdboot.dis
