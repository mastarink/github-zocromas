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

#include "duf_utils.h"
#include "duf_sys_wrap.h"

#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_action.h"

#include "duf_maindb_info.h"

/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */


static int
duf_main_db_tune( void )
{
  DEBUG_STARTR( r );
#ifdef MAS_SPLIT_DB
  {
    static const char *sql = "ATTACH DATABASE :dbfPath AS adm";

    DUF_TRACE( explain, 0, "attach adm database" );
    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_BIND_S( dbfPath, duf_config->db.adm.fpath, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
  }
#endif
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
  DEBUG_ENDR( r );
}

static int
duf_main_db_opened( int argc, char **argv )
{
  DEBUG_STARTR( r );

  DORF( r, duf_main_db_tune );
  DORF( r, duf_action, argc, argv ); /* duf_action : duf_action.c */
  DORF( r, duf_main_db_info );
  if ( r < 0 && r != DUF_ERROR_MAX_REACHED )
  {
    DUF_TEST_RX( r );
    DUF_SHOW_ERROR( "action FAIL ; [%s] (#%d)", duf_error_name( r ), r );
  }

  DEBUG_ENDR( r );
}

static int
duf_main_db_locate( void )
{
  DEBUG_STARTR( r );

  if ( duf_config->db.dir && duf_config->db.main.name )
  {
    DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", duf_config->db.dir, duf_config->db.main.name );
    /* TODO move db.main.fpath and db.adm.fpath from duf_config to tmp etc. - it's not config values */
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
      duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, duf_config->db.adm.name );
      DUF_TRACE( explain, 0, "config->db.adm.fpath set: %s", duf_config->db.adm.fpath );
    }
#endif

    if ( duf_config->db.main.fpath )
    {
      DUF_TRACE( any, 0, "dbfile: %s", duf_config->db.main.fpath );
#ifdef MAS_SPLIT_DB
      DUF_TRACE( any, 0, "adm dbfile: %s", duf_config->db.adm.fpath );
#endif
    }
    else
      DOR( r, DUF_ERROR_PTR );
  }
  else if ( !duf_config->db.dir )
  {
    DOR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.dir not set" );
  }
  else if ( !duf_config->db.main.name )
  {
    DOR( r, DUF_ERROR_PTR );
    DUF_SHOW_ERROR( "db.main.name not set" );
  }
  else
    DOR( r, DUF_ERROR_UNKNOWN );
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
  if ( DUF_ACT_FLAG( remove_database ) )
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
duf_main_db_optionally_config_show( int argc, char **argv )
{
  DEBUG_STARTR( r );
  if ( duf_config->cli.dbg.verbose )
  {
    for ( int ia = 0; ia < argc; ia++ )
      DUF_TRACE( any, 0, "######### argv[%d]: %s", ia, argv[ia] );
    DOR( r, duf_config_show(  ) );
  }
  else
  {
    DUF_TRACE( explain, 0, "not showing config: not verbose" );
  }

  DEBUG_ENDR( r );
}

static int
duf_main_db_open( void )
{
  DEBUG_STARTR( r );

  DORF( r, duf_sql_open, duf_config->db.main.fpath );

  DEBUG_ENDR( r );
}

static int
duf_main_db_close( int ra )
{
  DEBUG_STARTR( r );
  r = ra;
  {
    int rt = 0;

    /* don't DOR it directly! call allways! */
    DORF( rt, duf_sql_close );

    if ( r == 0 && rt < 0 )
      DOR( r, rt );
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

  DORF( r, duf_main_db_locate );
  DORF( r, duf_main_db_optionally_config_show, argc, argv );
  DORF( r, duf_main_db_optionally_remove_files );
  DORF( r, duf_main_db_open );

  DUF_TEST_RX_START( r );
  DUF_SHOW_ERROR( "db not opened @ %s ( %s )", duf_config->db.main.fpath, duf_error_name( r ) );
  DUF_TEST_RX_END( r );

  DORF( r, duf_main_db_opened, argc, argv );
  DORF( r, duf_main_db_close, r );
  DEBUG_ENDR( r );
}
