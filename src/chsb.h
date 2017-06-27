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

#ifndef CHSB_H
#define CHSB_H
#include <inttypes.h>

#define CHSB_FLAG_SECTOR_BASE1 1

//CHSB structure
typedef struct
{
	long c;
	long h;
	long s;
	long b;
	long offset;
	const char *str;
	unsigned int flags;
} CHSB;

extern void chsbopt( CHSB *desc, CHSB *lim );
extern void chsbsum( CHSB *desc, CHSB *val );
extern int chsbnull( CHSB *desc );
extern void chsb2lba( CHSB *desc, CHSB *lim );
extern void lba2chsb( CHSB *desc, CHSB *lim );
extern int str2chsb( CHSB *desc );
extern const char *chsb2str( CHSB *desc );

#endif
