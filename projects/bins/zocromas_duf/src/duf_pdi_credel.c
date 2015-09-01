#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_maintenance.h"


#include "duf_pdi.h"

/* ###################################################################### */
#include "duf_pdi_credel.h"
/* ###################################################################### */


duf_depthinfo_t *
duf_pdi_create( void )
{
  duf_depthinfo_t *pdi;

  pdi = mas_malloc( sizeof( duf_depthinfo_t ) );
  memset( pdi, 0, sizeof( duf_depthinfo_t ) );

  return pdi;
}

void
duf_pdi_delete( duf_depthinfo_t * pdi )
{
  duf_pdi_shut( pdi );
  mas_free( pdi );
}

void
duf_pdi_kill( duf_depthinfo_t ** ppdi )
{
  if ( ppdi )
    duf_pdi_delete( *ppdi );
  *ppdi = NULL;
}


