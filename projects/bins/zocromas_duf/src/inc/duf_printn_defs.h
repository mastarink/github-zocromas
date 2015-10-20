#ifndef MAS_DUF_PRINTN_DEFS_H
#  define MAS_DUF_PRINTN_DEFS_H

#  include "duf_defs.h"
/* #  include "duf_config_ref.h" */
#  include "duf_config_util.h"

#  define P_FN_FMT "32"

/* #  define T_FN_FMTN 29 */
/* #  define T_FN_FMTN 32 */
/* #  define T_FN_FMTN 27 */
#  define T_FN_FMTN 23
#  define T_FN_FMT #T_FN_FMTN



#  define DUF_WRITES( _min, _str)		MAST_WRITES( _min, _str)
#  define DUF_PUTS( _min, _str)			MAST_PUTS( _min, _str)
#  define DUF_PUTSL( _min )			MAST_PUTSL( _min )

#  define DUF_VFPRINTF( _min, _out, ...)	MAST_VFPRINTF( _min, _out, __VA_ARGS__)

#  define DUF_VFPRINTFNE( _min, _out, ...)	MAST_VFPRINTFNE( _min, _out, __VA_ARGS__)

#  define DUF_FPRINTF0( _min, _out, ...)	MAST_FPRINTF0( _min, _out, __VA_ARGS__)

#  define DUF_FPRINTF( _min, _out, ...)		MAST_FPRINTF( _min, _out, __VA_ARGS__ )
#  define DUF_FPRINTFNE( _min, _out, ...)	MAST_FPRINTFNE( _min, _out, __VA_ARGS__)


#  define DUF_PRINTF( _min, ...)		MAST_PRINTF( _min, __VA_ARGS__)
#  define DUF_PRINTF0( _min, ...)		MAST_PRINTF0( _min, __VA_ARGS__)

#  define DUF_DIE( _min, ...)			MAST_DIE( _min, __VA_ARGS__)





/* #  define P(txt) DUF_PRINTF(0, #txt)                                                                                   */
/* #  define PF(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " #__VA_ARGS__ " $$$ : " fmt " $$$$$$$$$$$$",  __VA_ARGS__ ) */
/* #  define PF0(fmt, ...) DUF_PRINTF(0, "[PF] $$$$$$$$$$$$$$ " fmt " $$$$$$$$$$$$",  __VA_ARGS__ )                       */

#  include <mastar/trace/mas_utils_print.h>

#endif

/*
vi: ft=c
*/
