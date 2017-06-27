#ifndef ARGS_H
#define ARGS_H
#include <argp.h>

extern const char *argp_program_version;
extern const char *argp_program_bug_address;
extern char argp_doc[];
extern char argp_keydoc[];
extern struct argp_option argp_options[];
extern error_t parse_opt( int key, char *arg, struct argp_state *state );

#endif
