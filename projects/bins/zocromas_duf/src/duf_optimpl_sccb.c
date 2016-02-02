#include <string.h>
#include <dlfcn.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_status_ref.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"     /* DUF_PRINTF ; DUF_CONFIGG */
#include "duf_config_defs.h"

#include "duf_action_table.h"


#include "duf_pdi_reinit.h"
#include "duf_pdi_ref.h"

#include "duf_sccb.h"
#include "duf_sccb_eval.h"
#include "duf_sccb_eval_std.h"

#include "duf_levinfo_ref.h"
#include "duf_path2dirid.h"
#include "duf_maindb.h"


#include "duf_options_file.h"

/* ###################################################################### */
#include "duf_optimpl_sccb.h"
/* ###################################################################### */

mas_error_code_t
duf_option_O_list_sccbs( void )
{
  DEBUG_STARTR( r );
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
  DEBUG_ENDR( r );
}

#if 0
mas_error_code_t
duf_option_O_list_sccb( int x_unused DUF_UNUSED )
{
  DEBUG_STARTR( r );
  for ( duf_action_table_t * act = _duf_action_table(  ); act->sccb; act++ )
  {
    DUF_PRINTF( 0, "* %s", duf_uni_scan_action_title( act->sccb ) );
  }
  DEBUG_ENDR( r );
}
#endif
/*! 20151114.204523 
 * \brief evaluate sccb
 *         for current directory
 *
 * */
mas_error_code_t
duf_option_O_evaluate_sccb( const char *names )
{
  DEBUG_STARTR( r );


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
  T( "@names:%s; dirid:%llu", names, duf_levinfo_dirid( global_status.scn.pdi ) );
#endif
  if ( duf_levinfo_dirid( global_status.scn.pdi ) )
  {
#if 0
    DOR( r, duf_ev_evnamed_list( names, _duf_action_table(  ) ) );
#else
    DOR( r, duf_ev_evnamed_list_std( names ) );
#endif
  }
  else
  {
    /* DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN ); (* 20160105.125904 *) */
  }
  DEBUG_ENDR( r );
}

/*! 20151114.204538
 * \brief call file
 * */


/*mas_error_code_t
duf_option_O_call_file( const char *name )
*/
SR( SNIPPET_OPTION, option_O_call_file, const char *name )
{
  /* DEBUG_STARTR( r ); */
  /* int r DUF_UNUSED = 0; */

  CR( infile_options_at_filepath, DUF_OPTION_STAGE_ANY /* FIXME should be current stage! */ , name, ( duf_option_adata_t * ) NULL /* paod */  );
  /* DEBUG_ENDR( r ); */
  ER( SNIPPET_OPTION, option_O_call_file, const char *name );
}

mas_error_code_t
duf_option_O_db_open( void )
{
  DEBUG_STARTR( r );

  DOR( r, duf_main_db_open( global_status.scn.pdi ) );
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_O_cd( const char *s )
{
  DEBUG_STARTR( r );
  if ( s && *s )
  {
#if 1
    char *new_path = NULL;

    if ( duf_levinfo_path( global_status.scn.pdi ) )
    {
      if ( *s == '/' )
      {
        new_path = mas_strdup( s );
      }
      else
      {
        new_path = mas_strdup( duf_levinfo_path( global_status.scn.pdi ) );
#  if 0
        if ( new_path[strlen( new_path ) - 1] != '/' )
          new_path = mas_strcat_x( new_path, "/" );
#  else
        {
          char *p;

          p = mas_normalize_path( new_path );
          mas_free( new_path );
          new_path = p;
        }
#  endif
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
    DUF_TRACE( path, 0, "cd to %s (now: %s)", new_path, duf_levinfo_path( global_status.scn.pdi ) );
    {

      /* duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set, int caninsert, int frecursive ) */

      DOR( r, duf_pdi_reinit_anypath( global_status.scn.pdi, new_path, ( const duf_ufilter_t * ) NULL, ( duf_sql_set_t * ) NULL, 1 /* caninsert */ ,
                                      duf_pdi_recursive( global_status.scn.pdi ), duf_pdi_allow_dirs( global_status.scn.pdi ),
                                      duf_pdi_linear( global_status.scn.pdi ) ) );
    }
    mas_free( new_path );
#else
    {

      T( "@@[%p] sql_beginning_done:%d", global_status.scn.pdi, duf_pdi_root( global_status.scn.pdi )->sql_beginning_done );
      DOR( r, duf_pdi_reinit_anypath( global_status.scn.pdi, s, NULL, 1 /* caninsert */ , duf_pdi_recursive( global_status.scn.pdi ) ) );
    }
#endif
  }
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_O_cdid( long topdirid )
{
  DEBUG_STARTR( r );
  char *path;

  path = duf_dirid2path( topdirid, &r );
  T( "%lu: %s", topdirid, path );
  if ( DUF_NOERROR( r ) && path )
    r = duf_option_O_cd( path );
  mas_free( path );
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_O_error_level( long lev )
{
  DEBUG_STARTR( r );

  mas_force_offset_ereport( lev );
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_O_set_dir_priority( long prio )
{
  DEBUG_STARTR( r );

  /* DOR( r, duf_pdi_reinit_anypath( global_status.scn.pdi, new_path, ( const duf_ufilter_t * ) NULL, ( duf_sql_set_t * ) NULL, 1 (* caninsert *) , */
  /*                                 duf_pdi_recursive( global_status.scn.pdi ) (*  *)  ) );                                                        */

  TT( "@@@@@@dir priority to be set:%ld for %s", prio, "????" );
  DEBUG_ENDR( r );
}
