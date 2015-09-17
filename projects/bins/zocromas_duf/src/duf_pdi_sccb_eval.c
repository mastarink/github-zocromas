#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_sccb.h"
#include "duf_sccb_handle.h"

#include "duf_levinfo_ref.h"

#include "duf_sccbh_eval.h"

#include "duf_maindb.h"


/* ###################################################################### */
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */

const duf_action_table_t *
duf_find_sccb_by_evnamen( const char *name, size_t namelen, const duf_action_table_t * table )
{
  for ( const duf_action_table_t * act = table; !act->end_of_table; act++ )
  {
    if ( act->sccb && act->in_use && 0 == strncmp( name, act->sccb->name, namelen ) )
      return act;
  }
  return NULL;
}


/*
 * make/evaluate sccb
 * ******************
 * 1. «open» sccb handle - incorporate argv to the handle - by calling duf_sccb_handle_open
 * 2. ?? call duf_count_total_items
 * 3. call sccb->init_scan
 * 4. calls duf_sccbh_eval_each_path
 * 5. «close» sccb handle - by calling duf_close_sccb_handle
 * */
int
duf_ev_pdi_sccb( duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_argvc_t * ptarg /* , const duf_ufilter_t * pu_unused */  )
{
  DEBUG_STARTR( r );

  duf_sccb_handle_t *sccbh = NULL;

  DUF_TRACE( sccb, 0, "to open sccb handle %s", sccb->name );
  DUF_TRACE( path, 0, "@(to open sccbh) levinfo_path: %s", duf_levinfo_path( pdi ) );
  sccbh = duf_sccb_handle_open( pdi, sccb, ptarg->argc, ptarg->argv /* , pu */ , &r );
  {
    DUF_TRACE( sccbh, 0, "opened(?) sccb handle (%d)", sccbh ? 1 : 0 );
    DOR( r, DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( sccbh ) ); /* XXX XXX XXX XXX XXX XXX */
  }
  {
    int r1 = 0;

    DOR( r1, duf_sccb_handle_close( sccbh ) );
    if ( DUF_NOERROR( r ) && !DUF_NOERROR( r1 ) )
      r = r1;
  }
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evnamen( duf_depthinfo_t * pdi, const char *name, size_t len, const duf_action_table_t * table,
                    duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  )
{
  DEBUG_STARTR( r );
  const duf_action_table_t *act = NULL;

  assert( pdi );

  if ( DUF_NOERROR( r ) )
    act = duf_find_sccb_by_evnamen( name, len, table ); /* XXX XXX */
  if ( act )
  {
    DUF_TRACE( path, 0, "@(to evaluate pdi sccb) [%s] levinfo_path: %s", act->sccb->name, duf_levinfo_path( pdi ) );

    DOR( r, duf_ev_pdi_sccb( pdi, act->sccb, ptarg /*, pu */  ) ); /* XXX XXX XXX XXX */
  }
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, duf_ev_pdi_evnamen( pdi, name, strlen( name ), table, ptarg /*, pu */  ) );
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname_at( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const char *arg /*, const duf_ufilter_t * pu */  )
{
  DEBUG_STARTR( r );

  duf_argvc_t targ = {.argc = 0,.argv = NULL };

  assert( pdi );

  if ( !arg )
    arg = duf_levinfo_path( pdi );
  targ.argc = mas_add_argv_arg( targ.argc, &targ.argv, arg );

  DOR( r, duf_ev_pdi_evname( pdi, name, table, &targ /*, pu */  ) );

  mas_del_argv( targ.argc, targ.argv, 0 );
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evnamed_list( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table,
                         duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  )
{
  DEBUG_STARTR( r );

  int ok = 0;
  const char *pnames;

  assert( pdi );

  pnames = names;
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( pdi ) );


  while ( DUF_NOERROR( r ) && pnames && *pnames )
  {
    size_t len = 0;
    const char *ename = NULL;

    ename = strchr( pnames, ',' );
#if 0
    if ( ename )
      len = ename - pnames;
    else
      len = strlen( pnames );
#else
    len = ename ? ename - pnames : strlen( pnames );
#endif
    DOR( r, duf_ev_pdi_evnamen( pdi, pnames, len, table, ptarg /*, pu */  ) );
    if ( DUF_NOERROR( r ) )
      ok++;
    /* DUF_SHOW_ERROR( "(%d) sccb not found: %s", r, names ); */
    /* DUF_PRINTF( 0, "pnames:%s; ename:%s", pnames, ename ); */
#if 0
    if ( ename && *ename )
      pnames = ename + 1;
    else
      pnames = NULL;
#else
    pnames = ( ename && *ename ) ? ename + 1 : NULL;
#endif
    ename = NULL;
  }
  if ( !ok )
  {
    /* DUF_SHOW_ERROR( "sccb not found: %s", names ); */
  }
  DEBUG_ENDR( r );
}
