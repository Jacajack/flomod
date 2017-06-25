#ifndef DISKTYPES_H
#define DISKTYPES_H
#include "chsb.h"

#define DISKTYPE_FLOPPY_3_5 0

const CHSB disktypes[] =
{
	//3.5 inch, 1.44MB floppy
	[DISKTYPE_FLOPPY_3_5] = {
		.c = 80,
		.h = 2,
		.s = 18,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_3.5",
		.flags = CHSB_FLAG_SECTOR_BASE1,
	}
};

#define DISKTYPE_COUNT ( sizeof( disktypes ) / sizeof( disktypes[0] ) )

#endif
