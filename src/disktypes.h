/*
	flomod - a floppy disk image editor
	Copyright (C) 2017 Jacek Wieczorek <mrjjot@gmail.com>
	This file is part of flomod.
	Flomod is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Flomod is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DISKTYPES_H
#define DISKTYPES_H
#include "chsb.h"

#define DISKTYPE_FLOPPY_35_740	0
#define DISKTYPE_FLOPPY_35_144	1
#define DISKTYPE_FLOPPY_35_288	2
#define DISKTYPE_FLOPPY_525_12	3
#define DISKTYPE_FLOPPY_525_360 4

const CHSB disktypes[] =
{
	//3.5 inch, 720KB floppy
	[DISKTYPE_FLOPPY_35_740] = {
		.c = 80,
		.h = 2,
		.s = 9,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_3.5_740K",
		.flags = 0,
	},

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
	},

	//5.25 inch, 1.2MB floppy
	[DISKTYPE_FLOPPY_525_12] = {
		.c = 80,
		.h = 2,
		.s = 15,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_5.25_1.2M",
		.flags = 0,
	},

	//5.25 inch, 360KB floppy
	[DISKTYPE_FLOPPY_525_360] = {
		.c = 40,
		.h = 2,
		.s = 9,
		.b = 512,
		.offset = 0,
		.str = "FLOPPY_5.25_360K",
		.flags = 0,
	},
};

#define DISKTYPE_COUNT ( (int) ( sizeof( disktypes ) / sizeof( disktypes[0] ) ) )

#endif
