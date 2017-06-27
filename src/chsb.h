#ifndef CHSB_H
#define CHSB_H
#include <inttypes.h>

#define CHSB_FLAG_SECTOR_BASE1 1

typedef struct
{
	long c;
	long h;
	long s;
	long b;
	long offset;
	const char *str;
	unsigned int flags;
} CHSB;

extern void chsbopt( CHSB *desc, CHSB *lim );
extern void chsbsum( CHSB *desc, CHSB *val );
extern int chsbnull( CHSB *desc );
extern void chsb2lba( CHSB *desc, CHSB *lim );
extern void lba2chsb( CHSB *desc, CHSB *lim );
extern int str2chsb( CHSB *desc );
extern const char *chsb2str( CHSB *desc );

#endif
