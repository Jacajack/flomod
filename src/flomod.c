#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "chsb.h"

#define FLOMOD_VERSION "v0.0"

struct
{
	char *exename;
	char *outfilename;

	FILE *diskfile;
	CHSB start;
	CHSB length;
	CHSB end;
	CHSB limits;
} flomod;

void help( )
{
	fprintf( stderr,
		"%s - floppy disk editor " FLOMOD_VERSION "\n" \
		"Usage: %s filename [OPTIONS]\n" \
		, flomod.exename, flomod.exename );
}

//void parsearg( int *status, const char )

int main( int argc, char **argv )
{
	/*
	CHSB a = {0, 0, 0, 0};
	CHSB lim = {80, 2, 18, 512};
	printf( "(%d)\n", str2chsb( &a, argv[1] ) );
	printf( "%ld:%ld:%ld:%ld\n", a.c, a.h, a.s, a.b );
	chsbopt( &a, &lim );
	printf( "%ld:%ld:%ld:%ld\n", a.c, a.h, a.s, a.b );
	a.flags |= CHSB_FLAG_OFFSET_BYTES;
	chsb2lba( &a, &lim );
	printf( "%ld\n", a.offset );
	lba2chsb( &a, &lim );
	printf( "%ld:%ld:%ld:%ld\n", a.c, a.h, a.s, a.b );
	*/

	int i;
	unsigned char badarg, badval;

	if ( argc >= 1 ) flomod.exename = argv[0];

	if ( argc <= 2 )
	{
		help( );
		exit( 1 );
	}

	for ( i = 2; i < argc; i++ )
	{
		badval = 0;
		badarg = 1;

		if ( !strcmp( argv[i], "-o" ) && ( ++i >= argc || \
			!sscanf( argv[i], "%[^\t\n]", flomod.outfilename ) ) ) {badval = 1; badarg = 0;}


		if ( badarg )
		{
			fprintf( stderr, "%s: unknown option '%s'\n", flomod.exename, argv[i] );
			exit( 1 );
		}

		if ( badval )
		{
			fprintf( stderr, "%s: bad value for '%s'\n", flomod.exename, argv[i - 1] );
			exit( 1 );
		}
	}

	return 0;
}
