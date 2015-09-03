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
duf_pdi_create( const char *name )
{
  duf_depthinfo_t *pdi;

  pdi = mas_malloc( sizeof( duf_depthinfo_t ) );
  memset( pdi, 0, sizeof( duf_depthinfo_t ) );
  if ( name )
  {
    pdi->pdi_name = mas_strdup( name );
    pdi->created_name = 1;
  }
  return pdi;
}

void
duf_pdi_delete( duf_depthinfo_t * pdi )
{
  duf_pdi_shut( pdi );
  if ( pdi->created_name )
    mas_free( pdi->pdi_name );
  pdi->pdi_name = NULL;
  mas_free( pdi );
}

void
duf_pdi_kill( duf_depthinfo_t ** ppdi )
{
  if ( ppdi )
    duf_pdi_delete( *ppdi );
  *ppdi = NULL;
}
