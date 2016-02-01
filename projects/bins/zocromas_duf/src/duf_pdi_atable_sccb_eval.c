#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"

#include "duf_atable_sccb.h"
#include "duf_sccb_handle.h"

#include "duf_levinfo_ref.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccbh_eval.h"

#include "duf_maindb.h"


#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_pdi_atable_sccb_eval.h"
/* ###################################################################### */

/* 20150922.123744
 * make/evaluate sccb
 * ******************
 * 1. «open» sccb handle - incorporate argv to the handle - by calling duf_sccb_handle_open
 * 2. ?? call duf_count_total_items
 * 3. call sccb->init_scan
 * 4. calls duf_sccbh_eval_each_path
 * 5. «close» sccb handle - by calling duf_close_sccb_handle
 * */


/* 20150922.123731 */
int
duf_ev_pdi_atable_evnamen( duf_depthinfo_t * pdi, const char *name, size_t len, const duf_action_table_t * table, const mas_argvc_t * ptarg,
                           bool f_summary )
{
  DEBUG_STARTR( r );
  const duf_action_table_t *act = NULL;

  assert( pdi );

  if ( DUF_NOERROR( r ) )
    act = duf_find_atable_sccb_by_evnamen( name, len, table ); /* XXX XXX */
  DUF_TRACE( sccb, 0, "evaluate sccb name '%s' [%s] : found act:%s", name, pdi->pdi_name, act ? ( act->sccb ? act->sccb->name : "NONAME" ) : "-" );
  if ( act )
  {
    DUF_TRACE( path, 0, "@(to evaluate pdi sccb) [%s] levinfo_path: %s", act->sccb->name, duf_levinfo_path( pdi ) );

    DOR( r, duf_ev_pdi_sccb( pdi, act->sccb, ptarg /*, pu */ , f_summary ) ); /* XXX XXX XXX XXX */
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_SCCB_NOT_FOUND );
  }
  DEBUG_ENDR( r );
}

/* 20150922.123721 */
int
duf_ev_pdi_atable_evname( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const mas_argvc_t * ptarg, bool f_summary )
{
  DEBUG_STARTR( r );
  assert( pdi );

  DOR( r, duf_ev_pdi_atable_evnamen( pdi, name, strlen( name ), table, ptarg /*, pu */ , f_summary ) );
  DEBUG_ENDR( r );
}

/* 20150922.123718 */
int
duf_ev_pdi_atable_evname_at( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const char *arg, bool f_summary )
{
  DEBUG_STARTR( r );

  mas_argvc_t targ = {.argc = 0,.argv = NULL };

  assert( pdi );

  if ( !arg )
    arg = duf_levinfo_path( pdi );
  targ.argc = mas_add_argv_arg( targ.argc, &targ.argv, arg );

  DOR( r, duf_ev_pdi_atable_evname( pdi, name, table, &targ /*, pu */ , f_summary ) );

  mas_del_argv( targ.argc, targ.argv, 0 );
  DEBUG_ENDR( r );
}

/* 20150922.123706 */
int
duf_ev_pdi_atable_evnamed_list( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table, const mas_argvc_t * ptarg,
                                bool f_summary )
{
  DEBUG_STARTR( r );

  int ok = 0;
  const char *pnames;

  assert( pdi );

  pnames = names;
  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( pdi ) );

  /* assert( pdi->pyp ); */
  while ( DUF_NOERROR( r ) && pnames && *pnames )
  {
    size_t len = 0;
    const char *ename = NULL;

    ename = strchr( pnames, ',' );

    len = ename ? ( size_t ) ( ename - pnames ) : strlen( pnames );
    DOR( r, duf_ev_pdi_atable_evnamen( pdi, pnames, len, table, ptarg /*, pu */ , f_summary ) );
    if ( DUF_NOERROR( r ) )
      ok++;

    pnames = ( ename && *ename ) ? ename + 1 : NULL;
    ename = NULL;
  }
  if ( !ok )
  {
    /* DUF_SHOW_ERROR( "sccb not found: %s", names ); */
  }
  DEBUG_ENDR( r );
}
