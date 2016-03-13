#ifndef MAS_MUC_PRINTN_DEFS_H
# define MAS_MUC_PRINTN_DEFS_H

# include <mastar/wrap/mas_std_def.h>

/* TODO move ‘mas_printfo’ to new libtty */
# include <mastar/trace/mas_utils_print.h>

# include "muc_config_output_weak.h"                                 /* mas_output_level ; mas_output_file ; mas_output_nocolor ; mas_output_force_color */


# define MUC_VFPRINTF( _min, _out, ...)	mas_vprintfo( mas_output_level? mas_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				_out, mas_output_force_color? mas_output_force_color():0, mas_output_nocolor? mas_output_nocolor():0, __VA_ARGS__ )

# define MUC_VFPRINTFNE( _min, _out, ...)	mas_vprintfo( mas_output_level? mas_output_level():0, 1 /*noeol*/, _min, 0, FL, \
				_out, mas_output_force_color? mas_output_force_color():0, mas_output_nocolor? mas_output_nocolor():0, __VA_ARGS__ )

# define MUC_FPRINTF0( _min, _out, ...)	mas_printfo( 0, 0 /*noeol*/, _min, 0, FL, \
				_out, mas_output_force_color? mas_output_force_color():0, mas_output_nocolor? mas_output_nocolor():0, __VA_ARGS__ )

# define MUC_FPRINTF( _min, _out, ...)		mas_printfo( mas_output_level? mas_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				_out, mas_output_force_color? mas_output_force_color():0, mas_output_nocolor? mas_output_nocolor():0, __VA_ARGS__ )

# define MUC_FPRINTFNE( _min, _out, ...)	mas_printfo( mas_output_level? mas_output_level():0, 1 /*noeol*/, _min, 0, FL, \
				_out, mas_output_force_color? mas_output_force_color():0, mas_output_nocolor? mas_output_nocolor():0, __VA_ARGS__ )

# define MUC_PRINTF( _min, ...)		mas_printfo( mas_output_level? mas_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				mas_output_file? mas_output_file():stderr, \
				mas_output_force_color? mas_output_force_color():0, \
				mas_output_nocolor? mas_output_nocolor():0, \
			__VA_ARGS__ )

# define MUC_PRINTF0( _min, ...)	mas_printfo( 0, 0 /*noeol*/, _min, 0, FL, \
    				mas_output_file? mas_output_file():stderr, \
    				mas_output_force_color? mas_output_force_color():0, \
    				mas_output_nocolor? mas_output_nocolor():0, \
			__VA_ARGS__ )

# define MUC_PUTS( _min, _str)			mas_puts( mas_output_level? mas_output_level():0, _min, FL, mas_output_file? mas_output_file():stderr, _str)
# define MUC_PUTSL( _min )			mas_puts( mas_output_level? mas_output_level():0, _min, FL, mas_output_file? mas_output_file():stderr, NULL)

#endif

/*
vi: ft=c
*/