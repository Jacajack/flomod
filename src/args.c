#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "args.h"

const char *argparse( ArgParser *conf, int conflen, int argc, char **argv )
{
	int i, j, buflen;
	char *buf;

	for ( i = 0; i < argc; i++ )
	{
		for ( j = 0; j < conflen; j++ )
		{
			//Attempt to match option name
			if ( !strcmp( argv[i], conf[j].optname ) )
			{
				//Flag argument as matched
				conf[j].flags |= ARGPARSE_FLAG_MATCH;

				//Argument is followed by its value
				if ( conf[j].flags & ARGPARSE_FLAG_VALUE )
				{
					if ( i + 1 < argc && argv[i + 1] != NULL )
					{
						conf[j].str = strdup( argv[i + 1] );
						*conf[j].destptr = conf[j].str;
						++i;
					}
					else
					{
						//User later needs to free memory allocated here!
						buflen = snprintf( NULL, 0, "bad value for '%s'", argv[i] );
						buf = (char*) malloc( buflen * sizeof( char ) );
						if ( buf == NULL ) return "malloc error";
						snprintf( buf, buflen, "bad value for '%s'", argv[i] );
						return buf;
					}
				}
				break;
			}
		}

		if ( j == conflen )
		{
			//User later needs to free memory allocated here!
			buflen = snprintf( NULL, 0, "unknown argument '%s'", argv[i] );
			buf = (char*) malloc( buflen * sizeof( char ) );
			if ( buf == NULL ) return "malloc error";
			snprintf( buf, buflen, "unknown argument '%s'", argv[i] );
			return buf;
		}
	}

	//On success do not return anything
	return NULL;
}
