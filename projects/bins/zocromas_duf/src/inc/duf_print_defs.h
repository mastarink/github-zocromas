#ifndef MAS_DUF_PRINT_DEFS_H
#  define MAS_DUF_PRINT_DEFS_H

#  include "duf_defs.h"
#  include "duf_config_ref.h"

#  define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
#  define T_FN_FMTN 23
#  define T_FN_FMT #T_FN_FMTN



#  define DUF_WRITES( _min, _str) \
    duf_writes( duf_config ? duf_config->cli.output.level:0, _min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, _str)
#  define DUF_PUTS( _min, _str) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, _min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, _str)
#  define DUF_PUTSL( _min) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, _min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, NULL)

#  define DUF_VFPRINTF( _min, _out, ...) \
    duf_vprintf( duf_config ? duf_config->cli.output.level:0, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_VFPRINTFNE( _min, _out, ...) \
    duf_vprintf( duf_config ? duf_config->cli.output.level:0, 1 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )

#  define DUF_FPRINTF0( _min, _out, ...) \
    duf_printf( 0, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_FPRINTF( _min, _out, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, 0 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )
#  define DUF_FPRINTFNE( _min, _out, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, 1 /*noeol*/, _min, 0, \
		DUF_FL, _out, __VA_ARGS__ )


#  define DUF_PRINTF( _min, ...) DUF_FPRINTF(_min, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )

/* #  define DUF_PRINTF( _min, ...) \                                                                                */
/*     duf_printf( duf_config ? duf_config->cli.output.level:0, 0, _min, 0, \                                           */
/*                 DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ ) */

#  define DUF_DIE( _min, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, 0 /*noeol*/, _min, 1, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )


#  define P(txt) DUF_PRINTF(0, #txt)
#  define PF(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " #__VA_ARGS__ " $$$ : " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )
#  define PF0(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )


#endif

/*
vi: ft=c
*/
