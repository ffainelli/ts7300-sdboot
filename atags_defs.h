/*
 * From: http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html
 *
 */
#ifndef __ATAGS_DEFS_H
#define __ATAGS_DEFS_H

struct atag_header {
	/* Length of the tag in words including this header */
	unsigned int	size;
	/* tag value */
	unsigned int	tag;
};

#define ATAG_NONE	0x00000000
#define ATAG_CORE	0x54410001

struct atag_core {
	unsigned int flags;		/* bit 0 = read-only */
	unsigned int pagesize;
	unsigned int rootdev;
};

#define ATAG_RAMDISK	0x54410004

struct atag_ramdisk {
	unsigned int flags;
	unsigned int size;
	unsigned int start;
};

#define ATAG_INITRD2	0x54420005

struct atag_initrd2 {
	unsigned int start;
	unsigned int size;
};

#define ATAG_MEM	0x54410002

struct atag_mem {
	unsigned int	size;
	/* physical start address */
	unsigned int	start;
};

struct atag {
	struct atag_header hdr;
	union {
		struct atag_core	core;
		struct atag_mem		mem;
		struct atag_ramdisk	ramdisk;
		struct atag_initrd2	initrd2;
	} u;
};

#define atag_next(t)	((struct atag *)((unsigned int *)(t) + (t)->hdr.size))
#define atag_size(type)	((sizeof(struct atag_header) + sizeof(struct type)) >> 2)

#endif /* __ATAGS_DEFS_H */
