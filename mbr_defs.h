#ifndef __SDBOOT_MBR_H
#define __SDBOOT_MBR_H

#define NUM_PE	4

struct mbr_chs {
	char	head;
	char sect_cyl;
	char cyl;
};

struct mbr_part {
	char	status;
	struct mbr_chs first;
	char type;
	struct mbr_chs last;
	unsigned int lba;
	unsigned int num_sects;
} __attribute__((__packed__));

struct mbr {
	char bootcode[446];
	struct mbr_part pe[NUM_PE];
	unsigned short sig;
};

#define PART_TYPE_OTHER_DATA	0xda
#define MBR_SIG			0xAA55

#define SECT_SIZE		512

#endif /* __SDBOOT_MBR_H */
