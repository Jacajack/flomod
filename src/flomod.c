#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "chsb.h"
#include "disktypes.h"
#include "args.h"

#define FLOMOD_VERSION "v0.0"

#define FLOMOD_FLAG_WRITE 1
#define FLOMOD_FLAG_SECTOR_BASE1 2
#define FLOMOD_FLAG_DEFAULT_FLOPPY 4

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
		"\t-h - display this help message\n" \
		"\t-s - specify start point in C:H:S:B format\n" \
		"\t-n - specify read/write length in C:H:S:B format\n" \
		"\t-e - specify end point in C:H:S:B format\n" \
		"\t-l - specify disk geometry in C:H:S:B format\n" \
		"\t-t - specify disk geometry from predefined list:\n" \
		"\t\t- FLOPPY_3.5_1.44M\n" \
		"\t\t- FLOPPY_3.5_2.88M\n" \
		, flomod.exename, flomod.exename );
}

int main( int argc, char **argv )
{
	int i, b;

	flomod.flags = FLOMOD_FLAG_SECTOR_BASE1;

	if ( argc >= 1 ) flomod.exename = argv[0];

	if ( argc < 2 )
	{
		help( );
		exit( 1 );
	}

	ArgParser args[] =
	{
		{"-s", ARGPARSE_FLAG_VALUE, &flomod.start.str},
		{"-n", ARGPARSE_FLAG_VALUE, &flomod.length.str},
		{"-l", ARGPARSE_FLAG_VALUE, &flomod.limits.str},
		{"-e", ARGPARSE_FLAG_VALUE, &flomod.end.str},
		{"-t", ARGPARSE_FLAG_VALUE, &flomod.disktype},
		{"-f", ARGPARSE_FLAG_VALUE, &flomod.diskfname},
		{"-w"},
		{"-r"},
		{"-z"}
	};

	const char *argerr = argparse( args, sizeof( args ) / sizeof( args[0] ), argc - 1, argv + 1 );
	if ( argerr != NULL )
	{
		fprintf( stderr, "%s: %s\n", flomod.exename, argerr );
		exit( 1 );
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

	//No filename
	if ( flomod.diskfname == NULL )
	{
		fprintf( stderr, "%s: disk file not specified!\n", flomod.exename );
		exit( 1 );
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
	chsb2str( &flomod.start );

	//Get length
	str2chsb( &flomod.length );
	chsbopt( &flomod.length, &flomod.limits );
	chsb2lba( &flomod.length, &flomod.limits );

	//Open disk file
	if ( flomod.flags & FLOMOD_FLAG_WRITE )
	{
		if ( access( flomod.diskfname, F_OK ) != -1 )
			flomod.diskf = fopen( flomod.diskfname, "r+b" );
		else flomod.diskf = fopen( flomod.diskfname, "wb" );
	}
	else
	{
		flomod.diskf = fopen( flomod.diskfname, "rb" );
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
			flomod.end.offset = flomod.disklen - 1;
			lba2chsb( &flomod.end, &flomod.limits );
			fprintf( stderr, "%s: no endpoint specified - reading whole file\n", flomod.exename );
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

	fprintf( stderr, "%s: start at %s - %ld\n", flomod.exename, flomod.start.str, flomod.start.offset );
	fprintf( stderr, "%s: end at   %s - %ld\n", flomod.exename, flomod.end.str, flomod.end.offset );

	if ( flomod.flags & FLOMOD_FLAG_WRITE )
	{
		while ( ( ( b = getchar( ) ) != EOF ) && ( ftell( flomod.diskf ) <= flomod.end.offset ) )
			fputc( b, flomod.diskf );
	}
	else
	{
		while ( ( ( b = fgetc( flomod.diskf ) ) != EOF ) && ( ftell( flomod.diskf ) <= flomod.end.offset ) )
			putchar( b );
	}

	if ( b == EOF ) fprintf( stderr, "%s: EOF reached\n", flomod.exename );

	//Close disk file
	fclose( flomod.diskf );
	return 0;
}
