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

#ifndef FLOMOD_H
#define FLOMOD_H
#include <stdio.h>
#include "chsb.h"

#define FLOMOD_FLAG_WRITE 1
#define FLOMOD_FLAG_SECTOR_BASE1 2
#define FLOMOD_FLAG_DEFAULT_FLOPPY 4
#define FLOMOD_FLAG_VERBOSE 8
#define FLOMOD_FLAG_DUMP 16

//Flomod configuration structure
struct flomod
{
	unsigned int flags;
	const char *exename;

	char *disktype;
	char *diskfname;
	FILE *diskf;
	long disklen;
	CHSB start;
	CHSB length;
	CHSB end;
	CHSB geom;
};

#endif
