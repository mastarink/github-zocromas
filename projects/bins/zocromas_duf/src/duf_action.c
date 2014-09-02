/* File #4 20140902.174509 */
#include <string.h>
#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_defs.h"
#include "duf_maintenance.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_cli_options.h"
#include "duf_options.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_create.h"

#include "duf_add.h"

#include "duf_filedata.h"
#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */


static int
duf_store_log( int argc, char *const argv[] )
{
  int r = 0;
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
  return r;
}

int
duf_action( int argc, char **argv )
{
  int r = 0;

  DUF_TEST_R( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      r = duf_clear_tables(  );
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }
  DUF_TEST_R( r );
  if ( r >= 0 && DUF_ACT_FLAG( vacuum ) )
  {
    static const char *sql = "VACUUM";

    DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) );
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
    {
      DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
    }
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }
  DUF_TEST_R( r );
/* --create-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( create_tables ) )
  {
    DUF_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
    r = duf_check_tables(  );
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }

  DUF_TEST_R( r );
  if ( r >= 0 )
    r = duf_store_log( argc, argv );
  DUF_TEST_R( r );
  DUF_TEST_R( r );

/* --add-path									*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( add_path ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_FLAG_NAME( ADD_PATH ) );
    for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
    {
      DUF_TRACE( explain, 0, "to add (#%d) path %s", ia, duf_config->targv[ia] );
      if ( DUF_CLI_FLAG( dry_run ) )
        DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( ADD_PATH ) );
      else
        r = duf_add_path_uni( duf_config->targv[ia] );
      duf_config->actions_done++;
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, you may need it for adding initial path", DUF_OPT_FLAG_NAME( ADD_PATH ) );
  }
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.md5id.min, duf_config->u.md5id.max );   */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.nameid.min, duf_config->u.nameid.max ); */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.inode.min, duf_config->u.inode.max );   */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %llu %llu", duf_config->u.size.min, duf_config->u.size.max ); */
  DUF_TEST_R( r );
  if ( r >= 0 && DUF_ACT_FLAG( uni_scan ) )
    r = duf_make_all_sccbs_wrap(  );
  return r;
}
