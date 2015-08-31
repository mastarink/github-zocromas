#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <openssl/md5.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql_field2.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */
static int sd5_dirent_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "sd5 SET dup2cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "sd5 AS sd " /* */
          " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.sd5id=sd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "sd5." DUF_SQL_IDNAME "=sd." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "sd5.sd5sum1=sd.sd5sum1 AND " DUF_DBPREF "sd5.sd5sum2=sd.sd5sum2)" (* *) */
          ,
          NULL}
};

/* ########################################################################################## */


duf_scan_callbacks_t duf_collect_openat_sd5_callbacks = {
  .title = "collect sd5",
  .name = "sd5",
  .init_scan = NULL,
  .def_opendir = 1,





  .leaf_scan_fd2 = sd5_dirent_content2,

  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {.fieldset =          /* */
           "'sd5-leaf' AS fieldset_id, " /* */
           " fn.Pathid AS dirid " /* */
           ", 0 as ndirs, 0 as nfiles" /* */
           " , fd." DUF_SQL_IDNAME " AS filedataid, fd.inode AS inode " /* */
           " , fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           " , md.dup5cnt AS nsame " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fn." DUF_SQL_IDNAME " AS nameid " /* */
           " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           ,
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.sd5id   IS NULL OR sd." DUF_SQL_IDNAME " IS NULL ) AND " /* */
           " sz.size > 0                                             AND " /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) AND " /* */
           " 1 "                /* */
           ,
           .count_aggregate = "distinct fd." DUF_SQL_IDNAME
#if 0
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           ,                    /* */
#endif
           }
  ,
  .node = {.fieldset =          /* */
           "'sd5-node' AS fieldset_id, " /* */
           " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           ,
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,                    /* */
           .filter = NULL       /* */
           ,
#if 0
           .selector_total2 =   /* */
           " /* sd5 */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static unsigned long long
duf_insert_sd5_uni( duf_depthinfo_t * pdi, unsigned long long *md64, const char *msg, int need_id, int *pr )
{
  unsigned long long sd5id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( md64 && md64[1] && md64[0] )
  {
    if ( !duf_config->cli.disable.flag.insert )
    {
      if ( 1 )
      {
        static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "sd5 ( sd5sum1, sd5sum2 ) VALUES ( :sd5sum1, :sd5sum2 )";

        DUF_TRACE( sd5, 0, "%016llx%016llx %s%s", md64[1], md64[0], real_path, msg );
        DUF_SQL_START_STMT( pdi, insert_sd5, sql, lr, pstmt );
        DUF_TRACE( insert, 0, "S:%s", sql );
        DUF_SQL_BIND_LL( sd5sum1, md64[1], lr, pstmt );
        DUF_SQL_BIND_LL( sd5sum2, md64[0], lr, pstmt );
        DUF_SQL_STEP( lr, pstmt );
        DUF_SQL_CHANGES( changes, lr, pstmt );
        DUF_SQL_END_STMT( insert_sd5, lr, pstmt );
      }
      else
      {
        lr = duf_sql( "INSERT OR IGNORE INTO " DUF_DBPREF "sd5 (sd5sum1,sd5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
      }
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == DUF_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.leaf.fieldset = "sd5id" };
        duf_sccb_handle_t csccbh = {.sccb = &sccb };
        lr = duf_sql_select( duf_sel_cb_field_by_sccb, &sd5id, STR_CB_DEF, STR_CB_UDATA_DEF,
                             &csccbh,
                             "SELECT " DUF_SQL_IDNAME " AS sd5id FROM " DUF_DBPREF "sd5 WHERE sd5sum1='%lld' AND sd5sum2='%lld'", md64[1], md64[0] );
      }
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        sd5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert sd5 %d", lr );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
    DUF_TEST_R( lr );
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( sd5id );
  return sd5id;
}

static int
duf_make_sd5_uni( int fd, unsigned char *pmd )
{
  DEBUG_STARTR( r );
  size_t bufsz = 256 * 1;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( sd5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !duf_config->cli.disable.flag.calculate && ( MD5_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
      DUF_TEST_R( r );
      {
        int maxcnt = 2;
        int cnt = 0;

        while ( r >= 0 && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int rr;

          DUF_TRACE( sd5, 10, "read fd:%u", fd );
          rr = read( fd, buffer, bufsz );
          DUF_TRACE( sd5, 10, "read rr:%u", rr );
          if ( rr < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( rr > 0 && !duf_config->cli.disable.flag.calculate )
          {
            if ( MD5_Update( &ctx, buffer, rr ) != 1 )
              DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
          }
          if ( rr <= 0 )
            break;
          DUF_TEST_R( r );
        }
      }
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
  }
  if ( !duf_config->cli.disable.flag.calculate && MD5_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
  DEBUG_ENDR( r );
}

/* 20150820.143755 */
static int
sd5_dirent_content2( duf_sqlite_stmt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned char amd5r[MD5_DIGEST_LENGTH];
  unsigned char amd5[MD5_DIGEST_LENGTH];


  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( sd5, 0, "+ %s", filename );

  memset( amd5, 0, sizeof( amd5 ) );
  DUF_TRACE( sd5, 0, "+ %s", filename );
  if ( !duf_config->cli.disable.flag.calculate )
    DOR( r, duf_make_sd5_uni( duf_levinfo_dfd( pdi ), amd5 ) );
  DUF_TRACE( sd5, 0, "+ %s", filename );
  DUF_TEST_R( r );
  /* reverse */
  for ( int i = 0; i < sizeof( amd5 ) / sizeof( amd5[0] ); i++ )
    amd5r[i] = amd5[sizeof( amd5 ) / sizeof( amd5[0] ) - i - 1];

  if ( r >= 0 )
  {
    unsigned long long sd5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &amd5r;
    DUF_TRACE( sd5, 0, "insert %s", filename );
    if ( duf_config->cli.disable.flag.calculate )
      pmd[0] = pmd[1] = duf_levinfo_dirid( pdi ) + 74; /* FIXME What is it? */
    sd5id = duf_insert_sd5_uni( pdi, pmd, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( sd5id )
    {
      int changes = 0;

      if ( !duf_config->cli.disable.flag.update )
      {
        DOR( r, duf_sql( "UPDATE " DUF_DBPREF "filedatas SET sd5id=%llu WHERE " DUF_SQL_IDNAME "=%lld", &changes, sd5id, filedataid ) );
        duf_pdi_reg_changes( pdi, changes );
      }
    }
    DUF_TRACE( sd5, 0, "%016llx%016llx : sd5id: %llu", pmd[1], pmd[0], sd5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sd5id ); */
  }
  DEBUG_ENDR( r );
}
