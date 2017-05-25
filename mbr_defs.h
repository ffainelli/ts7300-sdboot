#ifndef __SDBOOT_MBR_H
#define __SDBOOT_MBR_H

#include <inttypes.h>

#define NUM_PE	4

struct mbr_chs {
	uint8_t	head;
	uint8_t sect_cyl;
	uint8_t cyl;
};

struct mbr_part {
	uint8_t	status;
	struct mbr_chs first;
	uint8_t type;
	struct mbr_chs last;
	uint32_t lba;
	uint32_t num_sects;
} __attribute__((__packed__));

struct mbr {
	uint8_t bootcode[446];
	struct mbr_part pe[NUM_PE];
	uint16_t sig;
};

#define PART_TYPE_OTHER_DATA	0xda
#define MBR_SIG			0xAA55

#define SECT_SIZE		512

#endif /* __SDBOOT_MBR_H */
