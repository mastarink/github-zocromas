
#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_utils.h"


#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"


/* ###################################################################### */
#include "duf_maindb_info.h"
/* ###################################################################### */


#if 0
static unsigned long *
duf_info_from_db( int count, const char *sql )
{
  unsigned long *tuple = NULL;

  if ( count > 0 )
  {
    int r = 0;
    duf_sqlite_stmt_t *pstmt = NULL;

    /* DUF_TRACE( explain, 0, "SQL:'%s'", sql ); */
    DOR( r, duf_sql_prepare( sql, &pstmt ) );
    DOR_NOE( r, duf_sql_step( pstmt ), DUF_SQL_ROW, DUF_SQL_DONE );
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
int
duf_main_db_info( void )
{
  DEBUG_STARTR( r );
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

            duf_strfgmtime( buf, sizeof( buf ), "%F %T", &tv );
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
  DEBUG_ENDR( r );
}
#endif
