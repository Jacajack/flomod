#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>
#include "chsb.h"
#include "disktypes.h"
#include "args.h"
#include "flomod.h"

//Main configuration structure (all flags etc.)
struct flomod flomod;

int main( int argc, char **argv )
{
	int i, b;

	//Hardcoded executable name (I'm sorry)
	flomod.exename = "flomod";

	//Parse command line arguments
	static struct argp argp = {argp_options, parse_opt, argp_keydoc, argp_doc};
	argp_parse( &argp, argc, argv, 0, 0, &flomod );

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

			//Unknown disk type - quit
			if ( i == DISKTYPE_COUNT )
			{
				fprintf( stderr, "%s: unknown disk type\n", flomod.exename );
				exit( 1 );
			}
		}
	}

	//No filename
	if ( flomod.diskfname == NULL )
	{
		if ( flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: disk file not specified!\n", flomod.exename );
		exit( 1 );
	}

	//Default disk type
	if ( flomod.flags & FLOMOD_FLAG_DEFAULT_FLOPPY )
	{
		flomod.limits = disktypes[DISKTYPE_FLOPPY_35_144];
		if ( flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: default disk type is 3.5\" floppy\n", flomod.exename );
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
		//Attempt to open for writing
		if ( access( flomod.diskfname, F_OK ) != -1 )
			flomod.diskf = fopen( flomod.diskfname, "r+b" ); //If file exists
		else
		{
			if ( flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: creating '%s'\n", flomod.exename, flomod.diskfname );
			flomod.diskf = fopen( flomod.diskfname, "wb" ); //If file doesn't exist
		}
	}
	else
	{
		//Open only for reading
		flomod.diskf = fopen( flomod.diskfname, "rb" );
	}

	//If file could not be opened
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

	//Output start and end position
	if ( flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: start at %s - %ld\n", flomod.exename, flomod.start.str, flomod.start.offset );
	if ( flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: end at   %s - %ld\n", flomod.exename, flomod.end.str, flomod.end.offset );

	if ( flomod.flags & FLOMOD_FLAG_WRITE )
	{
		//Write to disk file
		while ( ( ( b = getchar( ) ) != EOF ) && ( ftell( flomod.diskf ) <= flomod.end.offset ) )
			fputc( b, flomod.diskf );
	}
	else
	{
		//Read from disk file
		while ( ( ( b = fgetc( flomod.diskf ) ) != EOF ) && ( ftell( flomod.diskf ) <= flomod.end.offset ) )
			putchar( b );
	}

	//EOF message
	if ( b == EOF && flomod.flags & FLOMOD_FLAG_VERBOSE ) fprintf( stderr, "%s: EOF reached\n", flomod.exename );

	//Close disk file
	fclose( flomod.diskf );
	return 0;
}
