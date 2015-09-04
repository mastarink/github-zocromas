#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_config.h"
#include "duf_status_ref.h"

/* #include "duf_utils.h" */
#include "duf_sys_wrap.h"

#include "duf_option_defs.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"

#include "duf_levinfo_ref.h"

/* #include "duf_sql_defs.h" */
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_action.h"

#include "duf_maindb_info.h"

#include "duf_begfin.h"
#include "duf_selector.h"

#include "sql_beginning_common.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_vacuum.h"
#include "sql_beginning_create.h"

/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */

int
duf_main_db_attach_selected( const char *name )
{
  DEBUG_STARTR( r );
  static const char *sql = "ATTACH DATABASE '${DB_PATH}${SELECTED_DB}' AS duf${SELECTED_DB}";
  char *worksql;

  worksql = duf_expand_selected_db( sql, name );
  DORF( r, duf_main_db_open );
  DUF_TRACE( temp, 0, "@@@@@attach selected database %s", worksql );

  DUF_TRACE( explain, 0, "attach selected database %s", worksql );
  DUF_SQL_START_STMT_NOPDI( worksql, r, pstmt );
  DUF_SQL_STEP( r, pstmt );
  DUF_SQL_END_STMT_NOPDI( r, pstmt );
  DUF_TRACE( temp, 0, "@@@@attached selected database %s", worksql );
  mas_free( worksql );
  DEBUG_ENDR( r );
}

static int
duf_main_db_locate( void )
{
  DEBUG_STARTR( r );

  if ( duf_config->db.dir && duf_config->db.main.name )
  {
    DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", duf_config->db.dir, duf_config->db.main.name );
    /* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
    /* DUF_TRACE( action, 4, "db.dir:%s; db.name:%s", duf_config->db.dir, duf_config->db.main.name ); */
    duf_config->db.main.fpath = mas_strdup( duf_config->db.dir );
    duf_config->db.main.fpath = mas_strcat_x( duf_config->db.main.fpath, "/" );
    duf_config->db.main.fpath = mas_strcat_x( duf_config->db.main.fpath, duf_config->db.main.name );
    DUF_TRACE( explain, 0, "config->db.main.fpath set: %s", duf_config->db.main.fpath );
#ifdef MAS_SPLIT_DB
    if ( duf_config->db.adm.name )
    {
      DUF_TRACE( explain, 0, "setting config->db.adm.fpath by db.dir: %s and db.adm.name: %s", duf_config->db.dir, duf_config->db.adm.name );
      duf_config->db.adm.fpath = mas_strdup( duf_config->db.dir );
      duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, "/" );
      {
        char *n;

        n = strchr( duf_config->db.main.name, '.' );
        if ( !n )
          n = duf_config->db.main.name + strlen( duf_config->db.main.name );
        duf_config->db.adm.fpath = mas_strncat_x( duf_config->db.adm.fpath, duf_config->db.main.name, n - duf_config->db.main.name );
        duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, "_$_" );
      }
      duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, duf_config->db.adm.name );
      DUF_TRACE( explain, 0, "config->db.adm.fpath set: %s", duf_config->db.adm.fpath );
    }
    if ( duf_config->db.tempo.name )
    {
      DUF_TRACE( explain, 0, "setting config->db.tempo.fpath by db.dir: %s and db.tempo.name: %s", duf_config->db.dir, duf_config->db.tempo.name );
      duf_config->db.tempo.fpath = mas_strdup( duf_config->db.dir );
      duf_config->db.tempo.fpath = mas_strcat_x( duf_config->db.tempo.fpath, "/" );
      {
        char *n;

        n = strchr( duf_config->db.main.name, '.' );
        if ( !n )
          n = duf_config->db.main.name + strlen( duf_config->db.main.name );
        duf_config->db.tempo.fpath = mas_strncat_x( duf_config->db.tempo.fpath, duf_config->db.main.name, n - duf_config->db.main.name );
        duf_config->db.tempo.fpath = mas_strcat_x( duf_config->db.tempo.fpath, "_$_" );
      }
      duf_config->db.tempo.fpath = mas_strcat_x( duf_config->db.tempo.fpath, duf_config->db.tempo.name );
      DUF_TRACE( explain, 0, "config->db.tempo.fpath set: %s", duf_config->db.tempo.fpath );
    }
#if 0
    if ( duf_config->db.selected.name )
    {
      DUF_TRACE( explain, 0, "setting config->db.selected.fpath by db.dir: %s and db.selected.name: %s", duf_config->db.dir,
                 duf_config->db.selected.name );
      duf_config->db.selected.fpath = mas_strdup( duf_config->db.dir );
      duf_config->db.selected.fpath = mas_strcat_x( duf_config->db.selected.fpath, "/" );
      {
        char *n;

        n = strchr( duf_config->db.main.name, '.' );
        if ( !n )
          n = duf_config->db.main.name + strlen( duf_config->db.main.name );
        duf_config->db.selected.fpath = mas_strncat_x( duf_config->db.selected.fpath, duf_config->db.main.name, n - duf_config->db.main.name );
        duf_config->db.selected.fpath = mas_strcat_x( duf_config->db.selected.fpath, "_$_" );
      }
      duf_config->db.selected.fpath = mas_strcat_x( duf_config->db.selected.fpath, duf_config->db.selected.name );
      DUF_TRACE( explain, 0, "config->db.selected.fpath set: %s", duf_config->db.selected.fpath );
    }
#endif

#endif

    if ( duf_config->db.main.fpath )
    {
      DUF_TRACE( any, 0, "dbfile: %s", duf_config->db.main.fpath );
#ifdef MAS_SPLIT_DB
      DUF_TRACE( any, 0, "adm dbfile: %s", duf_config->db.adm.fpath );
      DUF_TRACE( any, 0, "tempo dbfile: %s", duf_config->db.tempo.fpath );
      /* DUF_TRACE( any, 0, "selected dbfile: %s", duf_config->db.selected.fpath ); */
#endif
    }
    else
      DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
  }
  else if ( !duf_config->db.dir )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.dir not set" );
  }
  else if ( !duf_config->db.main.name )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.main.name not set" );
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN );
  DEBUG_ENDR( r );
}

static int
duf_main_db_remove_files( void )
{
  DEBUG_STARTR( r );
  if ( duf_config->db.main.fpath )
    DORF( r, duf_unlink, duf_config->db.main.fpath );
  DEBUG_ENDR( r );
}

static int
duf_main_db_optionally_remove_files( void )
{
  DEBUG_STARTR( r );
  if ( DUF_ACTG_FLAG( remove_database ) )
  {
    DUF_TRACE( explain, 0, "     option %s, removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
    DORF( r, duf_main_db_remove_files );
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
  }
  DEBUG_ENDR( r );
}

static int
duf_main_db_create_tables( void )
{
  DEBUG_STARTR( r );
  DUF_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  /* DOR( r, duf_check_tables(  ) ); */
  if ( DUF_CLIG_FLAG( dry_run ) )
    DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( CREATE_TABLES ) );
  else
    DORF( r, duf_eval_sql_sequence, &sql_beginning_create, 0, NULL /* title */, NULL /* selected.db */ );

  DEBUG_ENDR( r );
}

static int
duf_main_db_pre_action( void )
{
  DEBUG_STARTR( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACTG_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_CLIG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      DORF( r, duf_eval_sql_sequence, &sql_beginning_clear, 0, NULL /* title */, NULL /* selected.db */ );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }
  if ( r >= 0 && DUF_ACTG_FLAG( vacuum ) )
  {
    /* static const char *sql = "VACUUM"; */

    /* DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) ); */
    if ( DUF_CLIG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
      DORF( r, duf_eval_sql_sequence, &sql_beginning_vacuum, 0, NULL /* title */, NULL /* selected.db */ );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }
/* --create-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 /* && DUF_ACTG_FLAG( create_tables ) */  )
  {
    DOR( r, duf_main_db_create_tables(  ) );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }
  DORF( r, duf_eval_sql_sequence, &sql_beginning_tables, 0, NULL /* title */, NULL /* selected.db */ );

  DEBUG_ENDR( r );
}

static int
duf_main_db_tune( void )
{
  DEBUG_STARTR( r );
#ifdef MAS_SPLIT_DB
  if ( duf_config->db.adm.fpath )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBADMALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, duf_config->db.adm.fpath );
    DUF_TRACE( explain, 0, "attach adm database %s", duf_config->db.adm.fpath );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, duf_config->db.adm.fpath, r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }
  if ( duf_config->db.tempo.fpath )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBTEMPALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, duf_config->db.tempo.fpath );
    DUF_TRACE( explain, 0, "attach tempo database %s -- %s", duf_config->db.tempo.fpath, sql );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, duf_config->db.tempo.fpath, r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }

  /* DOR( r, duf_main_db_attach_selected( "dumplet" ) ); */

#if 0
  if ( duf_config->db.selected.fpath )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBSELECTEDALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, duf_config->db.selected.fpath );
    DUF_TRACE( explain, 0, "attach selected database %s -- %s", duf_config->db.selected.fpath, sql );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, duf_config->db.selected.fpath, r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }
#endif

#endif

#if 0
  {
    static const char *sql = "PRAGMA synchronous = OFF";

    DUF_TRACE( explain, 0, "PRAGMA synchronous = OFF" );

    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
  }

  /* DOR( r, duf_sql_exec( "PRAGMA synchronous = OFF", ( int * ) NULL ) ); */
  /* DUF_TEST_R( r );                                                      */
  {
    static const char *sql = "PRAGMA encoding = 'UTF-8'";

    DUF_TRACE( explain, 0, "PRAGMA encoding = 'UTF-8'" );

    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
  }
#endif
/* TODO : part to only after possible tables creation */
  DOR( r, duf_eval_sql_sequence( &sql_beginning_common, 0, NULL /* title */, NULL /* selected.db */ ) ); /* PRAGMAs etc. */
  DEBUG_ENDR( r );
}

int
duf_main_db_open( void )
{
  DEBUG_STARTR( r );

  /* DUF_TRACE( temp, 0, "db:%s : %s", duf_config->db.main.name, duf_config->db.opened_name ); */
  if ( duf_config->db.opened_name && 0 != strcmp( duf_config->db.main.name, duf_config->db.opened_name ) )
  {
    DOR( r, duf_main_db_close( 0 ) );
  }
  if ( !duf_config->db.opened )
  {
    DORF( r, duf_main_db_locate );
    DORF( r, duf_main_db_optionally_remove_files );
    DORF( r, duf_sql_open, duf_config->db.main.fpath );
    duf_config->db.opened = ( r >= 0 );
    if ( r >= 0 )
      duf_config->db.opened_name = mas_strdup( duf_config->db.main.name );
    DORF( r, duf_main_db_tune );
    DORF( r, duf_main_db_pre_action );
  }

  DEBUG_ENDR( r );
}

int
duf_main_db_close( int ra )
{
  DEBUG_STARTR( r );
  r = ra;
  if ( duf_config->db.opened )
  {
    int rt = 0;

#if 0
    {
#  ifdef MAS_SPLIT_DB
      if ( duf_config->db.adm.fpath )
      {
        static const char *sql = "DETACH DATABASE 'adm'";

        DUF_TRACE( explain, 0, "detach adm database %s", duf_config->db.adm.fpath );
        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      if ( duf_config->db.tempo.fpath )
      {
        static const char *sql = "DETACH DATABASE 'tempo'";

        DUF_TRACE( explain, 0, "detach tempo database %s -- %s", duf_config->db.tempo.fpath, sql );
        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
#  endif
    }
#endif
    /* don't DOR it directly! call allways! */
    DORF( rt, duf_sql_close );

    if ( r == 0 && rt < 0 )
      DOR( r, rt );
    duf_config->db.opened = !( r >= 0 );
  }
  DEBUG_ENDR( r );
}

static int DUF_UNUSED
duf_store_log( int argc, char *const argv[] )
{
  DEBUG_STARTR( r );
  /* if ( argv ) */
  {
    char *sargv1, *sargv2;

#ifdef MAS_TRACING
    int changes = 0;
#else
    int DUF_UNUSED changes = 0;
#endif
    sargv1 = mas_argv_string( argc, argv, 1 );
    sargv2 = duf_restore_some_options( argv[0] );
    DUF_TRACE( any, 0, "restored optd:%s", sargv2 );
    DORF( r, duf_main_db_open );
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
  }
  DEBUG_ENDR( r );
}

/* do necessary actions to perform tasks, formulated in duf_config global variable and ... for standard database
 *    - global variable duf_config must be created/inited and set
 * ***************************************************************************************
 * 1. build database path
 * 2. optionally call duf_config_show (see also duf_show_options call at duf_main)
 * 3. optionally remove existing database files (dangerous); (* to make sure it's before opening database *)
 * 4. call duf_sql_open to open database
 *   5. make necessary initial sql at database
 *   6. call duf_action to perform tasks
 *   7. optionally collect statistics from database ('info' option) by calling duf_info_from_db
 * 8. call duf_sql_close to close database
 * */
int
duf_main_db( int argc, char **argv )
{
  DEBUG_STARTR( r );

  DUF_VERBOSE( 0, "verbose test 0> %d %s", 17, "hello" );
  DUF_VERBOSE( 1, "verbose test 1> %d %s", 17, "hello" );

  DORF( r, duf_config_optionally_show );
  /* DORF( r, duf_main_db_open ); */

  DUF_TRACE( temporary, 0, "@ maxitems.total %lld", duf_config->pu->maxitems.total );
  DUF_TRACE( temporary, 0, "@ maxitems.files %lld", duf_config->pu->maxitems.files );
  DUF_TRACE( temporary, 0, "@ maxitems.dirs %lld", duf_config->pu->maxitems.dirs );
  DUF_TRACE( temporary, 0, "@ dirfiles.min %u", duf_config->pu->dirfiles.min );
  DUF_TRACE( temporary, 0, "@ dirfiles.max %u", duf_config->pu->dirfiles.max );
#ifdef MAS_TRACING
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->pu->globx.include_fs_files.argc, duf_config->pu->globx.include_fs_files.argv, 0 );
    DUF_TRACE( temporary, 0, "@ include-fs %s", sif );
    mas_free( sif );
  }
  {
    char *sif = NULL;

    sif = mas_argv_string( duf_config->pu->globx.exclude_fs_files.argc, duf_config->pu->globx.exclude_fs_files.argv, 0 );
    DUF_TRACE( temporary, 0, "@ exclude-fs %s", sif );
    mas_free( sif );
  }
#endif
  DUF_TEST_RX_START( r );
  DUF_SHOW_ERROR( "db not opened @ %s ( %s )", duf_config->db.main.fpath, duf_error_name( r ) );
  DUF_TEST_RX_END( r );

#if 0
  if ( duf_config->targ.argc > 0 )
    DORF( r, duf_store_log, duf_config->targ.argc, duf_config->targ.argv );
#endif

  DORF( r, DUF_WRAPPED( duf_action ) /* , argc, argv */  ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
  DORF( r, duf_main_db_close, r ); /* [@] */
  DEBUG_ENDR( r );
}
