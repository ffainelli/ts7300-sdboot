/*
 * From: http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html
 *
 */
#ifndef __ATAGS_DEFS_H
#define __ATAGS_DEFS_H

#include <inttypes.h>

struct atag_header {
	/* Length of the tag in words including this header */
	uint32_t	size;
	/* tag value */
	uint32_t	tag;
};

#define ATAG_NONE	0x00000000
#define ATAG_CORE	0x54410001

struct atag_core {
	uint32_t flags;		/* bit 0 = read-only */
	uint32_t pagesize;
	uint32_t rootdev;
};

#define ATAG_RAMDISK	0x54410004

struct atag_ramdisk {
	uint32_t flags;
	uint32_t size;
	uint32_t start;
};

#define ATAG_INITRD2	0x54420005

struct atag_initrd2 {
	uint32_t start;
	uint32_t size;
};

#define ATAG_MEM	0x54410002

struct atag_mem {
	uint32_t	size;
	/* physical start address */
	uint32_t	start;
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

#define atag_next(t)	((struct atag *)((uint32_t *)(t) + (t)->hdr.size))
#define atag_size(type)	((sizeof(struct atag_header) + sizeof(struct type)) >> 2)

#endif /* __ATAGS_DEFS_H */
