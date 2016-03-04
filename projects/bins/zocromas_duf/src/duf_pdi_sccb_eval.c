/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_sccb.h"
#include "duf_sccb_handle.h"

#include "duf_levinfo_ref.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccbh_eval.h"

#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ♠ */

/* ###################################################################### */
#include "duf_pdi_sccb_eval.h"
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
int
duf_ev_pdi_sccb( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, const mas_argvc_t * ptarg, bool f_summary )
{
  DUF_STARTR( r );

  duf_sccb_handle_t *sccbh = NULL;

/* assert( duf_levinfo_dirid( pdi ) ); */
  MAST_TRACE( sccbh, 0, "to open sccb handle %s at %s", sccb ? sccb->name : NULL, duf_levinfo_path( pdi ) );
  MAST_TRACE( path, 0, "@(to open sccbh) levinfo_path: %s", duf_levinfo_path( pdi ) );
/* T( "sccb:%d; dirid:%llu", sccb ? 1 : 0, duf_levinfo_dirid( pdi ) ); */
  sccbh = duf_sccb_handle_open( pdi, sccb, ptarg->argc, ptarg->argv, &r );
  if ( sccbh )
  {
    {
      MAST_TRACE( sccbh, 0, "(%d) opened to eval all & summ sccb handle (%d) %s", r, sccbh ? 1 : 0, sccb ? SCCB->name : "-" );
      DOR( r, DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( sccbh, f_summary ) ); /* XXX XXX XXX XXX XXX XXX */
      DUF_CLEAR_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
    }
    {
      int r1 = 0;

    /* T( "@@sccb:%d; dirid:%llu", sccb ? 1 : 0, duf_levinfo_dirid( pdi ) ); */
      DOR( r1, duf_sccb_handle_close( sccbh ) );
      if ( DUF_NOERROR( r ) && !DUF_NOERROR( r1 ) )
        r = r1;
    }
  }
  else
  {
    T( "sccbh not opened %d", sccbh ? 1 : 0 );
  }
  DUF_ENDR( r );
}

/* 20150922.123731 */
static int
duf_ev_pdi_evnamen( duf_depthinfo_t * pdi, const char *name, size_t len, duf_scan_callbacks_t * first, const mas_argvc_t * ptarg, bool f_summary )
{
  DUF_STARTR( r );
  const duf_scan_callbacks_t *sccb = NULL;

  assert( pdi );

  assert( duf_levinfo_node_type( pdi ) == DUF_NODE_NODE );

#if 0
  if ( 0 == strcmp( name, "NULL" ) )
  {
    DOR( r, duf_ev_pdi_sccb( pdi, NULL, ptarg, f_summary ) );        /* XXX XXX XXX XXX */
  }
  else
#endif
  {
#if 0
    if ( DUF_NOERROR( r ) )
      sccb = duf_find_sccb_by_evnamen( name, len, first );           /* XXX XXX */
#else
    if ( DUF_NOERROR( r ) )
      sccb = duf_find_or_load_sccb_by_evnamen( name, len, first );   /* XXX XXX */
#endif
    MAST_TRACE( sccb, 0, "evaluate sccb name '%s' [%s] : found act:%s", name, pdi->pdi_name, sccb ? sccb->name : "NONAME" );
    if ( sccb )
    {
      MAST_TRACE( path, 0, "@(to evaluate pdi sccb) [%s] levinfo_path: %s", sccb->name, duf_levinfo_path( pdi ) );

    /* T( "@sccb:%d; dirid:%llu", sccb ? 1 : 0, duf_levinfo_dirid( pdi ) ); */
      DOR( r, duf_ev_pdi_sccb( pdi, sccb, ptarg, f_summary ) );      /* XXX XXX XXX XXX */
    }
    else
    {
      DUF_MAKE_ERRORM( r, DUF_ERROR_SCCB_NOT_FOUND, "sccb module not found: '%s'", name );
    }
  }
  DUF_ENDR( r );
}

/* 20150922.123721 */
static int
duf_ev_pdi_evname( duf_depthinfo_t * pdi, const char *name, duf_scan_callbacks_t * first, const mas_argvc_t * ptarg, bool f_summary )
{
  DUF_STARTR( r );
  assert( pdi );

/* T( "name:%s; dirid:%llu", name, duf_levinfo_dirid( pdi ) ); */
  DOR( r, duf_ev_pdi_evnamen( pdi, name, strlen( name ), first, ptarg /*, pu */ , f_summary ) );
  DUF_ENDR( r );
}

/* 20150922.123718 */
int
duf_ev_pdi_evname_at( duf_depthinfo_t * pdi, const char *name, duf_scan_callbacks_t * first, const char *arg, bool f_summary )
{
  DUF_STARTR( r );

  mas_argvc_t targ = {.argc = 0,.argv = NULL };

  assert( pdi );

  if ( !arg )
    arg = duf_levinfo_path( pdi );
  targ.argc = mas_add_argv_arg( targ.argc, &targ.argv, arg );

  DOR( r, duf_ev_pdi_evname( pdi, name, first, &targ /*, pu */ , f_summary ) );

  mas_del_argv( targ.argc, targ.argv, 0 );
  DUF_ENDR( r );
}

/* 20150922.123706 */
int
duf_ev_pdi_evnamed_list( duf_depthinfo_t * pdi, const char *names, duf_scan_callbacks_t * first, const mas_argvc_t * ptarg, bool f_summary )
{
  DUF_STARTR( r );

  int ok = 0;
  const char *pnames;

  assert( pdi );

  pnames = names;
  MAST_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( pdi ) );

/* assert( pdi->pyp ); */
  while ( DUF_NOERROR( r ) && pnames && *pnames )
  {
    size_t len = 0;
    const char *ename = NULL;

    ename = strchr( pnames, ',' );

    len = ename ? ( size_t ) ( ename - pnames ) : strlen( pnames );
  /* T( "pnames:%s; dirid:%llu", pnames, duf_levinfo_dirid( pdi ) ); */
    DOR( r, duf_ev_pdi_evnamen( pdi, pnames, len, first, ptarg /*, pu */ , f_summary ) );
    if ( DUF_NOERROR( r ) )
      ok++;

    pnames = ( ename && *ename ) ? ename + 1 : NULL;
    ename = NULL;
  }
  if ( !ok )
  {
  /* DUF_SHOW_ERROR( "sccb not found: %s", names ); */
  }
  DUF_ENDR( r );
}
