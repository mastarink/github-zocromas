#include <mastar/types/mas_common_types.h>

#include "mas_arg_tools.h"

/* ########################################################################################## */
#include "mas_argx_tools.h"
/* ########################################################################################## */

void
mas_add_argx_arg( mas_string_setv_t * ptargx, const char *arg )
{
  ptargx->c = mas_add_argv_arg( ptargx->c, &ptargx->v, arg );
}

void
mas_add_argx_arg_nodup( mas_string_setv_t * ptargx, char *arg )
{
  ptargx->c = mas_add_argv_arg_nodup( ptargx->c, &ptargx->v, arg );
}

void
mas_del_argx( mas_string_setv_t * ptargx, int from_a __attribute__ ( ( unused ) ) )
{
  ( void ) mas_del_argv( ptargx->c, ptargx->v, 0 );
  ptargx->c = 0;
  ptargx->v = NULL;
}
