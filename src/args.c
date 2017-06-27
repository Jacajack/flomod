/*
	flomod - a floppy disk image editor
	Copyright (C) 2017 Jacek Wieczorek <mrjjot@gmail.com>
	This file is part of flomod.
	Flomod is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Flomod is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <argp.h>
#include "flomod.h"

const char *argp_program_version = "flomod v0.1";
const char *argp_program_bug_address = "<mrjjot@gmail.com>";
char argp_doc[] = "flomod - A program to edit floppy disk.";
char argp_keydoc[] = "FILENAME";
struct argp_option argp_options[] =
{
	{"read",   'r', 0,          0, "Read data from disk"},
	{"write",  'w', 0,          0, "Write data to disk"},
	{"dump",   'd', 0,          0, "Dump read/written data in hex format"},
	{"verbose",'v', 0,          0, "Be verbose"},
	{"baseone",'1', 0,          0, "Use 1 based sector numeration (like int 13h)"},
	{"start",  's', "C:H:S:B",  0, "Set start point for disk operation"},
	{"end",    'e', "C:H:S:B",  0, "Set end point for disk operation"},
	{"num",    'n', "C:H:S:B",  0, "Set disk operation length"},
	{"geom",   'g', "C:H:S:B",  0, "Set disk geometry limits"},
	{"type",   't', "TYPE",     0, "Set disk type:\n" \
  	"FLOPPY_3.5_740K / FLOPPY_3.5_1.44M / FLOPPY_3.5_2.88M / FLOPPY_5.25_1.2M / FLOPPY_5.25_360K\n"},

	{0}
};

//Lovely callback for ARGP to parse command line options
error_t parse_opt( int key, char *arg, struct argp_state *state )
{
	struct flomod *conf = (struct flomod*) state->input;

	switch ( key )
	{
		case 'v':
			conf->flags |= FLOMOD_FLAG_VERBOSE;
			break;

		case 'r':
			conf->flags &= ~FLOMOD_FLAG_WRITE;
			break;

		case 'w':
    		conf->flags |= FLOMOD_FLAG_WRITE;
    		break;

		case 'd':
			conf->flags |= FLOMOD_FLAG_DUMP;
			break;

		case '1':
			conf->flags |= FLOMOD_FLAG_SECTOR_BASE1;
			break;

		case 's':
    		conf->start.str = arg;
    		break;

		case 'e':
			conf->end.str = arg;
			break;

		case 'n':
	    	conf->length.str = arg;
	    	break;

		case 'g':
			conf->geom.str = arg;
			break;

		case 't':
			conf->disktype = arg;
			break;

		case ARGP_KEY_ARG:
    		if ( state->arg_num >= 1 ) argp_usage( state );
			conf->diskfname = arg;
			break;

		case ARGP_KEY_END:
    		if ( state->arg_num < 1 ) argp_usage( state );
    		break;

    	default:
    		return ARGP_ERR_UNKNOWN;
    }
	return 0;
}
