#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_sccb.h"
#include "duf_levinfo_ref.h"

#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */



int
duf_ev_sccb( duf_scan_callbacks_t * sccb )
{
  assert( DUF_CONFIGX( pdi )->pdi_name );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", sccb->name, DUF_CONFIGX( pdi )->pdi_name );
  return duf_ev_pdi_sccb( DUF_CONFIGG( pdi ), sccb, DUF_CONFIGA( targ ) /*, DUF_CONFIGG( puz ) */  );
}

int
duf_ev_evnamen( const char *name, size_t len, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( pdi )->pdi_name );
  DOR( r, duf_ev_pdi_evnamen( DUF_CONFIGG( pdi ), name, len, table, DUF_CONFIGA( targ ) /*, DUF_CONFIGG( puz ) */  ) );
  DEBUG_ENDR( r );
}

int
duf_ev_evname( const char *name, const duf_action_table_t * table )
{
  assert( DUF_CONFIGX( pdi )->pdi_name );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", name, DUF_CONFIGX( pdi )->pdi_name );
  return duf_ev_pdi_evname( DUF_CONFIGG( pdi ), name, table, DUF_CONFIGA( targ ) /*, DUF_CONFIGG( puz ) */  );
}

int
duf_ev_evnamed_list( const char *names, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( pdi )->pdi_name );
  assert( DUF_CONFIGX( pdi )->pyp );
  DUF_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, DUF_CONFIGX( pdi )->pdi_name );
  DOR( r, duf_ev_pdi_evnamed_list( DUF_CONFIGG( pdi ), names, table, DUF_CONFIGA( targ ) /*, DUF_CONFIGG( puz ) */  ) );
  DEBUG_ENDR( r );
}

#if 0
static int
duf_show_sccb_sequence( duf_scan_callbacks_t ** psccbs, int sccb_num )
{
  DEBUG_STARTR( r );
  DUF_PRINTF( 0, "SCCBS" );
  for ( int astep = 0; DUF_NOERROR( r ) && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_PRINTF( 0, "%d. %s", astep, psccbs[astep]->title );
    }
  }
  DEBUG_ENDR( r );
}
#endif
/*
 * for each sccb from sequence/list (with given size - # of items) make/evaluate the task
 *    by calling duf_ev_sccb
 * */
int
duf_ev_sccb_array( duf_scan_callbacks_t ** psccbs, int sccb_num, int *pcnt )
{
  DEBUG_STARTR( r );

  int cnt = 0;

  assert( DUF_CONFIGX( pdi )->pdi_name );
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( DUF_CONFIGG( pdi ) ) );

  for ( int astep = 0; DUF_NOERROR( r ) && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( psccbs[astep] ), astep );

      /* duf_ev_sccb:
       * do everything needed from sccb, i.e. :
       * - call init  from psccbs[astep]
       * - do beginning sql set  from psccbs[astep]
       * - via duf_sccb_each_targv:
       *     - evaluate psccbs[astep] for each string  from DUF_CONFIGG(targ)[cv] as path
       *     - store number of changes to *pchanges
       * - do final sql set from psccbs[astep]
       * XXX  XXX */ DOR( r, duf_ev_sccb( psccbs[astep] /* sccb */  ) );
      cnt++;
    }
  }
  if ( pcnt )
    *pcnt += cnt;
  DEBUG_ENDR( r );
}
