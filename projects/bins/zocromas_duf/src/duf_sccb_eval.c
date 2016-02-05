#include "duf_maintenance.h"
#include "duf_fmt_defs.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_sccb.h"
#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"    /* DUF_ACTG_FLAG( summary ) */

#include "duf_pdi_global.h"
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval.h"
/* ###################################################################### */



int
duf_ev_sccb( const duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", sccb->name, duf_pdi_global_name(  ) );

  /* T( "sccb:%d; dirid:%llu", sccb ? 1 : 0, duf_levinfo_dirid( duf_pdi_global() ) ); */
  DOR( r, duf_ev_pdi_sccb( duf_pdi_global(  ), sccb, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_evnamen( const char *name, size_t len, duf_scan_callbacks_t * first )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_evnamen( duf_pdi_global(  ), name, len, first, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_evname( const char *name, duf_scan_callbacks_t * first )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", name, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_evname( duf_pdi_global(  ), name, first, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_evnamed_list( const char *names, duf_scan_callbacks_t * first )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  /* assert( duf_pdi_global()->pyp ); */
  DUF_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, duf_pdi_global_name(  ) );

  /* T( "names:%s; dirid:%llu", names, duf_levinfo_dirid( duf_pdi_global() ) ); */
  DOR( r, duf_ev_pdi_evnamed_list( duf_pdi_global(  ), names, first, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
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
duf_ev_sccb_array( const duf_scan_callbacks_t ** psccbs, int sccb_num, int *pcnt )
{
  DEBUG_STARTR( r );

  int cnt = 0;

  assert( duf_pdi_global_name(  ) );
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_pdi_global(  ) ) );

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
       *     - evaluate psccbs[astep] for each string  from DUF_CONFIGG(cli.targ)[cv] as path
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
