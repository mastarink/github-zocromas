#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_pathdepth */
#include "duf_status_ref.h"

#include "duf_config_ref.h"
#include "duf_option_defs.h"

#include "duf_sql2.h"

#include "duf_maindb.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"
/* #include "duf_levinfo_context.h" */
#include "duf_context.h"
#include "duf_pdi_context.h"

#include "duf_path2db.h"

/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */



int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->pu->max_seq && pdi->seq >= pdi->pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pu->maxitems.files && pdi->items.files >= pdi->pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.dirs && pdi->items.dirs >= pdi->pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.total && pdi->items.total >= pdi->pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );


  /* rv = ( ( !pdi->pu->max_seq || pdi->seq <= pdi->pu->max_seq )                                  */
  /*        && ( !pdi->pu->maxitems.files || ( pdi->items.files ) < pdi->pu->maxitems.files )    */
  /*        && ( !pdi->pu->maxitems.dirs || ( pdi->items.dirs ) < pdi->pu->maxitems.dirs )       */
  /*        && ( !pdi->pu->maxitems.total || ( pdi->items.total ) < pdi->pu->maxitems.total ) ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

int
duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? d - pdi->topdepth : 0;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_opendir( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->opendir ? 1 : 0 ) : 0;
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  DEBUG_STARTR( rd );

  assert( pdi );
  rd = pdi->opendir;
  pdi->opendir = od;
  DUF_TRACE( fs, 3, "set opendir:%d", od );

  DEBUG_ENDRN( rd );
}

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->depth : 0;
}

/* pdi->depth - pdi->topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pdi_deltadepth( pdi, pdi->depth ) : 0;
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
  if ( pdi )
    pdi->topdepth = pdi->depth;
}

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->topdepth : 0;
}

int
duf_pdi_maxdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->maxdepth : 0;
}
