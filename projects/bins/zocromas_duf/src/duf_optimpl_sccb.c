#include <assert.h>
#include <string.h>
#include <dlfcn.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_se.h>
#include <mastar/multiconfig/muc_options_file.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_action_table.h"                                        /* duf_first_sccb */

#include "duf_pdi_global.h"                                          /* duf_pdi_global() ✗ */

#include "duf_sccb.h"
#include "duf_sccb_eval_std.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_path2dirid.h"                                          /* duf_dirid2path; etc. ✗ */
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */

/* #include "duf_options_file.h" */

/* ###################################################################### */
#include "duf_optimpl_sccb.h"
/* ###################################################################### */

SR( SNIPPET_OPTION, optimpl_O_list_sccbs, void )
{
/* DUF_STARTR( r ); */
#if 0
  for ( duf_action_table_t * act = _duf_action_table(  ); act->sccb; act++ )
  {
    duf_scan_callbacks_t *sccb = act->sccb;

  /* DUF_PRINTF( 0, ".  %s", sccb->title ); */
    DUF_PRINTF( 0, "*%s: %s", sccb->name, duf_uni_scan_action_title( sccb ) );
  }
#else
  for ( duf_scan_callbacks_t * sccb = duf_first_sccb(  ); sccb; sccb = sccb->next )
  {
    DUF_PRINTF( 0, "*%s: %s", sccb->name, duf_uni_scan_action_title( sccb ) );
  }
#endif
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_list_sccbs, void );
}

#if 0
mas_error_code_t
duf_optimpl_O_list_sccb( int x_unused MAS_UNUSED )
{
  DUF_STARTR( r );
  for ( duf_action_table_t * act = _duf_action_table(  ); act->sccb; act++ )
  {
    DUF_PRINTF( 0, "* %s", duf_uni_scan_action_title( act->sccb ) );
  }
  DUF_ENDR( r );
}
#endif
/*! 20151114.204523 
 * \brief evaluate sccb
 *         for current directory
 *
 * */
SR( SNIPPET_OPTION, optimpl_O_evaluate_sccb, const char *names )
{
/* DUF_STARTR( r ); */

#if 0
  {
    void *han;

    han = dlopen( NULL, RTLD_NOLOAD | RTLD_LAZY );
    if ( han )
    {
      void *v;

      v = dlsym( han, "duf_ev_evnamed_list_std" );
      T( "han:%p : %p : %p", han, v, duf_ev_evnamed_list_std );

      dlclose( han );
    }
    T( "han:%p", han );
  }
  T( "@names:%s; dirid:%llu", names, duf_levinfo_dirid( duf_pdi_global(  ) ) );
#endif
  if ( duf_levinfo_dirid( duf_pdi_global(  ) ) )
  {
#if 0
    DOR( r, duf_ev_evnamed_list( names, _duf_action_table(  ) ) );
#else
    CR( ev_evnamed_list_std, names );
#endif
  }
  else
  {
  /* DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN ); (* 20160105.125904 *) */
  }
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_evaluate_sccb, const char *names );
}

/*! 20151114.204538
 * \brief call file
 * */

/*mas_error_code_t
duf_optimpl_O_call_file( const char *name )
*/
SR( SNIPPET_OPTION, optimpl_O_call_file, const char *name )
{
/* DUF_STARTR( r ); */
/* int r MAS_UNUSED = 0; */

  muc_CR( infile_options_at_filepath, duf_get_config_cli(  ), MUC_OPTION_STAGE_ANY /* FIXME should be current stage! */ , name,
          ( muc_option_adata_t * ) NULL /* paod */  );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_call_file, const char *name );
}

SR( SNIPPET_OPTION, optimpl_O_db_open, void )
{
/* DUF_STARTR( r ); */

  assert( duf_pdi_global(  ) );
  CR( main_db_open, duf_pdi_global(  ) );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_db_open, void );
}

SR( SNIPPET_OPTION, optimpl_O_cd, const char *s )
{
/* DUF_STARTR( r ); */
  if ( s && *s )
  {
#if 1
    char *new_path = NULL;

    if ( duf_levinfo_path( duf_pdi_global(  ) ) )
    {
      if ( *s == '/' )
      {
        new_path = mas_strdup( s );
      }
      else
      {
        new_path = mas_strdup( duf_levinfo_path( duf_pdi_global(  ) ) );
# if 0
        if ( new_path[strlen( new_path ) - 1] != '/' )
          new_path = mas_strcat_x( new_path, "/" );
# else
        {
          char *p;

          p = mas_normalize_path( new_path );
          mas_free( new_path );
          new_path = p;
        }
# endif
        new_path = mas_strcat_x( new_path, s );
      }
    }
    else if ( *s == '/' )
    {
      new_path = mas_strdup( s );
    }
    else
    {
      new_path = mas_strdup( s );
    }
    MAST_TRACE( path, 0, "cd to %s (now: %s)", new_path, duf_levinfo_path( duf_pdi_global(  ) ) );
    {

    /* duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set, int caninsert, int frecursive ) */
# if 0
      DOR( r, duf_pdi_reinit_anypath( duf_pdi_global(  ), new_path, ( const duf_ufilter_t * ) NULL, ( duf_sql_set_t * ) NULL, 1 /* caninsert */ ,
                                      duf_pdi_recursive( duf_pdi_global(  ) ), duf_pdi_allow_dirs( duf_pdi_global(  ) ),
                                      duf_pdi_linear( duf_pdi_global(  ) ) ) );
# else
      CR( pdi_reinit_anypath_global, new_path );
# endif
    }
    mas_free( new_path );
#else
    {

      T( "@@[%p] sql_beginning_done:%d", duf_pdi_global(  ), duf_pdi_root( duf_pdi_global(  ) )->sql_beginning_done );
      DOR( r, duf_pdi_reinit_anypath( duf_pdi_global(  ), s, NULL, 1 /* caninsert */ , duf_pdi_recursive( duf_pdi_global(  ) ) ) );
    }
#endif
  }
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_cd, const char *s );
}

SR( SNIPPET_OPTION, optimpl_O_cdid, long topdirid )
{
/* DUF_STARTR( r ); */
  char *path;

  path = duf_dirid2path( topdirid, QPERRIND );
  QT( "%lu: %s", topdirid, path );
  if ( QNOERR && path )
    CR( optimpl_O_cd, path );
  mas_free( path );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_cdid, long topdirid );
}

SR( SNIPPET_OPTION, optimpl_O_error_level, long lev )
{
/* DUF_STARTR( r ); */

  mas_force_offset_ereport( lev );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_error_level, long lev );
}

SR( SNIPPET_OPTION, optimpl_O_set_dir_priority, long prio MAS_UNUSED )
{
/* DUF_STARTR( r ); */

/* DOR( r, duf_pdi_reinit_anypath( duf_pdi_global(  ), new_path, ( const duf_ufilter_t * ) NULL, ( duf_sql_set_t * ) NULL, 1 (* caninsert *) , */
/*                                 duf_pdi_recursive( duf_pdi_global(  ) ) (*  *)  ) );                                                        */

  QTT( "@@@@@@dir priority to be set:%ld for %s", prio, "????" );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_set_dir_priority, long prio MAS_UNUSED );
}
