#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;

duf_config_t *
duf_get_config( void )
{
  assert( duf_config );
  return duf_config;
}
