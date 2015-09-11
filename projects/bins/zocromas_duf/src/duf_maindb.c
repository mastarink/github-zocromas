#include <assert.h>
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
/* #include "duf_option_restore.h" */

#include "duf_levinfo_ref.h"

/* #include "duf_sql_defs.h" */
#include "duf_sql.h"
#include "duf_sql2.h"

/* #include "duf_action.h" */
#include "duf_options.h"
#include "duf_pdi.h"

#include "duf_maindb_info.h"

#include "duf_begfin.h"
#include "duf_selector.h"

#include "sql_tables_defs.h"
#include "sql_beginning_common.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_vacuum.h"
#include "sql_beginning_create.h"

/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */

#if 0
static int
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
#endif

static int
duf_main_db_locate( void )
{
  DEBUG_STARTR( r );

  if ( DUF_CONFIGG( db.dir ) && DUF_CONFIGG( db.main.name ) )
  {
    DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", DUF_CONFIGG( db.dir ), DUF_CONFIGG( db.main.name ) );
    /* TODO move db.main.fpath and db.adm.fpath and db.tempo.fpath from duf_config to tmp etc. - it's not config values */
    /* DUF_TRACE( action, 4, "db.dir:%s; db.name:%s", DUF_CONFIGG(db.dir), DUF_CONFIGG(db.main.name) ); */
    mas_free( DUF_CONFIGW( db.main.fpath ) );
    /* TODO to use something like duf_expand_selected_db() here TODO */
    DUF_CONFIGW( db.main.fpath ) = mas_strdup( DUF_CONFIGG( db.dir ) );
    DUF_CONFIGW( db.main.fpath ) = mas_strcat_x( DUF_CONFIGG( db.main.fpath ), "/" );
    DUF_CONFIGW( db.main.fpath ) = mas_strcat_x( DUF_CONFIGG( db.main.fpath ), DUF_CONFIGG( db.main.name ) );
    DUF_CONFIGW( db.main.fpath ) = mas_strcat_x( DUF_CONFIGG( db.main.fpath ), ".db" );
    DUF_TRACE( explain, 0, "config->db.main.fpath set: %s", DUF_CONFIGG( db.main.fpath ) );
#ifdef MAS_SPLIT_DB
    if ( DUF_CONFIGG( db.adm.name ) )
    {
      DUF_TRACE( explain, 0, "setting config->db.adm.fpath by db.dir: %s and db.adm.name: %s", DUF_CONFIGG( db.dir ), DUF_CONFIGG( db.adm.name ) );
      mas_free( DUF_CONFIGW( db.adm.fpath ) );
      DUF_CONFIGW( db.adm.fpath ) = mas_strdup( DUF_CONFIGG( db.dir ) );
      DUF_CONFIGW( db.adm.fpath ) = mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), "/" );
      {
        char *n;

        n = strchr( DUF_CONFIGG( db.main.name ), '.' );
        if ( !n )
          n = DUF_CONFIGG( db.main.name ) + strlen( DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.adm.fpath ) = mas_strncat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGG( db.main.name ), n - DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.adm.fpath ) = mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), "_$_" );
      }
      DUF_CONFIGW( db.adm.fpath ) = mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), DUF_CONFIGG( db.adm.name ) );
      DUF_CONFIGW( db.adm.fpath ) = mas_strcat_x( DUF_CONFIGG( db.adm.fpath ), ".db" );
      DUF_TRACE( explain, 0, "config->db.adm.fpath set: %s", DUF_CONFIGG( db.adm.fpath ) );
    }
#ifndef DUF_SQL_TTABLES_TEMPORARY
    if ( DUF_CONFIGG( db.tempo.name ) )
    {
      DUF_TRACE( explain, 0, "setting config->db.tempo.fpath by db.dir: %s and db.tempo.name: %s", DUF_CONFIGG( db.dir ),
                 DUF_CONFIGG( db.tempo.name ) );
      mas_free( DUF_CONFIGW( db.tempo.fpath ) );
      DUF_CONFIGW( db.tempo.fpath ) = mas_strdup( DUF_CONFIGG( db.dir ) );
      DUF_CONFIGW( db.tempo.fpath ) = mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), "////" );
      {
        char *n;

        n = strchr( DUF_CONFIGG( db.main.name ), '.' );
        if ( !n )
          n = DUF_CONFIGG( db.main.name ) + strlen( DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.tempo.fpath ) = mas_strncat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGG( db.main.name ), n - DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.tempo.fpath ) = mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), "_$_" );
      }
      DUF_CONFIGW( db.tempo.fpath ) = mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), DUF_CONFIGG( db.tempo.name ) );
      DUF_CONFIGW( db.tempo.fpath ) = mas_strcat_x( DUF_CONFIGG( db.tempo.fpath ), ".db" );
      DUF_TRACE( explain, 0, "config->db.tempo.fpath set: %s", DUF_CONFIGG( db.tempo.fpath ) );
    }
#endif
#  if 0
    if ( DUF_CONFIGG( db.selected.name ) )
    {
      DUF_TRACE( explain, 0, "setting config->db.selected.fpath by db.dir: %s and db.selected.name: %s", DUF_CONFIGG( db.dir ),
                 DUF_CONFIGG( db.selected.name ) );
      DUF_CONFIGW( db.selected.fpath ) = mas_strdup( DUF_CONFIGG( db.dir ) );
      DUF_CONFIGW( db.selected.fpath ) = mas_strcat_x( DUF_CONFIGG( db.selected.fpath ), "/" );
      {
        char *n;

        n = strchr( DUF_CONFIGG( db.main.name ), '.' );
        if ( !n )
          n = DUF_CONFIGG( db.main.name ) + strlen( DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.selected.fpath ) =
              mas_strncat_x( DUF_CONFIGG( db.selected.fpath ), DUF_CONFIGG( db.main.name ), n - DUF_CONFIGG( db.main.name ) );
        DUF_CONFIGW( db.selected.fpath ) = mas_strcat_x( DUF_CONFIGG( db.selected.fpath ), "_$_" );
      }
      DUF_CONFIGW( db.selected.fpath ) = mas_strcat_x( DUF_CONFIGG( db.selected.fpath ), DUF_CONFIGG( db.selected.name ) );
      DUF_TRACE( explain, 0, "config->db.selected.fpath set: %s", DUF_CONFIGG( db.selected.fpath ) );
    }
#  endif

#endif

    if ( DUF_CONFIGG( db.main.fpath ) )
    {
      DUF_TRACE( any, 0, "dbfile: %s", DUF_CONFIGG( db.main.fpath ) );
#ifdef MAS_SPLIT_DB
      DUF_TRACE( any, 0, "adm dbfile: %s", DUF_CONFIGG( db.adm.fpath ) );
#ifndef DUF_SQL_TTABLES_TEMPORARY
      DUF_TRACE( any, 0, "tempo dbfile: %s", DUF_CONFIGG( db.tempo.fpath ) );
#endif
      /* DUF_TRACE( any, 0, "selected dbfile: %s", DUF_CONFIGG(db.selected.fpath) ); */
#endif
    }
    else
      DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
  }
  else if ( !DUF_CONFIGG( db.dir ) )
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.dir not set" );
  }
  else if ( !DUF_CONFIGG( db.main.name ) )
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
  if ( DUF_CONFIGG( db.main.fpath ) )
    DORF( r, duf_unlink, DUF_CONFIGG( db.main.fpath ) );
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
    DORF( r, duf_eval_sql_sequence, &sql_beginning_create, 0, NULL /* title */ , NULL /* selected.db */  );

  DEBUG_ENDR( r );
}

static int
duf_main_db_pre_action( void )
{
  DEBUG_STARTR( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_CLIG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      DORF( r, duf_eval_sql_sequence, &sql_beginning_clear, 0, NULL /* title */ , NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( vacuum ) )
  {
    /* static const char *sql = "VACUUM"; */

    /* DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) ); */
    if ( DUF_CLIG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
      DORF( r, duf_eval_sql_sequence, &sql_beginning_vacuum, 0, NULL /* title */ , NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }
/* --create-tables								*/ DEBUG_STEP(  );
  if ( DUF_NOERROR( r ) /* && DUF_ACTG_FLAG( create_tables ) */  )
  {
    DOR( r, duf_main_db_create_tables(  ) );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }
  DORF( r, duf_eval_sql_sequence, &sql_beginning_tables, 0, NULL /* title */ , NULL /* selected.db */  );

  DEBUG_ENDR( r );
}

static int
duf_main_db_tune( void )
{
  DEBUG_STARTR( r );
#ifdef MAS_SPLIT_DB
  if ( DUF_CONFIGG( db.adm.fpath ) )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBADMALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, DUF_CONFIGG( db.adm.fpath ) );
    DUF_TRACE( explain, 0, "attach adm database %s", DUF_CONFIGG( db.adm.fpath ) );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, DUF_CONFIGG(db.adm.fpath), r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }
#ifndef DUF_SQL_TTABLES_TEMPORARY
  if ( DUF_CONFIGG( db.tempo.fpath ) )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBTEMPALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, DUF_CONFIGG( db.tempo.fpath ) );
    DUF_TRACE( explain, 0, "attach tempo database %s -- %s", DUF_CONFIGG( db.tempo.fpath ), sql );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, DUF_CONFIGG(db.tempo.fpath), r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }
#endif
  /* DOR( r, duf_main_db_attach_selected( "dumplet" ) ); */

#  if 0
  if ( DUF_CONFIGG( db.selected.fpath ) )
  {
    static const char *sqlf = "ATTACH DATABASE '%s' AS " DUF_DBSELECTEDALIAS;
    char *sql;

    sql = duf_sql_mprintf( sqlf, DUF_CONFIGG( db.selected.fpath ) );
    DUF_TRACE( explain, 0, "attach selected database %s -- %s", DUF_CONFIGG( db.selected.fpath ), sql );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    /* DUF_SQL_BIND_S( dbfPath, DUF_CONFIGG(db.selected.fpath), r, pstmt ); */
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    sqlite3_free( sql );
  }
#  endif

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
  DOR( r, duf_eval_sql_sequence( &sql_beginning_common, 0, NULL /* title */ , NULL /* selected.db */  ) ); /* PRAGMAs etc. */
  DEBUG_ENDR( r );
}

int
duf_main_db_open( void )
{
  DEBUG_STARTR( r );

  /* global_status.db_opened_name */

  DUF_TRACE( db, 5, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
  DUF_TRACE( db, 5, "@@@@global_status.db_opened_name:%s => CFG->db.main.name:%s", global_status.db_opened_name, DUF_CONFIGG( db.main.name ) );

  /* DUF_TRACE( temp, 0, "db:%s : %s", DUF_CONFIGG(db.main.name), global_status.db_opened_name ); */
  if ( global_status.db_opened_name && 0 != strcmp( DUF_CONFIGG( db.main.name ), global_status.db_opened_name ) )
  {
    DUF_TRACE( db, 0, "@@@@autoclose db %s => %s", global_status.db_opened_name, DUF_CONFIGG( db.main.name ) );
    DUF_TRACE( db, 0, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
    DOR( r, duf_main_db_close( 0 ) );
    DUF_TRACE( db, 0, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
  }
  DUF_TRACE( db, 5, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
#if 0
  if ( !DUF_CONFIGG( db.opened ) )
#else
  if ( !global_status.db_opened_name )
#endif
  {
    assert( !global_status.db_attached_selected );
    DUF_TRACE( db, 0, "@@@@@to open db %s", DUF_CONFIGG( db.main.name ) );
    DORF( r, duf_main_db_locate );
    DORF( r, duf_main_db_optionally_remove_files );
    DORF( r, duf_sql_open, DUF_CONFIGG( db.main.fpath ) );
#if 0
    DUF_CONFIGW( db.opened ) = ( DUF_NOERROR( r ) );
#endif
    if ( DUF_NOERROR( r ) )
      global_status.db_opened_name = mas_strdup( DUF_CONFIGG( db.main.name ) );
    DORF( r, duf_main_db_tune );
    DORF( r, duf_main_db_pre_action );
    DOR( r, duf_pdi_reinit_min( DUF_CONFIGG( pdi ) ) );
    if ( r == 0 )
      r++;
  }

  DEBUG_ENDR( r );
}

int
duf_main_db_close( int ra )
{
  DEBUG_STARTR( r );
  r = ra;
#if 0
  if ( DUF_CONFIGG( db.opened ) )
#else
  if ( global_status.db_opened_name )
#endif
  {
    int rt = 0;

    DUF_TRACE( db, 0, "@@@@closing db %s", global_status.db_opened_name );
#if 0
    {
#  ifdef MAS_SPLIT_DB
      if ( DUF_CONFIGG( db.adm.fpath ) )
      {
        static const char *sql = "DETACH DATABASE 'adm'";

        DUF_TRACE( explain, 0, "detach adm database %s", DUF_CONFIGG( db.adm.fpath ) );
        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      if ( DUF_CONFIGG( db.tempo.fpath ) )
      {
        static const char *sql = "DETACH DATABASE 'tempo'";

        DUF_TRACE( explain, 0, "detach tempo database %s -- %s", DUF_CONFIGG( db.tempo.fpath ), sql );
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
#if 0
    DUF_CONFIGW( db.opened ) = !( DUF_NOERROR( r ) );
#else
    mas_free( global_status.db_opened_name );
    global_status.db_opened_name = NULL;
#endif
    mas_free( global_status.db_attached_selected );
    global_status.db_attached_selected = NULL;
  }
  DEBUG_ENDR( r );
}

static int DUF_UNUSED
duf_store_log( int argc, char *const argv[] )
{
  DEBUG_STARTR( r );
#if 0
  /* if ( argv ) */
  {
    char *sargv1, *sargv2;

#  ifdef MAS_TRACING
    int changes = 0;
#  else
    int DUF_UNUSED changes = 0;
#  endif
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
#endif
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

  /* I. duf_all_options -- STAGE_SETUP */
  DOR_NOE( r, duf_all_options( DUF_OPTION_STAGE_SETUP ), DUF_ERROR_OPTION_NOT_FOUND );
  DORF( r, duf_config_optionally_show ); /* FIXME similar to duf_show_options, called from duf_main_with_config after calling duf_main_db ??? FIXME */

  DUF_TEST_RX_START( r );
  /* > */ DUF_SHOW_ERROR( "db not opened @ %s ( %s )", DUF_CONFIGG( db.main.fpath ), duf_error_name( r ) );
  DUF_TEST_RX_END( r );

#if 0
  if ( DUF_CONFIGG( targ.argc ) > 0 )
    DORF( r, duf_store_log, DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ.argv ) );
#endif

#if 0
  /* to be removed as obsolete */
  DORF( r, DUF_WRAPPED( duf_action ) /* , argc, argv */  ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
#else
  /* call of duf_pdi_init -- after duf_all_options( DUF_OPTION_STAGE_SETUP ), before duf_all_options, DUF_OPTION_STAGE_(FIRST|INTERACTIVE) */
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( DUF_CONFIGG( pdi ), NULL /* real_path */ , 0 /* caninsert */ , NULL /* sql_set */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );

  DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( DUF_CONFIGG( pdi ) ) );

  /* II. duf_all_options -- (STAGE_FIRST + STAGE_LOOP)  or STAGE_INTERACTIVE */
  if ( DUF_ACTG_FLAG( interactive ) )
    DORF( r, duf_all_options, DUF_OPTION_STAGE_INTERACTIVE ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
  else
  {
    DORF( r, duf_all_options, DUF_OPTION_STAGE_FIRST ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
    for ( int ia = DUF_CONFIGG( targ_offset ); DUF_NOERROR( r ) && ia < DUF_CONFIGG( targ.argc ); ia++ )
    {
      DOR( r, duf_pdi_reinit_anypath( DUF_CONFIGG( pdi ), DUF_CONFIGG( targ.argv )[ia], 7 /* caninsert */ ,
                                      NULL /* node_selector2 */ , DUF_UG_FLAG( recursive ) ) );
      DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( DUF_CONFIGG( pdi ) ) );
      DORF( r, duf_all_options, DUF_OPTION_STAGE_LOOP ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
    }
  }
#endif
  DORF( r, duf_main_db_close, r ); /* [@] */
  DEBUG_ENDR( r );
}
