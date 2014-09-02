/* File #3 20140902.124011 */
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
#include "duf_service.h"

#include "duf_options.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

#include "duf_action.h"


/* ###################################################################### */
#include "duf_maindb.h"
/* ###################################################################### */

static unsigned long *
duf_info_from_db( int count, const char *sql )
{
  unsigned long *tuple = NULL;

  if ( count > 0 )
  {
    int r = 0;
    duf_sqlite_stmt_t *pstmt = NULL;

    /* DUF_TRACE( explain, 0, "SQL:'%s'", sql ); */
    if ( r >= 0 )
      r = duf_sql_prepare( sql, &pstmt );
    DUF_TEST_R( r );
    if ( r >= 0 )
      r = duf_sql_step( pstmt );
    DUF_TEST_RR( r );
    if ( r == DUF_SQL_ROW )
    {
      tuple = mas_malloc( count * sizeof( unsigned long ) );

      for ( int iv = 0; iv < count; iv++ )
      {
        long long val;

        val = duf_sql_column_long_long( pstmt, iv );
        /* DUF_TRACE( explain, 0, "%llu", val ); */
        tuple[iv] = val;
      }
    }
  }

  return tuple;
}

/* last function revision 20140902.124219   */
int
main_db( int argc, char **argv )
{
  int r = DUF_ERROR_MAIN;

  DUF_VERBOSE( 0, "verbose test 0> %d %s", 17, "hello" );
  DUF_VERBOSE( 1, "verbose test 1> %d %s", 17, "hello" );

/*										*/ DEBUG_START(  );
  if ( duf_config->db.dir && duf_config->db.main.name )
  {
    DUF_TRACE( explain, 0, "setting config->db.main.fpath by db.dir: %s and db.main.name: %s", duf_config->db.dir, duf_config->db.main.name );
    r = 0;
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




    /* if ( r >= 0 )                                      */
    /*   r = duf_cli_option_by_string( "trace-maction=2" ); */
    DUF_TRACE( current, 0, "r:%d", r );
    if ( r >= 0 && duf_config->cli.dbg.verbose )
    {
      for ( int ia = 0; ia < argc; ia++ )
        DUF_TRACE( any, 0, "######### argv[%d]: %s", ia, argv[ia] );
      r = duf_config_show(  );
    }
    else
    {
      DUF_TRACE( explain, 0, "not showing config: not verbose" );
    }
    DUF_TRACE( any, 0, "dbfile: %s", duf_config->db.main.fpath );
#ifdef MAS_SPLIT_DB
    DUF_TRACE( any, 0, "adm dbfile: %s", duf_config->db.adm.fpath );
#endif
    if ( r >= 0 )
    {
      if ( DUF_ACT_FLAG( remove_database ) )
      {
        DUF_TRACE( explain, 0, "     option %s, removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
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
      else
      {
        DUF_TRACE( explain, 1, "no %s option, not removing database", DUF_OPT_FLAG_NAME( REMOVE_DATABASE ) );
      }
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
      DUF_TRACE( explain, 0, "open database if fpath set; fpath:%s", duf_config->db.main.fpath );
      if ( duf_config->db.main.fpath )
        r = duf_sql_open( duf_config->db.main.fpath );
      else
        r = DUF_ERROR_PTR;
      DUF_TEST_R( r );
      DUF_TRACE( explain, 0, "opened (?%d) database", r );
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
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

      /* r = duf_sql_exec( "PRAGMA synchronous = OFF", ( int * ) NULL ); */

      DUF_TEST_R( r );
      {
        static const char *sql = "PRAGMA encoding = 'UTF-8'";

        DUF_TRACE( explain, 0, "PRAGMA encoding = 'UTF-8'" );

        DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
      /* if ( r >= 0 )                                                           */
      /*        r = duf_sql_exec( "PRAGMA encoding = 'UTF-8'", ( int * ) NULL ); */
      DUF_TEST_R( r );
      DUF_TRACE( explain, 0, "to do actions" );
      DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
      if ( r >= 0 )
        r = duf_action( argc, argv );
      if ( duf_config->cli.act.v.flag.info )
      {
        duf_infodata_t infod[] = {
          {.title = "paths",.count = 5,.labels = {"num of paths", "max num dirs", "max num files", "@min inow", "@max inow", NULL}
           ,.sql =
           "SELECT  count(*), max(numdirs), max(numfiles), strftime('%s',min(pt.inow)), strftime('%s',max(pt.inow)) FROM paths as pt LEFT JOIN pathtot_dirs AS td ON (td.Pathid=pt."
           DUF_SQL_IDNAME ") LEFT JOIN pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ")"}
          ,
          {.title = "datas with reasonable date",.count = 5,.labels = {"#", "@min mtim", "@max mtim", "@min inow", "@max inow", NULL}
           ,.sql =
           "SELECT count(*), strftime('%s',min(mtim)), strftime('%s',max(mtim)), strftime('%s',min(inow)), strftime('%s',max(inow)) FROM filedatas where cast(strftime('%s',mtim) as integer)>320000000 and cast(strftime('%s',mtim) as integer)<1600000000"}
          ,
          {.title = "zero files",.count = 1,.labels = {"#", NULL}
           ,.sql =
           "SELECT count(*) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME
           ") JOIN sizes as sz ON (sz.size=fd.size) where fd.size=0"}
          ,
          {.title = "nonzero files",.count = 1,.labels = {"#", NULL}
           ,.sql =
           "SELECT count(*) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME
           ") JOIN sizes as sz ON (sz.size=fd.size) where fd.size>0"}
          ,
          {.title = "names",.count = 3,.labels = {"#", "@min inow", "@max inow", NULL}
           ,.sql = "SELECT count(*), strftime('%s',min(inow)), strftime('%s',max(inow)) FROM filenames"}
          ,
          {.title = "names 2",.count = 1,.labels = {"#", NULL}
           ,.sql =
           "SELECT count(*) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN md5 AS md ON (md." DUF_SQL_IDNAME
           "=fd.md5id)"}
          ,
          {.title = "distinct md5id",.count = 1,.labels = {"#", NULL}
           ,.sql =
           "SELECT count(distinct md5id) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN md5 AS md ON (md."
           DUF_SQL_IDNAME "=fd.md5id)"}
          ,
          {.title = "n/z sizes",.count = 5,.labels = {"#", "min size", "max size", "@min inow", "@max inow", NULL}
           ,.sql = "SELECT count(*), min(size), max(size), strftime('%s',min(inow)), strftime('%s',max(inow)) FROM sizes where size>0"}
          ,
          {.title = "z sizes",.count = 3,.labels = {"#", "@min inow", "@max inow", NULL}
           ,.sql = "SELECT count(*), strftime('%s',min(inow)), strftime('%s',max(inow)) FROM sizes where size=0"}
          ,
          {.title = "dup sets by size",.count = 1,.labels = {"#", NULL}
           ,.sql = "SELECT count(*) FROM sizes where dupzcnt>1"}
          ,
          {.title = "md5's",.count = 1,.labels = {"#", "@min inow", "@max inow", NULL}
           ,.sql = "SELECT count(*), strftime('%s',min(md.inow)), strftime('%s',max(md.inow)) FROM md5 AS md"}
          ,
          {.title = "dup sets by md5's",.count = 1,.labels = {"#", NULL}
           ,.sql = "SELECT count(*) FROM md5 AS md where dup5cnt>1"}
          ,
          {.title = "mime's",.count = 3,.labels = {"#", "@min inow", "@max inow", NULL}
           ,.sql = "SELECT count(*), strftime('%s',min(mi.inow)), strftime('%s',max(mi.inow)) FROM mime AS mi"}
          ,
          {.title = "dup sets by mime's",.count = 1,.labels = {"#", NULL}
           ,.sql = "SELECT count(*) FROM mime AS mi where dupmimecnt>1"}
          ,
          {.title = "exif's",.count = 3,.labels = {"#", "@min date_time", "@max date_time", "@min inow", "@max inow", NULL}
           ,.sql =
           "SELECT count(*), strftime('%s',min(date_time)), strftime('%s',max(date_time)), strftime('%s',min(inow)), strftime('%s',max(inow)) FROM exif WHERE  cast(strftime('%s',date_time) as integer)>0"}
          ,
        };
        DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
        DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-    =-=-=-=-=-=-=-=-=-" );
        DUF_TRACE( explain, 0, "-=-=-=-=-=-                 -=-=-=-=-=-" );
        DUF_TRACE( explain, 0, "-=-=-=                         =-==--=-=-=-" );
        DUF_TRACE( explain, 0, "info                              info info info" );
        DUF_TRACE( explain, 0, "-=-=-=                      -=-=-=-=-=-" );
        DUF_TRACE( explain, 0, "-=-=-=-=-=-=-       -=-=-=-=-=-=-=-=-=-" );
        DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
        /*
           SELECT  count(*), min(pt.inow), max(pt.inow), max(numdirs), max(numfiles) FROM paths as pt LEFT JOIN pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") LEFT JOIN pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") ;
           SELECT count(*), datetime(min(mtim), 'unixepoch'), datetime(max(mtim), 'unixepoch'), min(inow), max(inow) FROM filedatas where mtim>320000000 and mtim<1600000000
           SELECT count(*) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN sizes as sz ON (sz.size=fd.size) where fd.size>0;
           SELECT count(*) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN sizes as sz ON (sz.size=fd.size) where fd.size=0;
           SELECT count(*), min(inow), max(inow)                                                                     FROM filenames;
           SELECT count(*), max(size), min(size), min(inow), max(inow)                                               FROM sizes; 
           SELECT count(*) FROM sizes where dupzcnt>1;
           SELECT count(*) FROM md5 AS md;   
           SELECT count(*)              FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id);
           SELECT count(distinct md5id) FROM filenames AS fn JOIN filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") JOIN md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id);
         */
        for ( int iop = 0; iop < sizeof( infod ) / sizeof( infod[0] ); iop++ )
        {
          unsigned long *tuple;
          int nolab = 0;

          /* DUF_TRACE( explain, 0, "#%d. SQL:'%s'", iop, infod[iop].sql ); */
          tuple = duf_info_from_db( infod[iop].count, infod[iop].sql );
          DUF_PRINTF( 0, ".#%2d. [ %30s ] -- ", iop, infod[iop].title );
          for ( int iv = 0; iv < infod[iop].count; iv++ )
          {
            const char *label;

            if ( !nolab && !infod[iop].labels[iv] )
            {
              nolab = 1;
              label = NULL;
            }
            else
            {
              label = infod[iop].labels[iv];
            }
            if ( label )
            {
              time_t tv;

              tv = ( time_t ) tuple[iv];
              if ( *label == '@' )
              {
                label++;
                char buf[64];

                duf_strfgmtime( buf, sizeof( buf ), "%F %T", tv );
                /* DUF_PRINTF( 0, ". %7s: %6lu:%15s;  ", label, tuple[iv], buf ); */
                DUF_PRINTF( 0, ". %s: %15s;  ", label, buf );
              }
              else
              {
                DUF_PRINTF( 0, ". %s: %6lu;  ", label, tuple[iv] );
              }
            }
            else
              DUF_PRINTF( 0, ". %6lu;  ", tuple[iv] );
          }
          DUF_PRINTF( 0, ";" );
          mas_free( tuple );
        }
        DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
      }
      DUF_TEST_R( r );
      DUF_TRACE( explain, 0, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" );
      DUF_TRACE( explain, 0, "after actions" );

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
