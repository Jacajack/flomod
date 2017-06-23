#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "chsb.h"

int main( int argc, char **argv )
{
	CHSB a = {0, 0, 0, 0};
	CHSB lim = {80, 2, 18, 512};
	printf( "(%d)\n", str2chsb( &a, argv[1] ) );
	printf( "%ld:%ld:%ld:%ld\n", a.c, a.h, a.s, a.b );
	chsbopt( &a, lim );
	printf( "%ld:%ld:%ld:%ld\n", a.c, a.h, a.s, a.b );
	a.flags |= CHSB_FLAG_OFFSET_BYTES;
	printf( "%ld\n", chsb2lba( a, lim ) );
	return 0;
}
