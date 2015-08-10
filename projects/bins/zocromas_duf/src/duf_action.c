#include <string.h>
#include <assert.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_utils_path.h"


#include "duf_cli_options.h"
#include "duf_option_file.h"
#include "duf_option_names.h"
#include "duf_option_defs.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_eval_at_config.h"

#include "duf_levinfo_ref.h"



#include "duf_pdi.h"
#include "duf_interactive.h"

#include "duf_sql_defs.h"

#include "duf_begfin.h"
/* #include "sql_beginning_create.h" */
/* #include "sql_beginning_vacuum.h" */
/* #include "sql_beginning_tables.h" */

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */


static int
duf_action_path( char *tpath )
{
  DEBUG_STARTR( r );
  /* const char *node_selector2 = NULL; */

  assert( duf_config );
  assert( duf_config->pdi );
  /* assert( !duf_levinfo_path( duf_config->pdi ) ); */
  DUF_TRACE( path, 0, "@[%s] %s", tpath, duf_levinfo_path( duf_config->pdi ) );

/* --add-path									*/ DEBUG_STEP(  );
  if ( r >= 0 )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_FLAG_NAME( ADD_PATH ) );
    DUF_TRACE( explain, 0, "to add path %s", tpath );
    if ( DUF_ACT_FLAG( add_path ) )
    {
      if ( DUF_CLI_FLAG( dry_run ) )
        DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( ADD_PATH ) );
      else
      {
        /* char *real_path = NULL; */

        /* real_path = duf_realpath( tpath, &r ); */
        duf_config->pdi->opendir = 1;
        duf_config->pdi->recursive = 1;

        DUF_TRACE( path, 0, "@[%s] %s", tpath, duf_levinfo_path( duf_config->pdi ) );

        DOR( r, duf_pdi_reinit_anypath( duf_config->pdi, tpath, 1 /* caninsert */ ,
                                        NULL /* node_selector2 *//* , duf_config->pu, DUF_U_FLAG( recursive ) */  ) );

        /* DOR( r, duf_add_path_uni( tpath, node_selector2 ) ); */
        /* DOR( r, DUF_WRAPPED( duf_pdi_init ) ( duf_config->pdi, real_path, 1 (* caninsert ~ "root" *) , node_selector2, */
        /*                                       1 (* recursive *) , 0 (* opendir *)  ) );                                */
        /* mas_free( real_path ); */
      }
      global_status.actions_done++;
    }

    {
      DOR( r, duf_pdi_reinit_anypath( duf_config->pdi, tpath, 0 /* caninsert */ ,
                                      NULL /* node_selector2 *//* , duf_config->pu, DUF_U_FLAG( recursive ) */  ) );

      /* stage DUF_OPTION_STAGE_FIRST  (1) - needs pdi inited with argv, which is known only after stage 0 */

      /*!XXX here duf_levinfo_path( duf_config->pdi ) is valid XXX! */

      assert( duf_levinfo_path( duf_config->pdi ) );
      DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );

      /* DOR( r, duf_parse_cli_options( duf_config->cli.shorts, DUF_OPTION_STAGE_FIRST ) ); */
      DUF_TRACE( options, 0, "@stage_first cli_options" );
      DOR( r, duf_cli_options( DUF_OPTION_STAGE_FIRST ) );
      DUF_TRACE( options, 0, "@stage_first indirect_options" );
      DOR( r, duf_indirect_options( DUF_OPTION_STAGE_FIRST ) );

#if 1
      /* TODO this will not work for path except first */
      DUF_TRACE( options, 0, "@stage_first stdin_options" );
      DOR( r, duf_stdin_options( DUF_OPTION_STAGE_FIRST ) );
#endif


      DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );
    }
#if 0
    duf_pdi_close( duf_config->pdi );
#endif
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, you may need it for adding initial path", DUF_OPT_FLAG_NAME( ADD_PATH ) );
  }

  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );


  DUF_TRACE( path, 0, "@levinfo_path: %s", duf_levinfo_path( duf_config->pdi ) );
  /* assert( duf_config->pdi->levinfo ); */

  if ( DUF_ACT_FLAG( interactive ) )
  {
    if ( isatty( STDIN_FILENO ) )
      DOR( r, duf_interactive(  ) );
  }
#if 0
  else if ( r >= 0 /* && DUF_ACT_FLAG( uni_scan ) */  )
  {
    /* TODO with new interface duf_evaluate_all_at_config is needless; remove also corresponding options */
    DORF( r, DUF_WRAPPED( duf_evaluate_all_at_config ) ); /* each targ.argv; reinit will be made */
  }
#endif
  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  DUF_TRACE( explain, 0, "after actions" );

  DEBUG_ENDR( r );
}

/* do necessary actions to perform tasks, formulated in duf_config global variable and ... for opened database
 *    - global variable duf_config must be created/inited and set
 *    - database must be opened
 * ***************************************************************************************
 * 1. optionally drop (all) tables from database by calling duf_clear_tables
 * 2. optionally vacuum database by performing specific sql
 * 3. optionally create/check tables at database by calling duf_check_tables
 * 4. optionally add given path(s) to database by calling duf_add_path_uni
 * 5. re-init (structures?) by calling duf_pdi_reinit_anypath
 * 6. stage 1 parse/execute options by calling duf_parse_cli_options
 * 7. optionally call duf_interactive to enter interactive mode of command execution
 * 8. optionally call duf_evaluate_all_at_config_wrap to scan files and/or db records to perform given tasks
 * TODO split to functions:
 *   1. ...init (1..6?)
 *   2. ...execute
 * */
DUF_WRAPSTATIC int
duf_action( void )
{
  DEBUG_STARTR( r );


  assert( !duf_levinfo_path( duf_config->pdi ) );

  DUF_TRACE( explain, 0, "to do actions" );
  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );

  DUF_E_SET( -96, DUF_ERROR_NO_ACTIONS );
  /* DUF_E_SET( 97, DUF_ERROR_TOO_DEEP, DUF_ERROR_NOT_IN_DB, (* DUF_ERROR_MAX_SEQ_REACHED, *) DUF_ERROR_MAX_REACHED ); */
#if 0
  DORF( r, duf_pre_action );
#endif


  /* TODO */
#if 1
  if ( duf_config->targ_offset < duf_config->targ.argc )
  {
    char *real_path = NULL;

    real_path = duf_realpath( ".", &r );
    duf_config->pdi->opendir = 1;
    duf_config->pdi->recursive = 1;
    DOR( r, DUF_WRAPPED( duf_pdi_init ) ( duf_config->pdi, real_path, 1 /* caninsert */ , NULL /* node_selector2 */ , 1 /* recursive */ ,
                                          0 /* opendir */  ) );
    mas_free( real_path );
  }
#endif


  for ( int ia = duf_config->targ_offset; r >= 0 && ia < duf_config->targ.argc; ia++ )
  {
    DUF_TRACE( path, 0, "@%d. [%s] %s", ia, duf_config->targ.argv[ia], duf_levinfo_path( duf_config->pdi ) );
    DOR( r, duf_action_path( duf_config->targ.argv[ia] ) );
  }
  /* assert( duf_config->pdi->levinfo ); */

  if ( DUF_ACT_FLAG( interactive ) && isatty( STDIN_FILENO ) )
  {
    DOR( r, duf_interactive(  ) );
  }
  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  DUF_TRACE( explain, 0, "after actions" );

  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
int
duf_action_wrap( void /* int argc, char **argv */ )
{
  DEBUG_STARTR( r );

  assert( !duf_levinfo_path( duf_config->pdi ) );
  DORF( r, duf_action /* , argc, argv */ ); /* duf_action : duf_action.c */
#  if 0
  DORF( r, duf_main_db_info );
#  endif
  if ( r < 0 && r != DUF_ERROR_MAX_REACHED )
  {
    DUF_TEST_RX_SHOW( r, "action FAIL" );
  }

  DEBUG_ENDR( r );
}
#endif
