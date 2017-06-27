#ifndef ARGS_H
#define ARGS_H

#define ARGPARSE_ERROR_OK 0
#define ARGPARSE_ERROR_NOMATCH 1
#define ARGPARSE_ERROR_BADVAL 2

#define ARGPARSE_FLAG_VALUE 1
#define ARGPARSE_FLAG_MATCH 2

typedef struct
{
	const char *optname;
	unsigned char flags;
	char **destptr;
	char *str;
} ArgParser;

const char *argparse( ArgParser *conf, int conflen, int argc, char **argv );

#endif
