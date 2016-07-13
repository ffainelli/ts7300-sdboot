CFLAGS+=-Wall -ffreestanding -mthumb -march=armv4t -mtune=arm920t -Os \
	-mabi=aapcs -mfloat-abi=softfp -nostartfiles -static -nostdlib
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld

drop-sections   = .comment .comments .ARM.exidx
strip-flags     = $(addprefix --remove-section=,$(drop-sections))

all: sdboot.bin

sdboot: sdboot.c
	$(CC) $(CFLAGS) -T sdboot.lds -o $@ $<

sdboot.bin: sdboot
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents \
		$(strip-flags) $^ $@

clean:
	rm -fr *.bin sdboot *.o
