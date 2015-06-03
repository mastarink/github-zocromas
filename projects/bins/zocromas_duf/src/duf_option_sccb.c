#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"



#include "duf_prepare_actions.h"
#include "duf_sccb.h"
#include "duf_sccb_eval.h"

#include "duf_levinfo_ref.h"
#include "duf_maindb.h"


#include "duf_option_descr.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"
#include "duf_option.h"

#include "duf_uni_scan.h"

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
  int ok = 0;
  const char *pnames;

  pnames = names;
  DUF_TRACE( path, 0, "@ levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );
  while ( pnames && *pnames )
  {
    const char *ename = NULL;

    ename = strchr( pnames, ',' );
    for ( duf_action_table_t * act = duf_action_table(  ); act->sccb; act++ )
    {
      size_t len = 0;
      duf_scan_callbacks_t *sccb = act->sccb;

      if ( ename )
        len = ename - pnames;
      else
        len = strlen( pnames );
      /* DUF_PRINTF( 0, "%s : %s ----- %s", name, sccb->name, duf_uni_scan_action_title( sccb ) ); */
      if ( 0 == strncmp( pnames, sccb->name, len ) )
      {
        int r = 0;

        DUF_TRACE( path, 0, "@ (to duf_evaluate_sccb) levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );


        DOR( r, duf_evaluate_sccb( sccb ) );
        ok++;
      }
      /* DUF_PRINTF( 0, ".  %s", sccb->title ); */
    }
    /* DUF_PRINTF( 0, "pnames:%s; ename:%s", pnames, ename ); */
    if ( ename && *ename )
      pnames = ename + 1;
    else
      pnames = NULL;
    ename = NULL;
  }
  if ( !ok )
  {
    DUF_SHOW_ERROR( "sccb not found: %s", names );
  }
}

void
duf_option_$_db_open( void )
{
  (void) duf_main_db_open(  );
}
