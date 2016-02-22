#ifndef MAS_MAST_PRINT_DEFS_H
#  define MAS_MAST_PRINT_DEFS_H

#include "mas_print_defs_preset.h"

#  define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
#  define T_FN_FMTN 23
#  define T_FN_FMT #T_FN_FMTN



#  define MAST_WRITES( _min, _str) \
    mas_writes( MAST_OUTPUT_LEVEL, _min, \
		MAST_FL, MAST_OUTPUT_FILE, _str)
#  define MAST_PUTS( _min, _str) \
    mas_puts( MAST_OUTPUT_LEVEL, _min, \
		MAST_FL, MAST_OUTPUT_FILE, _str)
#  define MAST_PUTSL( _min) \
    mas_puts( MAST_OUTPUT_LEVEL, _min, \
		MAST_FL, MAST_OUTPUT_FILE, NULL)

#  define MAST_VFPRINTFO( _min, _out, _force_color, _nocolor, ...) \
    mas_vprintfo( MAST_OUTPUT_LEVEL, 0 /*noeol*/, _min, 0, \
		MAST_FL, _out, _force_color, _nocolor, __VA_ARGS__ )
#  define MAST_VFPRINTFONE( _min, _out, _force_color, _nocolor, ...) \
    mas_vprintfo( MAST_OUTPUT_LEVEL, 1 /*noeol*/, _min, 0, \
		MAST_FL, _out, _force_color, _nocolor, __VA_ARGS__ )

#  define MAST_VFPRINTF( _min, _out, ...)  MAST_VFPRINTFO(_min, _out, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__)
#  define MAST_VFPRINTFNE( _min, _out, ...)   MAST_VFPRINTFONE(_min, _out, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__)


#  define MAST_FPRINTFO0( _min, _out, _force_color, _nocolor, ...) \
    mas_printfo( 0, 0 /*noeol*/, _min, 0, \
		MAST_FL, _out, _force_color, _nocolor, __VA_ARGS__ )
#  define MAST_FPRINTFO( _min, _out, _force_color, _nocolor, ...) \
    mas_printfo( MAST_OUTPUT_LEVEL, 0 /*noeol*/, _min, 0, \
		MAST_FL, _out, _force_color, _nocolor, __VA_ARGS__ )
#  define MAST_FPRINTFONE( _min, _out, _force_color, _nocolor, ...) \
    mas_printfo( MAST_OUTPUT_LEVEL, 1 /*noeol*/, _min, 0, \
		MAST_FL, _out, _force_color, _nocolor, __VA_ARGS__ )

#  define MAST_FPRINTF0( _min, _out, ...) MAST_FPRINTFO0(_min, _out, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__)
#  define MAST_FPRINTF( _min, _out, ...) MAST_FPRINTFO(_min, _out, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__)
#  define MAST_FPRINTFNE( _min, _out, ...) MAST_FPRINTFONE(_min, _out, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__)





#  define MAST_PRINTF( _min, ...) MAST_FPRINTF(_min, MAST_OUTPUT_FILE, __VA_ARGS__ )
#  define MAST_PRINTF0( _min, ...) MAST_FPRINTF0(_min, MAST_OUTPUT_FILE, __VA_ARGS__ )
#  define MAST_PRINTFNE( _min, ...) MAST_FPRINTFNE(_min, MAST_OUTPUT_FILE, __VA_ARGS__ )

/* #  define MAST_PRINTF( _min, ...) \                                                                                */
/*     mas_printfo( MAST_OUTPUT_LEVEL, 0, _min, 0, \                                           */
/*                 MAST_FL, MAST_OUTPUT_FILE, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__ ) */

#  define MAST_DIE( _min, ...) \
    mas_printfo( MAST_OUTPUT_LEVEL, 0 /*noeol*/, _min, 1, \
		MAST_FL, MAST_OUTPUT_FILE, MAST_OUTPUT_FORCE_COLOR, MAST_OUTPUT_NOCOLOR, __VA_ARGS__ )


#  define P(txt) MAST_PRINTF(0, #txt)
#  define PF(fmt, ...) MAST_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " #__VA_ARGS__ " $$$ : " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )
#  define PF0(fmt, ...) MAST_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )


#endif

/*
vi: ft=c
*/
