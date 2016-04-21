#include <assert.h>
#include <string.h>

/* #include <mastar/wrap/mas_memory.h>                                  (* mas_(malloc|free|strdup); etc. ♣ *) */

#include "duf_config_structs.h"
#include "duf_config_opt_structs.h"
/* ###################################################################### */
#include "duf_config.h"                                              /* duf_get_config ✗ */
/* ###################################################################### */

duf_config_t *duf_config = NULL;
duf_config_t *
duf_get_config( void )
{
  assert( duf_config );
  return duf_config;
}


