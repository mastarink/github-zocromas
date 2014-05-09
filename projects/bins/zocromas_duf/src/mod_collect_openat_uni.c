#define DUF_SQL_PDI_STMT

#include <assert.h>




#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>



#include "duf_types.h"

#include "duf_utils.h"
#include "duf_dbg.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_field.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_filedata.h"

#include "duf_path.h"



static int
collect_openat_insert_filename_uni( duf_depthinfo_t * pdi, const char *fname, unsigned long long dir_id, unsigned long long dataid )
{
  int r = 0;

  DEBUG_START(  );

  if ( fname && dir_id )
  {
    int r = 0;
    int changes = 0;

    const char *sql = "INSERT OR IGNORE INTO duf_filenames (Pathid, name, dataid) VALUES (:pathid, :name, :dataid)";

    DUF_SQL_START_STMT( pdi, insert_filename, sql, r, pstmt );
    DUF_SQL_BIND_LL( pathid, dir_id, r, pstmt );
    DUF_SQL_BIND_S( name, fname, r, pstmt );
    DUF_SQL_BIND_LL( dataid, dataid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( r, pstmt );
  }
  else
  {
    DUF_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
    DUF_TEST_R( r );
  }
  /* DUF_TRACE( current, 0, "%llu : %s @ %llu", dir_id, fname, dir_id ); */
  DEBUG_ENDULL( dir_id );
  return r;
}


/* callback of type duf_scan_hook_file_t */
static int DUF_UNUSED
collect_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

/* 
   * --uni-scan   -R   --collect   --files  --dirs  -FF
   *                   ^^^^^^   ^^^^^^^  ^^^^^^
   * */


  if ( DUF_IF_TRACE( collect ) )
  {
    DUF_SFIELD( filename );
    /* DUF_UFIELD( dirid ); */
    /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
    /* duf_dirhandle_t dh;                                        */
    /* char *path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r ); */
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );

    DUF_TEST_R( r );

    DUF_TRACE( collect, 1, "path=%s/%s", real_path, filename );
    /* duf_close_dh( &dh ); */
    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

static int DUF_UNUSED
collect_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

/* 
   * --uni-scan   -R   --collect   --files  --dirs  -FF
   *                   ^^^^^^   ^^^^^^^  ^^^^^^
   * */


  if ( DUF_IF_TRACE( collect ) )
  {
    DUF_SFIELD2( filename );
    /* DUF_UFIELD( dirid ); */
    /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
    /* duf_dirhandle_t dh;                                        */
    /* char *path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r ); */
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );

    DUF_TEST_R( r );

    DUF_TRACE( collect, 0, "path=%s/%s", real_path, filename );
    /* duf_close_dh( &dh ); */
    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

static int
collect_openat_entry_reg( const char *fname, const struct stat *pst_file, unsigned long long dirid, duf_depthinfo_t * pdi,
                    duf_record_t * precord )
{
  int r = 0;
  DUF_UNUSED unsigned long long dataid = 0;

  /* unsigned long long fnid = 0; */

  DEBUG_START(  );

  if ( pst_file && pst_file->st_size >= pdi->u.minsize && ( !pdi->u.maxsize || pst_file->st_size < pdi->u.maxsize ) )
  {
    dataid = duf_insert_filedata_uni( pdi, pst_file, 1 /*need_id */ , &r );
    r = collect_openat_insert_filename_uni( pdi, fname, dirid, dataid );
  }
  DEBUG_ENDR( r );
  return r;
}

static int
collect_openat_entry_reg2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pst_file, unsigned long long dirid,
                     duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );
  DUF_TRACE( scan, 1, "scan entry reg2 by %s", fname );

  if ( pst_file /* && !duf_config->cli.disable.insert */  )
    /* && pst_file->st_size >= pdi->u.minsize && ( !pdi->u.maxsize || pst_file->st_size < pdi->u.maxsize ) */
  {
    DUF_UNUSED unsigned long long dataid = 0;

    DUF_TRACE( scan, 1, "scan entry reg2 by %s", fname );
    dataid = duf_insert_filedata_uni( pdi, pst_file, 1 /*need_id */ , &r );
    DUF_TRACE( scan, 1, "scan entry reg2 by %s", fname );
    r = collect_openat_insert_filename_uni( pdi, fname, dirid, dataid );
    DUF_TRACE( scan, 1, "scan entry reg2 by %s", fname );
  }
  DUF_TRACE( scan, 1, "scan entry reg2 by %s", fname );
  DEBUG_ENDR( r );
  return r;
}


static int
collect_openat_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  ( void ) duf_insert_path_uni( pdi, fname, pstat->st_dev, pstat->st_ino, dirid, 0 /*need_id */ , &r );
  DUF_TEST_R( r );
  return r;
}

static int
collect_openat_entry_dir2( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat *pstat, unsigned long long dirid,
                     duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_TRACE( scan, 0, "scan entry dir2 by %s", fname );
  ( void ) duf_insert_path_uni( pdi, fname, pstat->st_dev, pstat->st_ino, dirid, 0 /*need_id */ , &r );
  DUF_TEST_R( r );
  return r;
}






static char *final_sql[] = {
  "INSERT OR IGNORE INTO duf_pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " GROUP BY fn.Pathid",
  "DELETE FROM duf_sizes",
  "INSERT OR IGNORE INTO duf_sizes (size, dupcnt) " /* */
        "SELECT size, COUNT(*) " /* */
        " FROM duf_filedatas AS fd GROUP BY fd.size",
  "UPDATE duf_pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " WHERE duf_pathtot_files.Pathid=fn.Pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (Pathid, numdirs) " /* */
        "SELECT p.id AS Pathid, COUNT(*) AS numdirs " /* */
        " FROM duf_paths AS p " /* */
        " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " /* */
        " GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET " /* */
        " numdirs=(SELECT COUNT(*) AS numdirs " /* */
        " FROM duf_paths AS p " /* */
        " WHERE p.parentid=duf_pathtot_dirs.Pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, Pathid) " /* */
        "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)" /* */
        " JOIN duf_paths AS p ON (fn.Pathid=p.id)" /* */
        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",


  NULL,
};



duf_scan_callbacks_t duf_collect_openat_callbacks = {
  .title = "collect o",
  .init_scan = NULL,
  .opendir = 1,
  .scan_mode_2 = 1,
  .dirent_dir_scan_before = collect_openat_entry_dir,
  .dirent_dir_scan_before2 = collect_openat_entry_dir2,

  .dirent_file_scan_before = collect_openat_entry_reg,
  .dirent_file_scan_before2 = collect_openat_entry_reg2,

  /* .node_scan_before = collect_scan_node_before, */
  /* .leaf_scan = collect_scan_leaf, */
  /* .leaf_scan2 = collect_scan_leaf2, */
  /* filename for debug only */
  .fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn.id AS filenameid " /* */
        ", md.dupcnt AS nsame, md.md5sum1, md.md5sum2 ",
  .leaf_selector = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN duf_md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        /* " fd.size >= %llu AND fd.size < %llu "                      */
        /* " AND( md.dupcnt IS NULL OR( md.dupcnt >= %llu AND md.dupcnt < %llu ) ) AND " */
        " fn.Pathid = '%llu' ",
  .leaf_selector2 = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " LEFT JOIN duf_md5 AS md ON ( md.id = fd.md5id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirid "  /* */
        " AND (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) " /* */
        " AND (:minsame IS NULL OR md.dupcnt>=:minsame) AND (:maxsame IS NULL OR md.dupcnt<=:maxsame) " /* */
        ,
  .node_selector = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        /* ", ( SELECT count( * ) FROM duf_paths AS subpaths WHERE subpaths.parentid = pt.id ) AS ndirs "        */
        /* ", ( SELECT count( * ) FROM duf_filenames AS sfn "                                                           */
        /* " LEFT JOIN duf_filedatas AS sfd ON( sfn.dataid = sfd.id ) "                                                     */
        /* " JOIN duf_md5 AS smd ON( sfd.md5id = smd.id ) "                                                            */
        /* " WHERE sfn.Pathid = pt.id "                                                                         */
        /* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                */
        /* " AND( smd.dupcnt IS NULL OR( smd.dupcnt >= %llu AND smd.dupcnt < %llu ) ) "                                */
        /* " ) AS nfiles "                                                                                             */
        /* ", ( SELECT min( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = pt.id ) AS minsize "                                                            */
        /* ", ( SELECT max( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = pt.id ) AS maxsize "                                                            */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = '%llu' ",
  .node_selector2 = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = :dirid ",
  .final_sql_argv = final_sql,
};
