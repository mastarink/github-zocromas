#ifndef MAS_DUF_PRINT_DEFS_H
#  define MAS_DUF_PRINT_DEFS_H

#  include "duf_defs.h"

#  define FN_FMT "29s"
/* #  define FN_FMT "27s" */
/* #  define FN_FMT "23s" */



#  define DUF_PUTS( _min, _str) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, _min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, _str)
#  define DUF_PUTSL( _min) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, _min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, NULL)


#  define DUF_PRINTF( _min, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, _min, 0, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )

#  define DUF_DIE( _min, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, _min, 1, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )


#  define P(txt) DUF_PRINTF(0, #txt)
#  define PF(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " #__VA_ARGS__ " $$$ : " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )
#  define PF0(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )


#endif
