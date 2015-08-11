#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>


#include <zlib.h>

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
static int dirent_contnt2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "crc32 SET dup32cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "crc32 AS c32 " /* */
          " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.crc32id=c32." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "crc32.crc32sum=c32.crc32sum )" /* */
          ,
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
          " numdirs=(SELECT COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths AS p " /* */
          " WHERE p.ParentId=" DUF_DBPREF "pathtot_dirs.Pathid )" /* */
          ,
#endif


          NULL,
          }
};


/* ########################################################################################## */

duf_scan_callbacks_t duf_collect_openat_crc32_callbacks = {
  .title = "collect crc32",
  .name = "crc32",
  .init_scan = NULL,
  .def_opendir = 1,
  .leaf_scan_fd2 = dirent_contnt2,

  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {.fieldset = "fn.Pathid AS dirid " /* */
           ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
           ", fn.name AS filename, fd.size AS filesize " /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           "  "                 /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", fd.mode AS filemode " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           "    WHERE "         /* */
           " fd.crc32id IS NULL AND" /* */
           " sz.size > 0 AND"
           /* " sz.dupzcnt > 1 AND "  (* *) */
           " fn.Pathid=:parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           "    WHERE "         /* */
           " fd.crc32id IS NULL AND" /* */
           /* " sz.dupzcnt > 1 AND "      (* *) */
           " sz.size > 0 AND "  /* */
           " 1 "},
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,
           .selector_total2 =   /* */
           " /* c3 */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static unsigned long long
duf_insert_crc32_uni( duf_depthinfo_t * pdi, unsigned long crc32sum, const char *filename, int need_id, int *pr )
{
  unsigned long long crc32id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( !duf_config->cli.disable.flag.insert )
  {
    static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "crc32 (crc32sum) VALUES (:crc32sum)";

    DUF_TRACE( crc32, 0, "%0lx %s%s", crc32sum, real_path, filename );
    DUF_SQL_START_STMT( pdi, insert_crc32, sql, lr, pstmt );
    DUF_TRACE( insert, 0, "S:%s", sql );
    DUF_SQL_BIND_LL( crc32sum, crc32sum, lr, pstmt );
    DUF_SQL_STEP( lr, pstmt );
    DUF_SQL_CHANGES( changes, lr, pstmt );
    DUF_SQL_END_STMT( insert_crc32, lr, pstmt );
  }
  duf_pdi_reg_changes( pdi, changes );
  if ( ( lr == DUF_SQL_CONSTRAINT || !lr ) && !changes )
  {
    if ( need_id )
    {
      duf_scan_callbacks_t sccb = {.leaf.fieldset = "crc32id" };
      duf_sccb_handle_t csccbh = {.sccb = &sccb };
      lr = duf_sql_select( duf_sel_cb_field_by_sccb, &crc32id, STR_CB_DEF, STR_CB_UDATA_DEF, &csccbh /*, ( const duf_dirhandle_t * ) NULL off */ ,
                           "SELECT " DUF_SQL_IDNAME " AS crc32id FROM " DUF_DBPREF "crc32 WHERE crc32sum='%lld'", crc32sum );
    }
  }
  else if ( !lr /* assume SQLITE_OK */  )
  {
    if ( need_id && changes )
    {
      crc32id = duf_sql_last_insert_rowid(  );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "insert crc32 %d", lr );
  }
  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( crc32id );
  return crc32id;
}

static unsigned long
duf_make_crc32_uni( int fd, int *pr )
{
  int lr = 0;
  size_t bufsz = 512 * 8;
  unsigned long crc32sum = 0;
  unsigned char *buffer;

  if ( !duf_config->cli.disable.flag.calculate )
    crc32sum = crc32( 0L, Z_NULL, 0 );
  /* if ( !duf_config->cli.disable.flag.calculate ) */
  {
    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 1;

      DUF_TEST_R( lr );
      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( lr >= 0 && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        DUF_TRACE( crc32, 0, "read %d : crc32sum:%lx", rr, crc32sum );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          DUF_MAKE_ERROR( lr, DUF_ERROR_READ );
        }
        DUF_TEST_R( lr );
        if ( rr > 0 && !duf_config->cli.disable.flag.calculate )
          crc32sum = crc32( crc32sum, buffer, rr );
        DUF_TRACE( crc32, 0, "rr:%d; lr:%d; crc32sum:%lx", rr, lr, crc32sum );
        if ( rr <= 0 )
          break;

        DUF_TEST_R( lr );
      }
      DUF_TRACE( crc32, 0, "last crc32sum:%lx", crc32sum );
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( lr, DUF_ERROR_MEMORY );
    }
  }
  if ( pr )
    *pr = lr;
  return crc32sum;
}

static int
dirent_contnt2( duf_sqlite_stmt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned long crc32sum = 0;



  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( crc32, 0, "+ %s", filename );
  if ( duf_config->cli.disable.flag.calculate )
    crc32sum = duf_levinfo_dirid( pdi );
  else
    crc32sum = duf_make_crc32_uni( duf_levinfo_dfd( pdi ), &r );
  DUF_TEST_R( r );

  if ( r >= 0 )
  {
    unsigned long long crc32id = crc32sum;

    crc32id = duf_insert_crc32_uni( pdi, crc32sum, filename, 1 /*need_id */ , &r );
    if ( r >= 0 && crc32id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;

      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET crc32id=%llu WHERE " DUF_SQL_IDNAME "=%lld", &changes, crc32id, filedataid );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( crc32, 0, "%04lx : crc32id: %llu (sz:%lu) \"%s\"", crc32sum, crc32id, duf_levinfo_stat( pdi )->st_size, filename );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( pdi ), crc32sum, crc32id ); */
  }
  DEBUG_ENDR( r );
}
