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
#define FLOMOD_FLAG_SECTOR_BASE1 4
#define FLOMOD_FLAG_DEFAULT_FLOPPY 8

struct
{
	unsigned int flags;
	char *exename;

	char *disktype;
	char *diskfname;
	FILE *diskf;
	long disklen;
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
	int i, b;
	unsigned char badarg, badval;

	flomod.flags = FLOMOD_FLAG_SECTOR_BASE1;

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
				badval = 0, flomod.start.str = strndup( argv[i], 1024 );

		if ( !strcmp( argv[i], "-n" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.length.str = strndup( argv[i], 1024 );

		if ( !strcmp( argv[i], "-l" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.limits.str = strndup( argv[i], 1024 );

		if ( !strcmp( argv[i], "-e" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.end.str = strndup( argv[i], 1024 );

		if ( !strcmp( argv[i], "-t" ) )
			if ( badarg = 0, ++i < argc )
				badval = 0, flomod.disktype = strndup( argv[i], 1024 );

		//if ( !strcmp( argv[i], "-o" ) )
		//	if ( badarg = 0, i++ < argc )


		if ( !strcmp( argv[i], "-w" ) ) badarg = badval = 0, flomod.flags |= FLOMOD_FLAG_WRITE;
		if ( !strcmp( argv[i], "-r" ) ) badarg = badval = 0, flomod.flags |= FLOMOD_FLAG_READ;
		if ( !strcmp( argv[i], "-z" ) ) badarg = badval = 0, flomod.flags &= ~FLOMOD_FLAG_SECTOR_BASE1;

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

	//Setup CHSB limits
	str2chsb( &flomod.limits );
	if ( chsbnull( &flomod.limits ) ) //Skip if set by user
	{
		//Default disk type if not specified
		if ( flomod.disktype == NULL ) flomod.flags |= FLOMOD_FLAG_DEFAULT_FLOPPY;
		else
		{
			//If disk type specified - find it
			for ( i = 0; i < DISKTYPE_COUNT; i++ )
			{
				if ( !strcmp( disktypes[i].str, flomod.disktype ) )
				{
					flomod.limits = disktypes[i];
					break;
				}
			}

			//Not found - default once more
			if ( i == DISKTYPE_COUNT ) flomod.flags |= FLOMOD_FLAG_DEFAULT_FLOPPY;
		}
	}

	//Default disk type
	if ( flomod.flags & FLOMOD_FLAG_DEFAULT_FLOPPY )
	{
		flomod.limits = disktypes[DISKTYPE_FLOPPY_35_144];
		fprintf( stderr, "%s: disk type not specified - default is 3.5\" floppy\n", flomod.exename );
	}

	//Base 1 sector numeration
	if ( flomod.flags & FLOMOD_FLAG_SECTOR_BASE1 )
	{
		flomod.start.flags |= CHSB_FLAG_SECTOR_BASE1;
		flomod.length.flags |= CHSB_FLAG_SECTOR_BASE1;
		flomod.end.flags |= CHSB_FLAG_SECTOR_BASE1;
		flomod.limits.flags |= CHSB_FLAG_SECTOR_BASE1;
	}

	//Get start descriptor
	str2chsb( &flomod.start );
	chsbopt( &flomod.start, &flomod.limits );
	chsb2lba( &flomod.start, &flomod.limits );

	//Get length
	str2chsb( &flomod.length );
	chsbopt( &flomod.length, &flomod.limits );
	chsb2lba( &flomod.length, &flomod.limits );

	//Open disk file
	flomod.diskfname = argv[1];
	if ( flomod.flags & FLOMOD_FLAG_READ )
		flomod.diskf = fopen( flomod.diskfname, "rb" );
	else if ( flomod.flags & FLOMOD_FLAG_WRITE )
	{
		if ( access( flomod.diskfname, F_OK ) != -1 )
			flomod.diskf = fopen( flomod.diskfname, "r+b" );
		else flomod.diskf = fopen( flomod.diskfname, "wb" );
	}
	if ( flomod.diskf == NULL )
	{
		fprintf( stderr, "%s: could not open '%s'\n", flomod.exename, flomod.diskfname );
		exit( 1 );
	}

	//Get file length
	fseek( flomod.diskf, 0, SEEK_END );
	flomod.disklen = ftell( flomod.diskf );

	//Seek to place pointed by start descriptor
	fseek( flomod.diskf, flomod.start.offset, SEEK_SET );

	//Calculate end descriptor
	str2chsb( &flomod.end );
	if ( chsbnull( &flomod.end ) ) //Skip if set by user
	{
		if ( chsbnull( &flomod.length ) )
		{
			//Calculate from detected file size
			flomod.end.offset = flomod.disklen;
			lba2chsb( &flomod.end, &flomod.limits );
		}
		else
		{
			//Calculate by adding length and start
			flomod.end = flomod.start;
			chsbsum( &flomod.end, &flomod.length );
		}
	}
	chsbopt( &flomod.end, &flomod.limits );
	chsb2lba( &flomod.end, &flomod.limits );
	chsb2str( &flomod.end );

	fprintf( stderr, "%s: end at %s\n", flomod.exename, flomod.end.str );

	if ( flomod.flags & FLOMOD_FLAG_READ )
	{
		while ( ( ( b = fgetc( flomod.diskf ) ) != EOF ) && ( ftell( flomod.diskf ) <= flomod.end.offset ) )
			putchar( b );
	}

	//Close disk file
	fclose( flomod.diskf );
	return 0;
}
