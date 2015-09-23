#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_utils_path.h"

#include "duf_action_table.h"

#include "duf_pdi_reinit.h"
#include "duf_pdi_ref.h"

#include "duf_sccb.h"
#include "duf_sccb_eval.h"
#include "duf_sccb_eval_std.h"

#include "duf_levinfo_ref.h"
#include "duf_maindb.h"


#include "duf_options_file.h"
/* #include "duf_option_descr.h" */
/* #include "duf_option_extended.h" */
/* #include "duf_option_names.h" */
/* #include "duf_option_restore.h" */
/* #include "duf_option.h" */

/* ###################################################################### */
#include "duf_option_sccb.h"
/* ###################################################################### */

duf_error_code_t
duf_option_$_list_sccbs( void )
{
  DEBUG_STARTR( r );
  for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
  {
    duf_scan_callbacks_t *sccb = act->sccb;

    /* DUF_PRINTF( 0, ".  %s", sccb->title ); */
    DUF_PRINTF( 0, "*%s: %s", sccb->name, duf_uni_scan_action_title( sccb ) );
  }
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_list_sccb( int x )
{
  DEBUG_STARTR( r );
  for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
  {
    DUF_PRINTF( 0, "* %s", duf_uni_scan_action_title( act->sccb ) );
  }
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_evaluate_sccb( const char *names )
{
  DEBUG_STARTR( r );

#if 0
  DOR( r, duf_ev_evnamed_list( names, duf_action_table(  ) ) );
#else
  DOR( r, duf_ev_evnamed_list_std( names ) );
#endif
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_call_file( const char *name )
{
  DEBUG_STARTR( r );
  int r DUF_UNUSED = 0;

  duf_infile_options_at_filepath( DUF_OPTION_STAGE_ANY, name );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_db_open( void )
{
  DEBUG_STARTR( r );

  DOR( r, duf_main_db_open(  ) );
  DEBUG_ENDR( r );
}

duf_error_code_t
duf_option_$_cd( const char *s )
{
  DEBUG_STARTR( r );
  if ( s && *s )
  {
#if 1
    char *new_path = NULL;

    if ( duf_levinfo_path( DUF_CONFIGG( pdi ) ) )
    {
      if ( *s == '/' )
      {
        new_path = mas_strdup( s );
      }
      else
      {
        new_path = mas_strdup( duf_levinfo_path( DUF_CONFIGG( pdi ) ) );
#  if 0
        if ( new_path[strlen( new_path ) - 1] != '/' )
          new_path = mas_strcat_x( new_path, "/" );
#  else
        new_path = duf_normalize_path( new_path );
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
    DUF_TRACE( path, 0, "cd to %s (now: %s)", new_path, duf_levinfo_path( DUF_CONFIGG( pdi ) ) );
    {

      /* duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set, int caninsert, int frecursive ) */

      DOR( r, duf_pdi_reinit_anypath( DUF_CONFIGG( pdi ), new_path, ( const duf_ufilter_t * ) NULL, ( duf_sql_set_t * ) NULL, 1 /* caninsert */ ,
                                      duf_pdi_recursive( DUF_CONFIGG( pdi ) ) /*  */  ) );
    }
    mas_free( new_path );
#else
    {

      T( "@@[%p] sql_beginning_done:%d", DUF_CONFIGG( pdi ), duf_pdi_root( DUF_CONFIGG( pdi ) )->sql_beginning_done );
      DOR( r, duf_pdi_reinit_anypath( DUF_CONFIGG( pdi ), s, NULL, 1 /* caninsert */ , duf_pdi_recursive( DUF_CONFIGG( pdi ) ) ) );
    }
#endif
  }
  DEBUG_ENDR( r );
}
