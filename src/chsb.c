#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "chsb.h"

void chsbopt( CHSB *desc, CHSB lim )
{
	//Optimize byte number
	desc->s += desc->b / lim.b;
	desc->b = desc->b % lim.b;

	//Optimize sector number
	if ( desc->flags & CHSB_FLAG_SECTOR_BASE1 )
	{
		//1-based sector numbers
		desc->h += ( desc->s - 1 ) / lim.s;
		desc->s = ( desc->s - 1 ) % lim.s + 1;
		if ( desc->s == 0 ) desc->s = 1;
	}
	else
	{
		//0-based sector numbers
		desc->h += desc->s / lim.s;
		desc->s = desc->s % lim.s;
	}
	//Optimize head number
	desc->c += desc->h / lim.h;
	desc->h = desc->h % lim.h;
}

CHSB chsbsum( CHSB *desc, CHSB val )
{
	desc->c += val.c;
	desc->h += val.h;
	desc->s += val.s;
	desc->b += val.b;
	return *desc;
}

//Convert CHSB structure to offset in sectors or bytes
uint64_t chsb2lba( CHSB desc, CHSB lim )
{
	uint64_t offset = ( desc.c * lim.h + desc.h ) * lim.s + desc.s;
	if ( desc.flags & CHSB_FLAG_SECTOR_BASE1 ) offset -= 1;
	if ( desc.flags & CHSB_FLAG_OFFSET_BYTES ) offset = offset * lim.b + desc.b;
	return offset;
}

//Convert colon-separated CHSB string to CHSB structure
int str2chsb( CHSB *desc, char *s )
{
	unsigned char ec = 1;
	char *delim = ":";
	char *str = NULL;
	char *token = NULL;

	if ( s == NULL ) return 1;
	str = strdup( s );
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
