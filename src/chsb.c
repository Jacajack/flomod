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

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "chsb.h"

void chsbopt( CHSB *desc, CHSB *lim )
{
	if ( desc == NULL || lim == NULL ) return;

	//Optimize byte number
	desc->s += desc->b / lim->b;
	desc->b = desc->b % lim->b;
	if ( desc->b < 0 ) desc->b = 0;

	//Optimize sector number
	if ( desc->flags & CHSB_FLAG_SECTOR_BASE1 )
	{
		//1-based sector numbers
		desc->h += ( desc->s - 1 ) / lim->s;
		desc->s = ( desc->s - 1 ) % lim->s + 1;
		if ( desc->s < 1 ) desc->s = 1;
	}
	else
	{
		//0-based sector numbers
		desc->h += desc->s / lim->s;
		desc->s = desc->s % lim->s;
		if ( desc->s < 0 ) desc->s = 0;
	}
	//Optimize head number
	desc->c += desc->h / lim->h;
	desc->h = desc->h % lim->h;
	if ( desc->h < 0 ) desc->h = 0;

	//Check cylinder number bounds
	if ( desc->c < 0 ) desc->c = 0;
}

//Sum two CHSB structures (optimization not included)
void chsbsum( CHSB *desc, CHSB *offset )
{
	if ( desc == NULL || offset == NULL ) return;
	desc->c += offset->c;
	desc->h += offset->h;
	desc->s += offset->s;
	desc->b += offset->b;
}

//Check if the CHSB struct is empty
int chsbnull( CHSB *desc )
{
	if ( desc == NULL ) return 1;
	return 	desc->c == 0 && \
			desc->h == 0 && \
			desc->s == 0 && \
			desc->b == 0;
}

//Convert CHSB structure to offset in sectors or bytes
void chsb2lba( CHSB *desc, CHSB *lim )
{
	if ( desc == NULL || lim == NULL ) return;
	desc->offset = ( desc->c * lim->h + desc->h ) * lim->s + desc->s;
	if ( desc->flags & CHSB_FLAG_SECTOR_BASE1 ) desc->offset -= 1;
	desc->offset = desc->offset * lim->b + desc->b;
}

//Convert offset in sectors or bytes to CHSB structure values
void lba2chsb( CHSB *desc, CHSB *lim )
{
	if ( desc == NULL || lim == NULL ) return;
	desc->c = desc->h = 0;
	desc->s = desc->flags & CHSB_FLAG_SECTOR_BASE1 ? 1 : 0;
	desc->b = desc->offset;
	chsbopt( desc, lim );
}

//Convert colon-separated CHSB string to CHSB structure
int str2chsb( CHSB *desc )
{
	unsigned char ec = 1;
	const char *delim = ":";
	char *str = NULL;
	char *token = NULL;

	if ( desc == NULL ) return 1;
	desc->c = 0;
	desc->h = 0;
	desc->s = 0;
	desc->b = 0;

	if ( desc->str == NULL ) return 1;
	str = strdup( desc->str );
	if ( str == NULL ) return 1;

	token = strtok( str, delim );
	if ( token != NULL && !sscanf( token, "%ld", &desc->c ) ) desc->c = 0;
	else ec = 0;

	token = strtok( NULL, delim );
	if ( token != NULL && !sscanf( token, "%ld", &desc->h ) ) desc->h = 0;
	else ec = 0;

	token = strtok( NULL, delim );
	if ( token != NULL && !sscanf( token, "%ld", &desc->s ) ) desc->s = 0;
	else ec = 0;

	token = strtok( NULL, delim );
	if ( token != NULL && !sscanf( token, "%ld", &desc->b ) ) desc->b = 0;
	else ec = 0;

	free( str );
	return ec;
}

//Generate new string representation for the CHSB structure
const char *chsb2str( CHSB *desc )
{
	const char *delim = ":";
	char str[64];

	if ( desc == NULL ) return NULL;

	snprintf( str, sizeof str, "%ld%s%ld%s%ld%s%ld", desc->c, delim, desc->h, delim, desc->s, delim, desc->b );

	desc->str = strdup( str );
	return desc->str;
}
