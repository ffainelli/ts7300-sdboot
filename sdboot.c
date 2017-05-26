/*
 * TS-7300 SD card bootloader replacement
 *
 * Copyright (C) 2016, Florian Fainelli <f.fainelli@gmail.com>
 *
 * Note: this must fit within 446 bytes of a MBR
 *
 * See LICENSE for details
*
 * Here is the state in which we start:
 *
 * R0 points to the sd_read function
 * R1 points to the ser_puts function
 *
 * The stack pointer is initialized in SRAM
 */
#include <inttypes.h>

#include "mbr_defs.h"
#include "atags_defs.h"

/* First EP93xx SDRAM bank start at PA 0x0 */
#define SDRAM_START_ADDR	0

#define ATAGS_OFFSET		(SDRAM_START_ADDR + 0x100)
#define MBR_LOADADDR		(SDRAM_START_ADDR + 0x1000)
#define LINUX_LOADADDR		(SDRAM_START_ADDR + 0x218000)
#define INITRD_LOADADDR		(SDRAM_START_ADDR + 0x1000000)

/* From arch/arm/tools/mach-types */
#define TS72XX_MACH_NUM		673

static const struct mbr *mbr = (struct mbr *)MBR_LOADADDR;
static struct atag *atags = (struct atag *)ATAGS_OFFSET;

#ifdef DEBUG
#define dbg_str(x)	ser_puts(x); ser_puts("\n\r");
#else
#define dbg_str(x)	do { } while (0);
#endif

static void die(void)
{
	while (1)
		;
}

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

int main(void (*sr_read)(unsigned int start_sector,
			char *buffer, int num_sectors),
	 void (*ser_puts)(const char *buffer))
{
	char *dst = (char *)LINUX_LOADADDR;
	const struct mbr_part *part;
	unsigned found = 0;
	unsigned int i;

	BUILD_BUG_ON(sizeof(struct mbr_part) != 16);
	BUILD_BUG_ON(sizeof(struct mbr) != SECT_SIZE);

	if (mbr->sig != MBR_SIG) {
		ser_puts("S");
		die();
	}

	for (i = 0; i < NUM_PE; i++) {
		part = &mbr->pe[i];

		/* First partition is Linux */
		if (part->type != PART_TYPE_OTHER_DATA) {
			ser_puts("P");
			continue;
		}

		ser_puts("L");
		sr_read(part->lba, dst, part->num_sects);
		ser_puts(".\r\n");
		found = 1;
		break;
	}

	if (!found) {
		dbg_str("N");
		die();
	}

	/* Prepare ATAGS */
	atags->hdr.tag = ATAG_CORE;
	atags->hdr.size = atag_size(atag_core);
	atags->u.core.flags = 1;
	atags->u.core.pagesize = 4096;
	atags->u.core.rootdev = 0;
	atags = atag_next(atags);

#if (SDRAM_SIZE_MB > 32)
#error "Need to add additional ATAG_MEM for second bank!"
#endif
	atags->hdr.tag = ATAG_MEM;
	atags->hdr.size = atag_size(atag_mem);
	atags->u.mem.start = 0;
	atags->u.mem.size = 8 << 20;
	atags = atag_next(atags);

	atags->hdr.tag = ATAG_MEM;
	atags->hdr.size = atag_size(atag_mem);
	atags->u.mem.start = 0x1000000;
	atags->u.mem.size = 8 << 20;
	atags = atag_next(atags);

	atags->hdr.tag = ATAG_MEM;
	atags->hdr.size = atag_size(atag_mem);
	atags->u.mem.start = 0x4000000;
	atags->u.mem.size = 8 << 20;
	atags = atag_next(atags);

	atags->hdr.tag = ATAG_MEM;
	atags->hdr.size = atag_size(atag_mem);
	atags->u.mem.start = 0x5000000;
	atags->u.mem.size = 8 << 20;
	atags = atag_next(atags);

	atags->hdr.tag = ATAG_NONE;
	atags->hdr.size = 0;

	/* Start kernel, bye! */
	ser_puts("G\r\n");

	void (*kernel_start)(int, int, uint32_t) = (void *)LINUX_LOADADDR;
	kernel_start(0, TS72XX_MACH_NUM, ATAGS_OFFSET);

	return 0;
}
