#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>
#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"
#include "duf_dh.h"

#include "duf_levinfo.h"


#include "duf_sql_field.h"
#include "duf_path.h"

#include "duf_add.h"


#include "duf_filedata.h"
#include "duf_dirent.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_dbg.h"

/* run  --db-name=test20140412   --uni-scan -R --collect --files --dirs /mnt/new_media/media/down/ */
/*                                                    ^^^^^^^ ^^^^^^                            */



static int
duf_collect_insert_filename_uni( const char *fname, unsigned long long dir_id, unsigned long long dataid )
{
  int r = 0;

  /* unsigned long long resf = 0; */

  DEBUG_START(  );

  if ( fname && dir_id )
  {
    int r;
    int changes = 0;
    char *qbase_name = NULL;
    const char *qfname;

    qbase_name = duf_single_quotes_2( fname );
    qfname = qbase_name ? qbase_name : fname;
    {
      r = duf_sql( "INSERT OR IGNORE INTO duf_filenames (pathid, name, dataid) VALUES ('%llu','%s','%llu')",
                   &changes, dir_id, qfname, dataid );
      DUF_TEST_R( r );
    }
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      /* if ( need_id )                                                                                                            */
      /* {                                                                                                                         */
      /*   duf_scan_callbacks_t sccb = {.fieldset = "resf" };                                                                      */
      /*   r = duf_sql_select( duf_sel_cb_field_by_sccb, &resf, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,          */
      /*                       &sccb (*, ( const duf_dirhandle_t * ) NULL off *) ,                                                 */
      /*                       "SELECT id AS resf " " FROM duf_filenames " " WHERE pathid='%lu' AND name='%lu'", dir_id, qfname ); */
      /*   DUF_TEST_R( r );                                                                                                        */
      /* }                                                                                                                         */
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      /* if ( need_id )                                                                     */
      /* {                                                                                  */
      /*   resf = duf_sql_last_insert_rowid(  );                                            */
      /*   DUF_TRACE( collect, 1, "inserted (SQLITE_OK) pathid=%llu:'%s'", dir_id, fname ); */
      /* }                                                                                  */
    }
    else
    {
      DUF_ERROR( "insert filename [%s] %d", fname, r );
    }
    mas_free( qbase_name );
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
static int
collect_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );
/* stat */
  DUF_TRACE( scan, 0, "*" );

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

static int
duf_scan_entry_reg( const char *fname, const struct stat *pst_file, unsigned long long dirid, duf_depthinfo_t * pdi,
                    duf_record_t * precord )
{
  int r = 0;
  unsigned long long dataid = 0;

  /* unsigned long long fnid = 0; */

  DEBUG_START(  );

  if ( pst_file && pst_file->st_size >= pdi->u.minsize && ( !pdi->u.maxsize || pst_file->st_size < pdi->u.maxsize ) )
  {
    dataid = duf_insert_filedata_uni( pst_file, 1 /*need_id */ , &r );
    r = duf_collect_insert_filename_uni( fname, dirid, dataid );
  }
  DEBUG_ENDR( r );
  return r;
}

static int
duf_scan_entry_dir( const char *fname, const struct stat *pstat, unsigned long long dirid, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  ( void ) duf_insert_path_uni( fname, pstat->st_dev, pstat->st_ino, dirid, 0 /*need_id */ , &r );
  return r;
}




 /* *INDENT-OFF*  */

static char *final_sql[] = {
  "INSERT OR IGNORE INTO duf_pathtot_files (pathid, numfiles, minsize, maxsize) "
        " SELECT fn.id AS pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize "
	  " FROM duf_filenames AS fn "
	      " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
	" GROUP BY fn.pathid",
  "UPDATE duf_pathtot_files SET "
      " minsize=(SELECT min(size) AS minsize "
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
	     " WHERE duf_pathtot_files.pathid=fn.pathid) "
     ", maxsize=(SELECT max(size) AS maxsize "
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
	     " WHERE duf_pathtot_files.pathid=fn.pathid) "
     ", numfiles=(SELECT COUNT(*) AS numfiles "
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) "
	     " WHERE duf_pathtot_files.pathid=fn.pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (pathid, numdirs) "
        " SELECT p.id AS pathid, COUNT(*) AS numdirs "
	  " FROM duf_paths AS p "
            " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) "
	" GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET "
      " numdirs=(SELECT COUNT(*) AS numdirs "
                  " FROM duf_paths AS p "
                      " WHERE p.parentid=duf_pathtot_dirs.pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, pathid) "
      "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS pathid "
	  " FROM duf_filenames AS fn "
	    " JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)"
	      " JOIN duf_paths AS p ON (fn.pathid=p.id)"
	        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

 /* *INDENT-ON*  */

NULL,};



duf_scan_callbacks_t duf_collect_openat_callbacks = {
  .title = __FILE__,.init_scan = NULL,
  .opendir = 1,
  .entry_dir_scan_before = duf_scan_entry_dir,
  .entry_file_scan_before = duf_scan_entry_reg,
  /* .node_scan_before = collect_scan_node_before, */
  .leaf_scan = collect_scan_leaf,
  /* filename for debug only */
  .fieldset = " duf_filenames.pathid AS dirid, " " duf_filenames.name AS filename, duf_filedatas.size AS filesize "
        ", uid, gid, nlink, inode, mtim AS mtime "
        ", duf_filedatas.mode AS filemode " ", duf_filenames.id AS filenameid " ", md.dupcnt AS nsame, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        " SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on( duf_filenames.dataid = duf_filedatas.id ) "
        " LEFT JOIN duf_md5 AS md on( md.id = duf_filedatas.md5id ) " " WHERE "
        /* " duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "                      */
        /* " AND( md.dupcnt IS NULL OR( md.dupcnt >= %llu AND md.dupcnt < %llu ) ) AND " */
        " duf_filenames.pathid = '%llu' ",
  .node_selector =
        " SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* ", ( SELECT count( * )FROM duf_paths AS subpaths WHERE subpaths.parentid = duf_paths.id ) AS ndirs "        */
        /* ", ( SELECT count( * )FROM duf_filenames AS sfn "                                                           */
        /* " JOIN duf_filedatas AS sfd ON( sfn.dataid = sfd.id ) "                                                     */
        /* " JOIN duf_md5 AS smd ON( sfd.md5id = smd.id ) "                                                            */
        /* " WHERE sfn.pathid = duf_paths.id "                                                                         */
        /* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                */
        /* " AND( smd.dupcnt IS NULL OR( smd.dupcnt >= %llu AND smd.dupcnt < %llu ) ) "                                */
        /* " ) AS nfiles "                                                                                             */
        /* ", ( SELECT min( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.pathid = duf_paths.id ) AS minsize "                                                            */
        /* ", ( SELECT max( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.pathid = duf_paths.id ) AS maxsize "                                                            */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid = '%llu' ",
  .final_sql_argv = final_sql,
};
