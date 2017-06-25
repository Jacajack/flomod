#ifndef DISKTYPES_H
#define DISKTYPES_H
#include "chsb.h"

#define DISKTYPE_FLOPPY_35_144 0
#define DISKTYPE_FLOPPY_35_288 1

const CHSB disktypes[] =
{
	//3.5 inch, 1.44MB floppy
	[DISKTYPE_FLOPPY_35_144] = {
		.c = 80,
		.h = 2,
		.s = 18,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_3.5_1.44M",
		.flags = 0,
	},

	//3.5 inch, 2.88MB floppy
	[DISKTYPE_FLOPPY_35_288] = {
		.c = 80,
		.h = 2,
		.s = 36,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_3.5_2.88M",
		.flags = 0,
	}
};

#define DISKTYPE_COUNT ( (int) ( sizeof( disktypes ) / sizeof( disktypes[0] ) ) )

#endif
