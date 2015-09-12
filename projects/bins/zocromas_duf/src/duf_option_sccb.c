#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"



#include "duf_action_table.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"

#include "duf_sccb.h"
/* #include "duf_sccb_eval.h" */
#include "duf_sccb_eval_std.h"

#include "duf_levinfo_ref.h"
#include "duf_maindb.h"


/* #include "duf_option_descr.h" */
/* #include "duf_option_extended.h" */
/* #include "duf_option_names.h" */
/* #include "duf_option_restore.h" */
/* #include "duf_option.h" */

/* ###################################################################### */
#include "duf_option_sccb.h"
/* ###################################################################### */

void
duf_option_$_list_sccbs( void )
{
  for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
  {
    duf_scan_callbacks_t *sccb = act->sccb;

    /* DUF_PRINTF( 0, ".  %s", sccb->title ); */
    DUF_PRINTF( 0, "*%s: %s", sccb->name, duf_uni_scan_action_title( sccb ) );
  }
}

void
duf_option_$_list_sccb( int x )
{
  for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
  {
    DUF_PRINTF( 0, "* %s", duf_uni_scan_action_title( act->sccb ) );
  }
}

void
duf_option_$_evaluate_sccb( const char *names )
{
#if 0
  ( void ) duf_evaluate_sccb_named_list( names, duf_action_table(  ) );
#else
  int r = 0;

  DOR( r, duf_evaluate_sccb_named_list_std( names ) );

#endif
}

void
duf_option_$_db_open( void )
{
  int rt = 0;

  DOR( rt, duf_main_db_open(  ) );
  DUF_TEST_R( rt );
}

void
duf_option_$_cd( const char *s )
{
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
        if ( new_path[strlen( new_path ) - 1] != '/' )
          new_path = mas_strcat_x( new_path, "/" );
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
      int rt = 0;

      DOR( rt, duf_pdi_reinit_anypath( DUF_CONFIGG( pdi ), new_path, 1 /* caninsert */ , NULL, duf_pdi_recursive( DUF_CONFIGG( pdi ) ) ) );
    }
    mas_free( new_path );
#else
    {
      int rt = 0;

      DOR( rt, duf_pdi_reinit_anypath( DUF_CONFIGG( pdi ), s, 1 /* caninsert */ , NULL, duf_pdi_recursive( DUF_CONFIGG( pdi ) ) ) );
    }
#endif
  }
}
