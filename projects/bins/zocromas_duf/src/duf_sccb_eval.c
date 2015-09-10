#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_sccb.h"
#include "duf_levinfo_ref.h"

#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */



int
duf_evaluate_sccb( duf_scan_callbacks_t * sccb )
{
  assert( DUF_CONFIGG( pdi )->pdi_name );
  return duf_evaluate_pdi_sccb( DUF_CONFIGG( pdi ), DUF_CONFIGA( targ ), DUF_CONFIGG( pu ), sccb );
}

int
duf_evaluate_sccb_namen( const char *name, size_t len, const duf_action_table_t * table )
{
  assert( DUF_CONFIGG( pdi )->pdi_name );
  return duf_ev_pdi_sccb_namen( name, len, table, DUF_CONFIGG( pdi ), DUF_CONFIGA( targ ), DUF_CONFIGG( pu ) );
}

int
duf_evaluate_sccb_name( const char *name, const duf_action_table_t * table )
{
  assert( DUF_CONFIGG( pdi )->pdi_name );
  return duf_evaluate_pdi_sccb_name( name, table, DUF_CONFIGG( pdi ), DUF_CONFIGA( targ ), DUF_CONFIGG( pu ) );
}

int
duf_evaluate_sccb_named_list( const char *names, const duf_action_table_t * table )
{
  assert( DUF_CONFIGG( pdi )->pdi_name );
  return duf_ev_pdi_named_sccbs( names, table, DUF_CONFIGG( pdi ), DUF_CONFIGA( targ ), DUF_CONFIGG( pu ) );
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
 *    by calling duf_evaluate_sccb
 * */
int
duf_evaluate_sccb_array( duf_scan_callbacks_t ** psccbs, int sccb_num, int *pcnt )
{
  DEBUG_STARTR( r );

  int cnt = 0;

  assert( DUF_CONFIGG( pdi )->pdi_name );
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( DUF_CONFIGG( pdi ) ) );

  for ( int astep = 0; DUF_NOERROR( r ) && astep < sccb_num; astep++ )
  {
    if ( psccbs[astep] )
    {
      DUF_TRACE( action, 2, "%" DUF_ACTION_TITLE_FMT ": astep %d", duf_uni_scan_action_title( psccbs[astep] ), astep );

      /* duf_evaluate_sccb:
       * do everything needed from sccb, i.e. :
       * - call init  from psccbs[astep]
       * - do beginning sql set  from psccbs[astep]
       * - via duf_sccb_each_targv:
       *     - evaluate psccbs[astep] for each string  from DUF_CONFIGG(targ)[cv] as path
       *     - store number of changes to *pchanges
       * - do final sql set from psccbs[astep]
       * XXX  XXX */ DOR( r, duf_evaluate_sccb( psccbs[astep] /* sccb */  ) );
      cnt++;
    }
  }
  if ( pcnt )
    *pcnt += cnt;
  DEBUG_ENDR( r );
}
