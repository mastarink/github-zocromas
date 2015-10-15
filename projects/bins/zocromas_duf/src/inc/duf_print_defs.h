#ifndef MAS_DUF_PRINT_DEFS_H
#  define MAS_DUF_PRINT_DEFS_H

#  include "duf_defs.h"
/* #  include "duf_config_ref.h" */
#  include "duf_config_util.h"
#  include "duf_print_defs_preset.h"

#  define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
#  define T_FN_FMTN 23
#  define T_FN_FMT #T_FN_FMTN



#  define DUF_WRITES( _min, _str) \
    duf_writes( DUF_OUTPUT_LEVEL, _min, \
		DUF_FL, DUF_OUTPUT_FILE, _str)
#  define DUF_PUTS( _min, _str) \
    duf_puts( DUF_OUTPUT_LEVEL, _min, \
		DUF_FL, DUF_OUTPUT_FILE, _str)
#  define DUF_PUTSL( _min) \
    duf_puts( DUF_OUTPUT_LEVEL, _min, \
		DUF_FL, DUF_OUTPUT_FILE, NULL)

#  define DUF_VFPRINTF( _min, _out, ...) \
    duf_vprintf( DUF_OUTPUT_LEVEL, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_VFPRINTFNE( _min, _out, ...) \
    duf_vprintf( DUF_OUTPUT_LEVEL, 1 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )

#  define DUF_FPRINTF0( _min, _out, ...) \
    duf_printf( 0, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_FPRINTF( _min, _out, ...) \
    duf_printf( DUF_OUTPUT_LEVEL, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_FPRINTFNE( _min, _out, ...) \
    duf_printf( DUF_OUTPUT_LEVEL, 1 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )


#  define DUF_PRINTF( _min, ...) DUF_FPRINTF(_min, DUF_OUTPUT_FILE, __VA_ARGS__ )

/* #  define DUF_PRINTF( _min, ...) \                                                                                */
/*     duf_printf( DUF_OUTPUT_LEVEL, 0, _min, 0, \                                           */
/*                 DUF_FL, DUF_OUTPUT_FILE, __VA_ARGS__ ) */

#  define DUF_DIE( _min, ...) \
    duf_printf( duf_output_level(), 0 /*noeol*/, _min, 1, \
		DUF_FL, DUF_OUTPUT_FILE, __VA_ARGS__ )


#  define P(txt) DUF_PRINTF(0, #txt)
#  define PF(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " #__VA_ARGS__ " $$$ : " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )
#  define PF0(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )


#endif

/*
vi: ft=c
*/
