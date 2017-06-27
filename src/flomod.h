#ifndef FLOMOD_H
#define FLOMOD_H
#include <stdio.h>
#include "chsb.h"

#define FLOMOD_FLAG_WRITE 1
#define FLOMOD_FLAG_SECTOR_BASE1 2
#define FLOMOD_FLAG_DEFAULT_FLOPPY 4
#define FLOMOD_FLAG_VERBOSE 8
#define FLOMOD_FLAG_DUMP 16

//Flomod configuration structure
struct flomod
{
	unsigned int flags;
	const char *exename;

	char *disktype;
	char *diskfname;
	FILE *diskf;
	long disklen;
	CHSB start;
	CHSB length;
	CHSB end;
	CHSB limits;
};

#endif
