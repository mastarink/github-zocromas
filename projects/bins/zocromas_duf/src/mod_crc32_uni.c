#define DUF_SQL_PDI_STMT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>


#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_path.h" */

#include "duf_sql_const.h"
#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql_field2.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

DUF_UNUSED static unsigned long long
duf_insert_crc32_uni( duf_depthinfo_t * pdi, unsigned long crc32sum, const char *filename, size_t fsize, int need_id, int *pr )
{
  unsigned long long crc32id = -1;
  int r = 0;
  int changes = 0;
  const char *real_path = duf_levinfo_path( pdi );

  DEBUG_START(  );
  if ( !duf_config->cli.disable.flag.insert )
  {
    static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "crc32 (crc32sum) VALUES (:crc32sum)";

    DUF_TRACE( crc32, 0, "%0lx %s%s", crc32sum, real_path, filename );
    DUF_SQL_START_STMT( pdi, insert_crc32, sql, r, pstmt );
    DUF_TRACE( insert, 0, "S:%s", sql );
    DUF_SQL_BIND_LL( crc32sum, crc32sum, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( r, pstmt );
  }
  duf_pdi_reg_changes( pdi, changes );
  if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
  {
    if ( need_id )
    {
      duf_scan_callbacks_t sccb = {.leaf_fieldset = "crc32id" };
      r = duf_sql_select( duf_sel_cb_field_by_sccb, &crc32id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                          &sccb /*, ( const duf_dirhandle_t * ) NULL off */ ,
                          "SELECT id AS crc32id FROM " DUF_DBPREF "crc32 WHERE crc32sum='%lld'", crc32sum );
    }
  }
  else if ( !r /* assume SQLITE_OK */  )
  {
    if ( need_id && changes )
    {
      crc32id = duf_sql_last_insert_rowid(  );
    }
  }
  else
  {
    DUF_ERROR( "insert crc32 %d", r );
  }
  if ( pr )
    *pr = r;

  DEBUG_ENDULL( crc32id );
  return crc32id;
}

static unsigned long
duf_make_crc32_uni( int fd, int *pr )
{
  int r = 0;
  size_t bufsz = 128 * 1;
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
      int maxcnt = 2;

      DUF_TEST_R( r );
      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( r >= 0 && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        DUF_TRACE( crc32, 0, "read %d", rr );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          r = DUF_ERROR_READ;
        }
        DUF_TEST_R( r );
        if ( rr > 0 && !duf_config->cli.disable.flag.calculate )
          crc32sum = crc32( crc32sum, buffer, rr );
        if ( rr <= 0 )
          break;

        DUF_TEST_R( r );
      }
      mas_free( buffer );
    }
    else
    {
      r = DUF_ERROR_MEMORY;
    }
  }
  if ( pr )
    *pr = r;
  return crc32sum;
}

static int
duf_scan_dirent_crc32_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  unsigned long crc32sum = 0;

  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( crc32, 0, "+ %s", filename );
  if ( duf_config->cli.disable.flag.calculate )
    crc32sum = duf_levinfo_dirid( pdi );
  else
    crc32sum = duf_make_crc32_uni( fd, &r );
  DUF_TEST_R( r );

  if ( r >= 0 )
  {
    unsigned long long crc32id = crc32sum;

    /* crc32id = duf_insert_crc32_uni( pdi, crc32sum, filename, pst_file->st_size, 1 (*need_id *) , &r ); */
    if ( r >= 0 && crc32id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;

      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET crc32id=%llu WHERE id=%lld", &changes, crc32id, filedataid );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( crc32, 0, "%04lx : crc32id: %llu", crc32sum, crc32id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( pdi ), crc32sum, crc32id ); */
  }
  return r;
}

static const char *final_sql[] = {
  "UPDATE " DUF_DBPREF "crc32 SET dup32cnt=(SELECT COUNT(*) " /* */
        " FROM " DUF_DBPREF "crc32 AS c32 " /* */
        " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.md5id=c32.id) " /* */
        " WHERE " DUF_DBPREF "crc32.crc32sum=c32.crc32sum )" /* */
        ,
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents.id=paths.parentid) " /* */
        " GROUP BY parents.id"  /* */
        ,
  "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths AS p " /* */
        " WHERE p.ParentId=" DUF_DBPREF "pathtot_dirs.Pathid )" /* */
        ,


  NULL,
};



duf_scan_callbacks_t duf_collect_openat_crc32_callbacks = {
  .title = "collect crc32",
  .init_scan = NULL,
  .def_opendir = 1,
  .scan_mode_2 = 1,
  .leaf_scan_fd2 = duf_scan_dirent_crc32_content2,
  .leaf_fieldset = "fn.Pathid AS dirid " /* */
        " , fd.id AS filedataid, fd.inode AS inode " /* */
        " , fn.name AS filename, fd.size AS filesize " /* */
        " , fn.id AS filenameid " /* */
        " , fd.mode AS filemode " /* */
        ,
  .leaf_selector2 =             /* */
        /* "SELECT %s " */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
        "    WHERE "            /* */
        " fd.crc32id IS NULL AND" /* */
	" sz.size > 0 AND"
        /* " sz.dupzcnt > 1 AND "  (* *) */
        " fn.Pathid=:dirid "    /* */
        ,
  .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
        "    WHERE "            /* */
        " fd.crc32id IS NULL AND" /* */
	" sz.size > 0 "
        /* " AND sz.dupzcnt > 1 "      (* *) */
        ,
  .node_fieldset = "pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ,
  .node_selector2 =             /* */
        /* "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
        /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.ParentId=:dirid" /* */
        ,
  .final_sql_argv = final_sql,
};