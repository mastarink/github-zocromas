#ifndef MAS_DUF_OUTPUT_DEFS_H
# define MAS_DUF_OUTPUT_DEFS_H

# include <mastar/wrap/mas_std_def.h>

/* TODO move ‘mas_printfo’ to new libtty */
# include <mastar/trace/mas_utils_print.h>

# define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
# define T_FN_FMTN 23
# define T_FN_FMT #T_FN_FMTN

# include "duf_config_output_weak.h"                                 /* mas_output_level ; mas_output_file ; mas_output_nocolor ; mas_output_force_color */

# define DUF_WRITES( _min, _str)		mas_writes( mas_output_level? mas_output_level():0, _min, FL, mas_output_file? mas_output_file():stderr, _str)


# define DUF_DIE( _min, ...)			mas_printfo( mas_output_level? mas_output_level():0, 0 /*noeol*/, _min, 1, FL, \
				mas_output_file? mas_output_file():stderr, \
				mas_output_force_color? mas_output_force_color():0, \
				mas_output_nocolor? mas_output_nocolor():0, \
			__VA_ARGS__ )

#endif

/*
vi: ft=c
*/

