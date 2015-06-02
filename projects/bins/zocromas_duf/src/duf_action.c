#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_cli_options.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"
#include "duf_option_defs.h"

#include "duf_sql.h"
#include "duf_sql2.h"

/* #include "duf_create.h" - obsolete 20150522.122406 */
#include "duf_add.h"
#include "duf_uni_scan.h"

#include "duf_pdi.h"
#include "duf_interactive.h"

#include "duf_sql_defs.h"

#include "duf_begfin.h"
#include "sql_beginning_create.h"
#include "sql_beginning_vacuum.h"
#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */


static int
duf_store_log( int argc, char *const argv[] )
{
  DEBUG_STARTR( r );
  char *sargv1, *sargv2;
  int changes = 0;

  sargv1 = mas_argv_string( argc, argv, 1 );
  sargv2 = duf_restore_some_options( argv[0] );
  DUF_TRACE( any, 0, "restored optd:%s", sargv2 );
  {
    static const char *sql = "INSERT OR IGNORE INTO " DUF_DBADMPREF "log (args, restored_args, msg) VALUES (:Args, :restoredArgs, '')";

    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_BIND_S( Args, sargv1, r, pstmt );
    DUF_SQL_BIND_S( restoredArgs, sargv2, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    /* if ( r == DUF_ERROR_SQL_NO_TABLE ) */
    /*   r = 0;                           */
  }
  DUF_TRACE( action, 0, "LOG inserted %d/%d [%s] - %d", changes, r, sargv1, argc );
  mas_free( sargv2 );
  mas_free( sargv1 );
  DEBUG_ENDR( r );
}

static int
duf_pre_action( void )
{
  DEBUG_STARTR( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
    {
      /* DOR( r, duf_clear_tables(  ) ); */
      DORF( r, duf_eval_sql_sequence, &sql_beginning_clear, 0, NULL );
    }
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }
  if ( r >= 0 && DUF_ACT_FLAG( vacuum ) )
  {
    /* static const char *sql = "VACUUM"; */

    /* DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) ); */
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
    {
      /* DUF_SQL_START_STMT_NOPDI( sql, r, pstmt ); */
      /* DUF_SQL_STEP( r, pstmt );                  */
      /* DUF_SQL_END_STMT_NOPDI( r, pstmt );        */
      DORF( r, duf_eval_sql_sequence, &sql_beginning_vacuum, 0, NULL );
    }
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }
/* --create-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( create_tables ) )
  {
    DUF_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
    /* DOR( r, duf_check_tables(  ) ); */
    DORF( r, duf_eval_sql_sequence, &sql_beginning_create, 0, NULL );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }
  duf_eval_sql_sequence( &sql_beginning_tables, 0, NULL );

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
int
duf_action( int argc, char **argv )
{
  DEBUG_STARTR( r );
#if 0
  const char *node_selector2 = " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND dirname=:dirName";

#elif 0
  extern duf_scan_callbacks_t duf_print_tree_callbacks;
  const char *node_selector2 = duf_print_tree_callbacks.node.selector2;
#else
  const char *node_selector2 = NULL;
#endif


  DUF_TRACE( explain, 0, "to do actions" );
  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );

  DUF_E_SET( -96, DUF_ERROR_NO_ACTIONS );
  /* DUF_E_SET( 97, DUF_ERROR_TOO_DEEP, DUF_ERROR_NOT_IN_DB, (* DUF_ERROR_MAX_SEQ_REACHED, *) DUF_ERROR_MAX_REACHED ); */

  DORF( r, duf_pre_action );
  DORF( r, duf_store_log, argc, argv );

/* --add-path									*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( add_path ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_FLAG_NAME( ADD_PATH ) );
    for ( int ia = 0; r >= 0 && ia < duf_config->targ.argc; ia++ )
    {
      DUF_TRACE( explain, 0, "to add (#%d) path %s", ia, duf_config->targ.argv[ia] );
      if ( DUF_CLI_FLAG( dry_run ) )
        DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( ADD_PATH ) );
      else
        DOR( r, duf_add_path_uni( duf_config->targ.argv[ia], node_selector2 ) );
      global_status.actions_done++;
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, you may need it for adding initial path", DUF_OPT_FLAG_NAME( ADD_PATH ) );
  }
  assert( duf_config );
  assert( duf_config->pdi );


/* re-init after db is open, created, paths are ready at targ.argv, paths added */
  {
    const char *path = NULL;

    path = duf_config->targ.argc > 0 ? duf_config->targ.argv[0] : "/";
    DOR( r, duf_pdi_reinit_anypath( duf_config->pdi, path, node_selector2 /* , duf_config->pu, DUF_U_FLAG( recursive ) */  ) );

    /* stage DUF_OPTION_STAGE_FIRST  (1) - needs pdi inited with argv, which is known only after stage 0 */
    DOR( r, duf_parse_cli_options( duf_config->cli.shorts, DUF_OPTION_STAGE_FIRST ) );
  }
  assert( duf_config->pdi->levinfo );

  if ( DUF_ACT_FLAG( interactive ) )
  {
    DOR( r, duf_interactive(  ) );
  }
  else if ( r >= 0 /* && DUF_ACT_FLAG( uni_scan ) */  )
    DORF( r, DUF_WRAPPED( duf_evaluate_all_at_config ) ); /* each targ.argv; reinit will be made */

  DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
  DUF_TRACE( explain, 0, "after actions" );

  DEBUG_ENDR( r );
}
