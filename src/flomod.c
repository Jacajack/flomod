#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "chsb.h"
#include "disktypes.h"

#define FLOMOD_VERSION "v0.0"

#define FLOMOD_FLAG_WRITE 1
#define FLOMOD_FLAG_READ 2
#define FLOMOD_FLAG_SECTOR_BASE1 2

struct
{
	unsigned int flags;
	char *exename;

	char *disktype;
	char *diskfname;
	FILE *diskf;
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

	if ( argc < 2 )
	{
		help( );
		exit( 1 );
	}

	for ( i = 2; i < argc; i++ )
	{
		badval = 1;
		badarg = 1;

		if ( !strcmp( argv[i], "-s" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.start.str = argv[i];

		if ( !strcmp( argv[i], "-n" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.length.str = argv[i];

		if ( !strcmp( argv[i], "-l" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.limits.str = argv[i];

		if ( !strcmp( argv[i], "-e" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.end.str = argv[i];

		if ( !strcmp( argv[i], "-t" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.disktype = argv[i];

		//if ( !strcmp( argv[i], "-o" ) )
		//	if ( badarg = 0, i++ < argc )


		if ( !strcmp( argv[i], "-w" ) ) badarg = badval = 0, flomod.flags |= FLOMOD_FLAG_WRITE;
		if ( !strcmp( argv[i], "-r" ) ) badarg = badval = 0, flomod.flags |= FLOMOD_FLAG_READ;

		//if ( !strcmp( argv[i], "-o" ) )
		//	if ( ++i < argc )
		//	!sscanf( argv[i], "%[^\t\n]", flomod.outfilename ) ) ) {badval = 1; badarg = 0;}


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

	str2chsb( &flomod.limits );

	if ( chsbnull( &flomod.limits ) )
	{
		if ( flomod.disktype == NULL ) flomod.limits = disktypes[DISKTYPE_FLOPPY_3_5];
		else
		{
			for ( i = 0; i < DISKTYPE_COUNT; i++ )
			{
				if ( !strcmp( disktypes[i].str, flomod.disktype ) )
				{
					flomod.limits = disktypes[i];
					break;
				}
			}
			if ( i == DISKTYPE_COUNT ) flomod.limits = disktypes[DISKTYPE_FLOPPY_3_5];
		}
	}

	str2chsb( &flomod.start );
	chsbopt( &flomod.start, &flomod.limits );
	chsb2lba( &flomod.start, &flomod.limits );

	str2chsb( &flomod.length );
	chsbopt( &flomod.length, &flomod.limits );
	chsb2lba( &flomod.length, &flomod.limits );

	str2chsb( &flomod.end );
	if ( chsbnull( &flomod.end ) )
	{
		flomod.end = flomod.start;
		chsbsum( &flomod.end, &flomod.length );
	}
	chsbopt( &flomod.end, &flomod.limits );
	chsb2lba( &flomod.end, &flomod.limits );


	//Open disk file
	flomod.diskfname = argv[1];
	if ( flomod.flags & FLOMOD_FLAG_READ )
		flomod.diskf = fopen( flomod.diskfname, "rb" );
	else if ( flomod.flags & FLOMOD_FLAG_WRITE )
	{
		if ( access( flomod.diskfname, F_OK ) != -1 )
			flomod.diskf = fopen( flomod.diskfname, "r+b" );
		else flomo
		d.diskf = fopen( flomod.diskfname, "wb" );
	}
	if ( flomod.diskf == NULL )
	{
		fprintf( stderr, "%s: could not open '%s'\n", flomod.exename, flomod.diskfname );
		exit( 1 );
	}

	fseek( flomod.diskf, flomod.start.offset, SEEK_SET );
	fputc( 'a', flomod.diskf );

	fclose( flomod.diskf );
	return 0;
}
