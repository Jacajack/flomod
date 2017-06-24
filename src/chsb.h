#ifndef CHSB_H
#define CHSB_H
#include <inttypes.h>

#define CHSB_FLAG_SECTOR_BASE1 1
#define CHSB_FLAG_OFFSET_BYTES 2

typedef struct
{
	int64_t c;
	int64_t h;
	int64_t s;
	int64_t b;
	int64_t offset;
	char *str;
	uint8_t flags;
} CHSB;

extern void chsbopt( CHSB *desc, CHSB *lim );
extern void chsbsum( CHSB *desc, CHSB *val );
extern void chsb2lba( CHSB *desc, CHSB *lim );
extern void lba2chsb( CHSB *desc, CHSB *lim );
extern int str2chsb( CHSB *desc );

#endif
