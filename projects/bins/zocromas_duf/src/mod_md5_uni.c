#define DUF_SQL_PDI_STMT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_sql_const.h" */
#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql_field2.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_md5_uni.h" */
/* ###################################################################### */

static unsigned long long
duf_insert_md5_uni( duf_depthinfo_t * pdi, unsigned long long *md64, const char *filename, size_t fsize, int need_id, int *pr )
{
  unsigned long long md5id = -1;
  int r = 0;
  int changes = 0;
  const char *real_path = duf_levinfo_path( pdi );

  DEBUG_START(  );
  if ( md64 && md64[1] && md64[0] )
  {
    if ( !duf_config->cli.disable.flag.insert )
    {
      if ( 1 )
      {
        static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "md5 (md5sum1,md5sum2) VALUES (:md5sum1,:md5sum2)";

        DUF_TRACE( md5, 0, "%016llx%016llx %s%s", md64[1], md64[0], real_path, filename );
        DUF_SQL_START_STMT( pdi, insert_md5, sql, r, pstmt );
        DUF_TRACE( insert, 0, "S:%s", sql );
        DUF_SQL_BIND_LL( md5sum1, md64[1], r, pstmt );
        DUF_SQL_BIND_LL( md5sum2, md64[0], r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( insert_md5, r, pstmt );
      }
      else
      {
        r = duf_sql( "INSERT OR IGNORE INTO " DUF_DBPREF "md5 (md5sum1,md5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
      }
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.leaf.fieldset = "md5id" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &md5id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb /*, ( const duf_dirhandle_t * ) NULL off */ ,
                            "SELECT " DUF_SQL_IDNAME " AS md5id FROM " DUF_DBPREF "md5 WHERE md5sum1='%lld' AND md5sum2='%lld'", md64[1], md64[0] );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        md5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert md5 %d", r );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
    DUF_TEST_R( r );
  }

  if ( pr )
    *pr = r;

  DEBUG_ENDULL( md5id );
  return md5id;
}

static int
duf_make_md5_uni( int fd, unsigned char *pmd )
{
  int r = 0;
  size_t bufsz = 1024 * 1024 * 100;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( md5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( MD5_Init( &ctx ) != 1 )
        r = DUF_ERROR_MD5;
      DUF_TEST_R( r );
      while ( r >= 0 )
      {
        int rr;

        DUF_TRACE( md5, 10, "read fd:%u", fd );
        rr = read( fd, buffer, bufsz );
        DUF_TRACE( md5, 10, "read rr:%u", rr );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );

          r = DUF_ERROR_READ;
          DUF_TEST_R( r );
          break;
        }
        if ( rr > 0 && !duf_config->cli.disable.flag.calculate )
        {
          if ( MD5_Update( &ctx, buffer, rr ) != 1 )
            r = DUF_ERROR_MD5;
        }
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
  if ( MD5_Final( pmd, &ctx ) != 1 )
    r = DUF_ERROR_MD5;
  return r;
}

/* static int                                                                                                                        */
/* duf_scan_dirent_md5_content( int fd, const struct stat *pst_file, duf_depthinfo_t * pdi, duf_record_t * precord )                     */
/* {                                                                                                                                 */
/*   int r = 0;                                                                                                                      */
/*   unsigned char mdr[MD5_DIGEST_LENGTH];                                                                                           */
/*   unsigned char md[MD5_DIGEST_LENGTH];                                                                                            */
/*                                                                                                                                   */
/*   DUF_UFIELD( filedataid );                                                                                                       */
/*   DUF_SFIELD( filename );                                                                                                         */
/*                                                                                                                                   */
/*   memset( md, 0, sizeof( md ) );                                                                                                  */
/*   r = duf_make_md5_uni( fd, md );                                                                                                 */
/*   DUF_TEST_R( r );                                                                                                                */
/*   (* reverse *)                                                                                                                   */
/*   for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )                                                                      */
/*     mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];                                                                          */
/*                                                                                                                                   */
/*   if ( r >= 0 )                                                                                                                   */
/*   {                                                                                                                               */
/*     unsigned long long md5id = 0;                                                                                                 */
/*     unsigned long long *pmd;                                                                                                      */
/*                                                                                                                                   */
/*     pmd = ( unsigned long long * ) &mdr;                                                                                          */
/*     md5id = duf_insert_md5_uni( pdi, pmd, filename, pst_file->st_size, 1 (*need_id *) , &r );                                     */
/*     if ( r >= 0 && md5id )                                                                                                        */
/*     {                                                                                                                             */
/*       int changes = 0;                                                                                                            */
/*                                                                                                                                   */
/*       if ( r >= 0 && !duf_config->cli.disable.flag.update )                                                                            */
/*         r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET md5id=%llu WHERE " DUF_SQL_IDNAME "=%lld", &changes, md5id, filedataid );                */
/*       duf_pdi_reg_changes( pdi, changes );                                                                                        */
/*       DUF_TEST_R( r );                                                                                                            */
/*     }                                                                                                                             */
/*     DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );                                                   */
/*     DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id ); */
/*   }                                                                                                                               */
/*   return r;                                                                                                                       */
/* }                                                                                                                                 */

static int
duf_dirent_md5_contnt2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  unsigned char mdr[MD5_DIGEST_LENGTH];
  unsigned char md[MD5_DIGEST_LENGTH];

  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( md5, 0, "+ %s", filename );

  memset( md, 0, sizeof( md ) );
  DUF_TRACE( md5, 0, "+ %s", filename );
  r = duf_make_md5_uni( fd, md );
  DUF_TRACE( md5, 0, "+ %s", filename );
  DUF_TEST_R( r );
  /* reverse */
  for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
    mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];

  if ( r >= 0 )
  {
    unsigned long long md5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &mdr;
    DUF_TRACE( md5, 0, "insert %s", filename );
    md5id = duf_insert_md5_uni( pdi, pmd, filename, pst_file->st_size, 1 /*need_id */ , &r );
    if ( r >= 0 && md5id )
    {
      int changes = 0;

      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET md5id='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, md5id, filedataid );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id ); */
  }
  return r;
}

/* static int                                                                                                                          */
/* duf_scan_dirent_content_by_precord( duf_depthinfo_t * pdi, duf_record_t * precord, const char *fname, unsigned long long filesize ) */
/* {                                                                                                                                   */
/*   int r = 0;                                                                                                                        */
/*   int ffd = duf_levinfo_dfd( pdi );                                                                                                 */
/*                                                                                                                                     */
/*   DUF_SHOW_ERROR( "ffd:%d for " DUF_DEPTH_PFMT "", ffd, duf_pdi_depth( pdi ) );                                                          */
/*   if ( ffd )                                                                                                                        */
/*   {                                                                                                                                 */
/*     DUF_TRACE( md5, 2, "openat ffd:%d", ffd );                                                                                      */
/*     if ( ffd > 0 )                                                                                                                  */
/*     {                                                                                                                               */
/*       r = duf_scan_dirent_md5_content( ffd, duf_levinfo_stat( pdi ), pdi, precord );                                                */
/*     }                                                                                                                               */
/*     else                                                                                                                            */
/*     {                                                                                                                               */
/*       DUF_ERRSYS( "open to read file : %s", fname );                                                                                */
/*       r = ffd;                                                                                                                      */
/*       r = DUF_ERROR_OPEN;                                                                                                           */
/*     }                                                                                                                               */
/*     DUF_TEST_R( r );                                                                                                                */
/*     DUF_TEST_R( r );                                                                                                                */
/*   }                                                                                                                                 */
/*   else                                                                                                                              */
/*     r = DUF_ERROR_DATA;                                                                                                             */
/*   DUF_TEST_R( r );                                                                                                                  */
/*   return r;                                                                                                                         */
/* }                                                                                                                                   */

/* callback of type duf_scan_hook_file_t */
/* static int                                                                    */
/* collect_openat_md5_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord ) */
/* {                                                                             */
/*   int r = 0;                                                                  */
/*                                                                               */
/*   DUF_SFIELD( filename );                                                     */
/*   DUF_UFIELD( filesize );                                                     */
/*   DEBUG_START(  );                                                            */
/*   r = duf_scan_dirent_content_by_precord( pdi, precord, filename, filesize ); */
/*   DEBUG_ENDR( r );                                                            */
/*   return r;                                                                   */
/* }                                                                             */

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* static int                                                                                                             */
/* collect_openat_md5_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord ) */
/* {                                                                                                                      */
/*   int r = 0;                                                                                                           */
/*   const char *real_path = NULL;                                                                                        */
/*                                                                                                                        */
/*   DEBUG_START(  );                                                                                                     */
/*                                                                                                                        */
/*   real_path = duf_levinfo_path( pdi );                                                                                 */
/*   DUF_TRACE( md5, 0, "L%d; " DUF_SQL_IDNAME "%-7llu  real_path=%s;", duf_pdi_depth( pdi ), pathid_unused, real_path );                 */
/*                                                                                                                        */
/*   DEBUG_ENDR( r );                                                                                                     */
/*   return r;                                                                                                            */
/* }                                                                                                                      */

/* currently used for --same-md5  ??? */




static const char *final_sql[] = {
  "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
        " FROM " DUF_DBPREF "md5 AS md " /* */
        " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "md5." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
        /* " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)" (* *) */
        ,
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM " DUF_DBPREF "paths " /* */
        " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " /* */
        " GROUP BY parents." DUF_SQL_IDNAME "" /* */
        ,
  "DELETE FROM path_pairs"      /* */
        ,
  "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT count(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
        " FROM filenames AS fna" /* */
        "   JOIN filedatas AS fda ON (fna.dataid=fda.rowid)" /* */
        "   JOIN md5 AS mda ON (fda.md5id=mda.rowid)" /* */
        "   JOIN filedatas AS fdb ON (fdb.md5id=mda.rowid)" /* */
        "   JOIN filenames AS fnb ON (fdb.rowid=fnb.dataid)" /* */
        " WHERE Pathid1 < Pathid2 AND fna.name=fnb.name" /* */
        " GROUP BY Pathid1, Pathid2" /* */
        ,


  /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                      */
  /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                 */
  /*       " FROM " DUF_DBPREF "paths AS p " (* *)                       */
  /*       " WHERE p.ParentId=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *) */
  /*       ,                                                             */
  /* "DELETE FROM " DUF_DBPREF "keydata", */
  /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
  /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
  /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
  /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
  /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
  /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")" (* *)                            */
  /*       ,                                                                                    */


  NULL,
};



duf_scan_callbacks_t duf_collect_openat_md5_callbacks = {
  .title = "collect md5",
  .init_scan = NULL,
  .def_opendir = 1,
  /* .dirent_dir_scan_before = NULL, */
  /* .dirent_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_md5_scan_node_before, */
  /*  .leaf_scan =  collect_openat_md5_scan_leaf, */
  /* .leaf_scan_fd = duf_scan_dirent_md5_content, */
  .leaf_scan_fd2 = duf_dirent_md5_contnt2,
  .leaf = {.fieldset = "fn.Pathid AS dirid " /* */
           " , fd." DUF_SQL_IDNAME " AS filedataid, fd.inode AS inode " /* */
           " , fn.name AS filename, fd.size AS filesize " /* */
           " , uid, gid, nlink, inode, strftime('%s',mtim) AS mtime, md.dup5cnt AS nsame " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           /* .selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " (* *)       */
           /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " (* *) */
           /*       " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" (* *)         */
           /*       " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" (* *)      */
           /*       "    WHERE "            (* *)                                          */
           /*       " fd.md5id IS NULL AND" (* *)                                          */
           /*       " sz.size > 0 AND"                                                     */
           /*       (* " sz.dupzcnt > 1 AND "  (* *) *)                                    */
           /*       " fn.Pathid='%llu' "    (* *)                                          */
           /*       ,                                                                      */
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           "    WHERE "         /* */
           " fd.md5id IS NULL AND" /* */
           " sz.size > 0 AND"
           /* "       sz.dupzcnt > 1 AND "  (* *) */
           /* "                sd.dup2cnt > 1 AND " (* *) */
           " fn.Pathid=:dirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           " WHERE "            /* */
           " fd.md5id IS NULL AND" /* */
           /* "        sz.dupzcnt > 1 AND "      (* *) */
           /* "                sd.dup2cnt > 1 AND " (* *) */
           " sz.size > 0 "},
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           /* .selector = "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " (* *)       */
           /*       ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " (* *) */
           /*       " FROM " DUF_DBPREF "paths AS pt " (* *)                                                            */
           /*       " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                           */
           /*       " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                          */
           /*       " WHERE pt.ParentId='%llu' " (* *)                                                                  */
           /*       ,                                                                                                   */
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " WHERE pt.ParentId=:dirID" /* */
           },
  .final_sql_argv = final_sql,
};
