#ifndef MAS_DUF_PRINTN_DEFS_H
# define MAS_DUF_PRINTN_DEFS_H

# include <mastar/wrap/mas_std_def.h>

/* TODO move ‘mas_printfo’ to new libtty */
# include <mastar/trace/mas_utils_print.h>

# define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
# define T_FN_FMTN 23
# define T_FN_FMT #T_FN_FMTN

# include "duf_config_output_weak.h"                                 /* duf_output_level ; duf_output_file ; duf_output_nocolor ; duf_output_force_color */

# define DUF_WRITES( _min, _str)		mas_writes( duf_output_level? duf_output_level():0, _min, FL, duf_output_file? duf_output_file():stderr, _str)
# define DUF_PUTS( _min, _str)			mas_puts( duf_output_level? duf_output_level():0, _min, FL, duf_output_file? duf_output_file():stderr, _str)
# define DUF_PUTSL( _min )			mas_puts( duf_output_level? duf_output_level():0, _min, FL, duf_output_file? duf_output_file():stderr, NULL)

# define DUF_VFPRINTF( _min, _out, ...)	mas_vprintfo( duf_output_level? duf_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				_out, duf_output_force_color? duf_output_force_color():0, duf_output_nocolor? duf_output_nocolor():0, __VA_ARGS__ )

# define DUF_VFPRINTFNE( _min, _out, ...)	mas_vprintfo( duf_output_level? duf_output_level():0, 1 /*noeol*/, _min, 0, FL, \
				_out, duf_output_force_color? duf_output_force_color():0, duf_output_nocolor? duf_output_nocolor():0, __VA_ARGS__ )

# define DUF_FPRINTF0( _min, _out, ...)	mas_printfo( 0, 0 /*noeol*/, _min, 0, FL, \
				_out, duf_output_force_color? duf_output_force_color():0, duf_output_nocolor? duf_output_nocolor():0, __VA_ARGS__ )

# define DUF_FPRINTF( _min, _out, ...)		mas_printfo( duf_output_level? duf_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				_out, duf_output_force_color? duf_output_force_color():0, duf_output_nocolor? duf_output_nocolor():0, __VA_ARGS__ )

# define DUF_FPRINTFNE( _min, _out, ...)	mas_printfo( duf_output_level? duf_output_level():0, 1 /*noeol*/, _min, 0, FL, \
				_out, duf_output_force_color? duf_output_force_color():0, duf_output_nocolor? duf_output_nocolor():0, __VA_ARGS__ )

# define DUF_PRINTF( _min, ...)		mas_printfo( duf_output_level? duf_output_level():0, 0 /*noeol*/, _min, 0, FL, \
				duf_output_file? duf_output_file():stderr, \
				duf_output_force_color? duf_output_force_color():0, \
				duf_output_nocolor? duf_output_nocolor():0, \
			__VA_ARGS__ )

# define DUF_PRINTF0( _min, ...)	mas_printfo( 0, 0 /*noeol*/, _min, 0, FL, \
    				duf_output_file? duf_output_file():stderr, \
    				duf_output_force_color? duf_output_force_color():0, \
    				duf_output_nocolor? duf_output_nocolor():0, \
			__VA_ARGS__ )

# define DUF_DIE( _min, ...)			mas_printfo( duf_output_level? duf_output_level():0, 0 /*noeol*/, _min, 1, FL, \
				duf_output_file? duf_output_file():stderr, \
				duf_output_force_color? duf_output_force_color():0, \
				duf_output_nocolor? duf_output_nocolor():0, \
			__VA_ARGS__ )

#endif

/*
vi: ft=c
*/
