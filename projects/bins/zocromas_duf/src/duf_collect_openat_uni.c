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



static unsigned long long
duf_collect_insert_filename_uni( const char *fname, unsigned long long dir_id, unsigned long long dataid, int need_id, int *pr )
{
  int r = 0;
  unsigned long long resf = 0;

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
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "resf" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &resf, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb, ( const duf_dirhandle_t * ) NULL,
                            "SELECT id AS resf " " FROM duf_filenames " " WHERE pathid='%lu' AND name='%lu'", dir_id, qfname );
        DUF_TEST_R( r );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        resf = duf_sql_last_insert_rowid(  );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) pathid=%llu:'%s'", dir_id, fname );
      }
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
  if ( pr )
    *pr = r;
  /* DUF_TRACE( current, 0, "%llu : %s @ %llu", dir_id, fname, dir_id ); */
  DEBUG_ENDULL( dir_id );
  return resf;
}


/* callback of type duf_scan_callback_file_t */
static int
collect_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
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
    DUF_UFIELD( dirid );
    /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
    duf_dirhandle_t dh;
    char *path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r );

    DUF_TEST_R( r );

    DUF_TRACE( collect, 1, "path=%s/%s", path, filename );
    duf_close_dh( &dh );
    mas_free( path );
  }
  DEBUG_ENDR( r );
  return r;
}

static unsigned long long
duf_collect_file_info_by_pdh_and_name_and_pathid( const char *fname, unsigned long long dirid, duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;
  unsigned long long fnid = 0;
  struct stat st_file;
  const struct stat *pst_file = NULL;

  DEBUG_START(  );
  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

  r = fstatat( pdhi->dfd, fname, &st_file, 0 );
  if ( r < 0 )
  {
    r = DUF_ERROR_STAT;
  }
  else
    pst_file = &st_file;
  if ( pst_file && st_file.st_size >= pdi->u.minsize && ( !pdi->u.maxsize || st_file.st_size < pdi->u.maxsize ) )
  {
    dataid = duf_insert_filedata_uni( pst_file, 1 /*need_id */ , &r );
    fnid = duf_collect_insert_filename_uni( fname, dirid, dataid, need_id, &r );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDR( r );
  return fnid;
}

static unsigned long long
duf_collect_file_or_dir_info_by_pdh_and_name_and_pathid( struct dirent *de,
                                                         unsigned long long pathid, duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long itemid = 0;

  DEBUG_START(  );
  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

  switch ( de->d_type )
  {
  case DT_REG:
    {
      DUF_TRACE( collect, 1, "regfile='.../%s'", de->d_name );
      itemid = duf_collect_file_info_by_pdh_and_name_and_pathid( de->d_name, pathid, pdi, 1 /*need_id */ , &r );
    }
    break;
  case DT_DIR:
    {
      struct stat st, *pst = NULL;

      r = fstatat( pdhi->dfd, de->d_name, &st, 0 );
      if ( r < 0 )
      {
        char *rp = duf_pathid_to_path_s( pathid, pdi, &r );

        DUF_ERROR( "No such entry %s/%s", rp, de->d_name );
        mas_free( rp );
        r = DUF_ERROR_STAT;
      }
      else
      {
        pst = &st;
        ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, pathid, 0 /*need_id */ , &r );
      }

      DUF_TRACE( collect, 1, "dir='.../%s'", de->d_name );

      /* don't return itemid ??? */
      /* itemid = */
    }
    break;
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( itemid );
  return itemid;
}


/* TODO scan for removed files; mark as absent or remove from db */
static int
duf_collect_ent_flt_uni( unsigned long long pathid, duf_depthinfo_t * pdi, const char *dfname )
{
  int r = 0;
  int items = 0;

  const struct stat *pst_dir;
  char *real_path = NULL;

  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

  assert( pdhi );

  pst_dir = &pdhi->st;

/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_dir || !( S_ISDIR( pst_dir->st_mode ) ) )
  {
    char *rp = duf_pathid_to_path_s( pathid, pdi, &r );

/* no such entry */
    DUF_ERROR( "No such entry %s", rp );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( collect, 1, "No such entry %s", dfname );
    mas_free( rp );
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    DEBUG_START(  );
    DUF_TRACE( collect, 1, "pathid=%llu; scandir dfname:[%s]", pathid, dfname );
    nlist = scandirat( pdhi->dfd, ".", &list, duf_direntry_filter, alphasort );
    DUF_TRACE( collect, 1, "pathid=%llu; nlist=%d", pathid, nlist );
    if ( nlist < 0 )
    {
      int e = errno;

      /* TODO change to levinfo_path... */
      if ( !real_path )
        real_path = duf_pathid_to_path_s( pathid, pdi, &r );

      DUF_ERRSYSE( e, "%s", real_path ? real_path : "..." );

      if ( e == EACCES )
        r = 0;
      else
        r = DUF_ERROR_SCANDIR;
      DUF_TEST_R( r );
    }
    else
    {
      for ( int il = 0; il < nlist; il++ )
      {
        {
          unsigned long long itemid = 0;

          DUF_TRACE( collect, 1, "pathid=%llu; entry='%s'", pathid, list[il]->d_name );

          itemid = duf_collect_file_or_dir_info_by_pdh_and_name_and_pathid(  /* pdhi, */ list[il], pathid, pdi, 1 /*need_id */ , &r );

          DUF_TEST_R( r );
          if ( itemid )
            items++;
        }
        if ( list[il] )
          free( list[il] );
      }
      if ( list )
        free( list );
      DUF_TEST_R( r );
    }
  }

  mas_free( real_path );
  DEBUG_ENDR( r );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
collect_scan_node_before( unsigned long long pathid, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );
  DUF_TRACE( collect, 1, "pathid=%llu", pathid );

  /* pathid needless? */
  assert( pathid == pdi->levinfo[pdi->depth].dirid );
  {
    char *path = NULL;

    if ( DUF_IF_TRACE( collect ) )
    {
      path = duf_pathid_to_path_s( pathid, pdi, &r );

      DUF_TRACE( collect, 1, "path=%s", path );
    }
/* TODO additionally 
 * for each direntry:
 *  -- 
 * */

/* stat */
    /* duf_collect_pathid_filter_uni:
     * update (collected below) information for path
     * */
    {
      DUF_SFIELD( dfname );
      DUF_TRACE( collect, 1, "pathid=%llu; scandir dfname:[%s]", pathid, dfname );
      r = duf_collect_ent_flt_uni( pathid, pdi, dfname );
    }

    if ( DUF_IF_TRACE( collect ) )
    {
      DUF_TRACE( collect, 1, "path=%s", path );
    }
    mas_free( path );
  }
  DEBUG_ENDR( r );
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
  .node_scan_before = collect_scan_node_before,
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
