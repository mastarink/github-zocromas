/* #include <stdio.h> */
/* #include <stdlib.h> */
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"

#include "duf_types.h"

#include "duf_config_ref.h"
#include "duf_config.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_options.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

#include "duf_action.h"
#include "duf_maindb.h"


/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */


int
main_db( int argc, char **argv )
{
  int r = DUF_ERROR_MAIN;

  DUF_VERBOSE( 0, "verbose test 0> %d %s", 17, "hello" );
  DUF_VERBOSE( 1, "verbose test 1> %d %s", 17, "hello" );

/*										*/ DEBUG_START(  );
  if ( duf_config->db.dir && duf_config->db.main.name )
  {
    r = 0;
    DUF_TRACE( action, 0, "db.dir:%s; db.name:%s", duf_config->db.dir, duf_config->db.main.name );
    duf_config->db.main.fpath = mas_strdup( duf_config->db.dir );
    duf_config->db.main.fpath = mas_strcat_x( duf_config->db.main.fpath, "/" );
    duf_config->db.main.fpath = mas_strcat_x( duf_config->db.main.fpath, duf_config->db.main.name );
#ifdef MAS_SPLIT_DB
    if ( duf_config->db.adm.name )
    {
      duf_config->db.adm.fpath = mas_strdup( duf_config->db.dir );
      duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, "/" );
      duf_config->db.adm.fpath = mas_strcat_x( duf_config->db.adm.fpath, duf_config->db.adm.name );
    }
#endif

    /* if ( r >= 0 )                                      */
    /*   r = duf_cli_option_by_string( "trace-maction=2" ); */
    DUF_TRACE( current, 0, "r:%d", r );
    if ( r >= 0 && duf_config->cli.dbg.verbose )
    {
      for ( int ia = 0; ia < argc; ia++ )
        DUF_TRACE( any, 0, "######### argv[%d]: %s", ia, argv[ia] );
      r = duf_config_show(  );
    }
    DUF_TRACE( any, 0, "dbfile: %s", duf_config->db.main.fpath );
#ifdef MAS_SPLIT_DB
    DUF_TRACE( any, 0, "adm dbfile: %s", duf_config->db.adm.fpath );
#endif
    if ( r >= 0 )
    {
      if ( DUF_ACT_FLAG( remove_database ) )
      {
        if ( duf_config->db.main.fpath )
        {
          DUF_TRACE( any, 0, "removing %s ...", duf_config->db.main.fpath );
          r = unlink( duf_config->db.main.fpath );
          if ( r < 0 )
          {
            char *s;
            char serr[1024];

            s = strerror_r( errno, serr, sizeof( serr ) );

            DUF_ERROR( "unlink %s: [%s]", duf_config->db.main.fpath, s );
            if ( errno == ENOENT )
              r = 0;
            else
              r = DUF_ERROR_UNLINK;
          }
        }
      }
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
      if ( duf_config->db.main.fpath )
        r = duf_sql_open( duf_config->db.main.fpath );
      else
        r = DUF_ERROR_PTR;
      DUF_TEST_R( r );
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
#ifdef MAS_SPLIT_DB
      {
        static const char *sql = "ATTACH DATABASE :dbfpath AS adm";

        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_BIND_S( dbfpath, duf_config->db.adm.fpath, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
#endif
      {
        static const char *sql = "PRAGMA synchronous = OFF";

        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }

      /* r = duf_sql_exec( "PRAGMA synchronous = OFF", ( int * ) NULL ); */

      DUF_TEST_R( r );
      {
        static const char *sql = "PRAGMA encoding = 'UTF-8'";

        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      /* if ( r >= 0 )                                                           */
      /*        r = duf_sql_exec( "PRAGMA encoding = 'UTF-8'", ( int * ) NULL ); */
      DUF_TEST_R( r );
      if ( r >= 0 )
        r = duf_action_new( argc, argv );
      DUF_TEST_R( r );

      if ( r < 0 && r != DUF_ERROR_MAX_REACHED )
      {
        DUF_TEST_RX( r );
        DUF_ERROR( "action FAIL ; [%s] (#%d)", duf_error_name( r ), r );
      }
      /* duf_action( argc, argv ); */
      {
        int rc = duf_sql_close(  );

        if ( r == 0 )
          r = rc;
      }
    }
  }
  else if ( !duf_config->db.dir )
  {
    r = DUF_ERROR_PTR;
    DUF_ERROR( "db.dir not set" );
  }
  else if ( !duf_config->db.main.name )
  {
    r = DUF_ERROR_PTR;
    DUF_ERROR( "db.main.name not set" );
  }
/*										*/ DEBUG_END(  );
  DUF_TEST_R( r );
  return r;
}
