#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config.h"
#include "duf_status_ref.h"

#include "duf_sys_wrap.h"

#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_open.h"

#include "duf_options.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_reinit.h"

#include "duf_maindb_info.h"

#include "evsql_begfin.h"
#include "evsql_selector.h"

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

  DOR( r, duf_config_make_db_paths(  ) );

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
  if ( DUF_OPTG_FLAG( dry_run ) )
    DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( CREATE_TABLES ) );
  else
  {
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_one, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_two, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_three, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_four, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
  }
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
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_drop, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( clean_tables ) )
  {
    DUF_TRACE( explain, 0, "clean (zero) tables: option %s", DUF_OPT_FLAG_NAME( CLEAN_TABLES ) );
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( CLEAN_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_clean, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }

/* --create-tables								*/ DEBUG_STEP(  );
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( create_tables ) /* && DUF_ACTG_FLAG( create_database ) */  )
  {
    DOR( r, duf_main_db_create_tables(  ) );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( vacuum ) )
  {
    /* static const char *sql = "VACUUM"; */

    /* DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) ); */
    if ( DUF_OPTG_FLAG( dry_run ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_vacuum, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
    global_status.actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }


  DORF( r, duf_eval_sqlsq, &sql_beginning_tables, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  );

  DEBUG_ENDR( r );
}




#ifdef DUF_ATTACH_TTABLES_PATTERN
#  ifdef DUF_SQL_TTABLES_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#else
#  ifndef DUF_SQL_TTABLES_TEMPORARY
#    error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
#  endif
#endif




static int
duf_main_db_tune( void )
{
  DEBUG_STARTR( r );
#ifdef MAS_SPLIT_DB
  if ( DUF_CONFIGG( db.adm.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_COMMON_PATTERN "adm.db' AS " DUF_DBADMALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBADMALIAS, NULL /* &changes */  ) );
    DUF_TRACE( db, 0, "(%d) %s", r, sql );
  }

#  ifndef DUF_SQL_TTABLES_TEMPORARY
  if ( DUF_CONFIGG( db.tempo.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_TTABLES_PATTERN "temp.db' AS " DUF_DBTEMPALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBTEMPALIAS, NULL /* &changes */  ) );
    DUF_TRACE( db, 0, "(%d) %s", r, sql );
  }
#  endif


#endif

/* TODO : part to only after possible tables creation */
  DOR( r, duf_eval_sqlsq( &sql_beginning_common, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  ) ); /* PRAGMAs etc. */
  DEBUG_ENDR( r );
}

int
duf_main_db_open( void )
{
  DEBUG_STARTR( r );

  /* global_status.db_opened_name */

  DUF_TRACE( db, 5, "@@@@global_status.db_attached_selected:%s", global_status.db_attached_selected );
  DUF_TRACE( db, 5, "@@@@global_status.db_opened_name:%s => CFG->db.main.name:%s", global_status.db_opened_name, DUF_CONFIGGSP( db.main.name ) );

  /* DUF_TRACE( temp, 0, "db:%s : %s", DUF_CONFIGGS(db.main.name), global_status.db_opened_name ); */
  if ( global_status.db_opened_name && 0 != strcmp( DUF_CONFIGGSP( db.main.name ), global_status.db_opened_name ) )
  {
    DUF_TRACE( db, 0, "@@@@autoclose db %s => %s", global_status.db_opened_name, DUF_CONFIGGSP( db.main.name ) );
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
    DORF( r, duf_main_db_locate );
    DORF( r, duf_main_db_optionally_remove_files );

    DUF_TRACE( db, 0, "@@@@@to open db %s [%s]", DUF_CONFIGGSP( db.main.name ), DUF_CONFIGGS( db.main.fpath ) );
    if ( DUF_CONFIGGS( db.main.fpath ) )
    {
      if ( !DUF_ACTG_FLAG( create_database ) )
      {
        struct stat st;
        int ry = 0;

        ry = stat( DUF_CONFIGGS( db.main.fpath ), &st );
        if ( ry < 0 )
          DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, DUF_CONFIGGS( db.main.fpath ) );
      }
      DORF( r, duf_sql_open, DUF_CONFIGGS( db.main.fpath ) );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    }
#if 0
    DUF_CONFIGWN( db.opened, ( DUF_NOERROR( r ) ) );
#endif
    if ( DUF_NOERROR( r ) )
      global_status.db_opened_name = mas_strdup( DUF_CONFIGGSP( db.main.name ) );
    DORF( r, duf_main_db_tune );
    DORF( r, duf_main_db_pre_action );
    DUF_TRACE( pdi, 0, "DUF_CONFIGG( scn.pdi ) %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
    DOR( r, duf_pdi_reinit_min( DUF_CONFIGG( scn.pdi ) ) );
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
    DUF_CONFIGWN( db.opened, !( DUF_NOERROR( r ) ) );
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
duf_store_log( int argc DUF_UNUSED, char *const argv[]DUF_UNUSED )
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
duf_main_db( int argc DUF_UNUSED, char **argv DUF_UNUSED )
{
  DEBUG_STARTR( r );

  DUF_VERBOSE( 0, "verbose test 0> %d %s", 17, "hello" );
  DUF_VERBOSE( 1, "verbose test 1> %d %s", 17, "hello" );

  /* DUF_TRACE( temp, 0, "@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( cli.trace.temp ) ); */

  /* I. duf_all_options -- STAGE_PRESETUP */
  DUF_TRACE( options, 0, "@@@@I" );
  DOR_LOWERE( r, duf_all_options( DUF_OPTION_STAGE_PRESETUP ), DUF_ERROR_OPTION_NOT_FOUND );

  /* II. duf_all_options -- STAGE_SETUP */
  DUF_TRACE( options, 0, "@@@@II" );
  DOR_LOWERE( r, duf_all_options( DUF_OPTION_STAGE_SETUP ), DUF_ERROR_OPTION_NOT_FOUND );
  DORF( r, duf_config_optionally_show ); /* FIXME similar to duf_show_options, called from duf_main_with_config after calling duf_main_db ??? FIXME */

  DUF_TEST_RX_START( r );
  /* (* > *) DUF_SHOW_ERROR( "db not opened @ %s ( %s )", DUF_CONFIGG( db.main.fpath ), duf_error_name_i( r ) ); */
  DUF_TEST_RX_END( r );

#if 0
  if ( DUF_CONFIGG( cli.targ.argc ) > 0 )
    DORF( r, duf_store_log, DUF_CONFIGG( cli.targ.argc ), DUF_CONFIGG( cli.targ.argv ) );
#endif

#if 0
  /* to be removed as obsolete */
  DORF( r, DUF_WRAPPED( duf_action ) /* , argc, argv */  ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
#else
#  if 0
  /* call of duf_pdi_init -- after duf_all_options( DUF_OPTION_STAGE_SETUP ), before duf_all_options, DUF_OPTION_STAGE_(FIRST|INTERACTIVE) */
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( DUF_CONFIGG( scn.pdi ), DUF_CONFIGG( scn.pdi )->pu, NULL /* real_path */ , NULL /* sql_set */ , 0 /* caninsert */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#  else
  DOR( r, duf_pdi_init_at_config(  ) );
  /* assert( DUF_CONFIGX( scn.pdi )->pup == DUF_CONFIGX( scn.puz ) ); */

#  endif

  DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );

  /* III. duf_all_options -- (STAGE_FIRST + STAGE_LOOP)  or STAGE_INTERACTIVE */
  DUF_TRACE( options, 0, "@@@@III %s", duf_error_name_i( r ) );
  if ( DUF_ACTG_FLAG( interactive ) )
    DORF( r, duf_all_options, DUF_OPTION_STAGE_INTERACTIVE ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
  else
  {
    DORF( r, duf_all_options, DUF_OPTION_STAGE_FIRST ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
    for ( int ia = DUF_CONFIGG( cli.targ_offset ); DUF_NOERROR( r ) && ia < DUF_CONFIGG( cli.targ.argc ); ia++ )
    {
      DOR( r, duf_pdi_reinit_anypath( DUF_CONFIGG( scn.pdi ), DUF_CONFIGG( cli.targ.argv )[ia], ( duf_ufilter_t * ) NULL /* take pu from config */ ,
                                      NULL /* node_selector2 */ , 7 /* caninsert */ , DUF_UG_FLAG( recursive ) ) );
      DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
      DORF( r, duf_all_options, DUF_OPTION_STAGE_LOOP ); /* XXX XXX XXX XXX XXX XXX XXX XXX */
    }
  }
#endif
  if ( DUF_ACTG_FLAG( info ) )
    DOR( r, duf_main_db_info(  ) );

  DORF( r, duf_main_db_close, r ); /* [@] */
  DEBUG_ENDR( r );
}
