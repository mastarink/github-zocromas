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


#include "duf_sql_def.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"

#include "duf_path.h"

#include "duf_add.h"


#include "duf_filedata.h"
#include "duf_dirent.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_collect_uni.h" */
/* ###################################################################### */

/* run  --db-name=test20140412   --uni-scan -R --collect --files --dirs /mnt/new_media/media/down/ */
/*                                                    ^^^^^^^ ^^^^^^                            */



static unsigned long long
duf_insert_filename_uni( const char *fname, unsigned long long dir_id, unsigned long long dataid, int need_id, int *pr )
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
      r = duf_sql( "INSERT OR IGNORE INTO duf_filenames (Pathid, name, dataid) VALUES ('%llu','%s','%llu')",
                   &changes, dir_id, qfname, dataid );
      DUF_TEST_R( r );
    }
    /* DUF_TRACE( current, 0, "<changes> : %d", changes ); */
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "resf" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &resf, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                            &sccb /*, ( const duf_dirhandle_t * ) NULL off */ ,
                            "SELECT id AS resf FROM duf_filenames WHERE Pathid='%lu' AND name='%lu'", dir_id, qfname );
        DUF_TEST_R( r );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        resf = duf_sql_last_insert_rowid(  );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) Pathid=%llu:'%s'", dir_id, fname );
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


/* callback of type duf_scan_hook_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  DEBUG_START(  );

  if ( DUF_IF_TRACE( collect ) )
  {
    DUF_SFIELD( filename );
    /* DUF_UFIELD( dirid ); */
    /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
    /* duf_dirhandle_t dh; */
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );
    /* char *path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r ); */

    DUF_TEST_R( r );

    DUF_TRACE( collect, 1, "path=%s/%s", real_path, filename );
    /* duf_close_dh( &dh ); */
    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

static unsigned long long
duf_update_realpath_file_name_inode_filter_uni( const char *real_path, const char *fname, unsigned long long pathid_unused,
                                                duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;
  unsigned long long fnid = 0;
  struct stat st_file, *pst_file = NULL;
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_START(  );
  {
    char *fpath;

    fpath = duf_join_path( real_path, fname );
    r = stat( fpath, &st_file );
    if ( r )
    {
/* no such entry */
      DUF_ERROR( "No such entry %s", fpath );
      r = DUF_ERROR_STAT;
    }
    else
    {
      pst_file = &st_file;
    }
    mas_free( fpath );
  }
  if ( st_file.st_size >= pdi->u.minsize && ( !pdi->u.maxsize || st_file.st_size < pdi->u.maxsize ) )
  {
    dataid = duf_insert_filedata_uni( pst_file, 1 /*need_id */ , &r );
    fnid = duf_insert_filename_uni( fname, dirid, dataid, need_id /*need_id */ , &r );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( fnid );
  return fnid;
}

static unsigned long long
duf_collect_file_info_by_pdh_and_name_and_pathid(   const char *fname, unsigned long long pathid_unused,
                                                  duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;
  unsigned long long fnid = 0;
  struct stat st_file;
  const struct stat *pst_file = NULL;
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_START(  );
  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

  /* DUF_TRACE( current, 0, "%p ? %p", ( void * ) pdh_unused, ( void * ) pdhi ); */
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
    fnid = duf_insert_filename_uni( fname, dirid, dataid, need_id, &r );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( fnid );
  return fnid;
}

/* ------------------------------------------------------------------- */
static unsigned long long
duf_collect_file_or_dir_info_by_rp_and_de_and_pdi( const char *real_path, struct dirent *de, unsigned long long pathid_unused,
                                                   duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long itemid = 0;
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_START(  );

  switch ( de->d_type )
  {
  case DT_REG:
    {
      DUF_TRACE( collect, 1, "regfile='%s/%s'", real_path, de->d_name );
      itemid = duf_update_realpath_file_name_inode_filter_uni( real_path, de->d_name, dirid, pdi, need_id, &r );
    }
    break;
  case DT_DIR:
    {
      struct stat st, *pst = NULL;
      int r;
      char *fpath;

      fpath = duf_join_path( real_path, de->d_name );
      r = stat( fpath, &st );
      if ( r )
      {
        DUF_ERROR( "No such entry %s", fpath );
        r = DUF_ERROR_STAT;
      }
      else
      {
        pst = &st;
        ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, dirid, 0 /*need_id */ , &r );
      }
      mas_free( fpath );

      DUF_TRACE( collect, 1, "dir='%s/%s'", real_path, de->d_name );

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

static unsigned long long
duf_collect_file_or_dir_info_by_pdh_and_pdi( struct dirent *de,
                                             unsigned long long pathid_unused, duf_depthinfo_t * pdi, int need_id, int *pr )
{
  int r = 0;
  unsigned long long itemid = 0;
  unsigned long long dirid = duf_levinfo_dirid( pdi );
  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );

  DEBUG_START(  );

  /* DUF_TRACE( current, 0, "%p ? %p", ( void * ) pdh_unused, ( void * ) pdhi ); */

  switch ( de->d_type )
  {
  case DT_REG:
    {
      DUF_TRACE( collect, 1, "regfile='.../%s'", de->d_name );
      itemid = duf_collect_file_info_by_pdh_and_name_and_pathid(  /* pdh_unused, */ de->d_name, dirid, pdi, 1 /*need_id */ , &r );
    }
    break;
  case DT_DIR:
    {
      struct stat st, *pst = NULL;

      r = fstatat( pdhi->dfd, de->d_name, &st, 0 );
      if ( r < 0 )
      {
        const char *real_path = NULL;

        if ( !real_path )
          real_path = duf_levinfo_path( pdi );

        DUF_ERROR( "No such entry %s/%s", real_path, de->d_name );
        r = DUF_ERROR_STAT;
      }
      else
      {
        pst = &st;
        ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, dirid, 0 /*need_id */ , &r );
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


/* to replace duf_update_realpath_entries_filter */
/* TODO scan for removed files; mark as absent or remove from db */
static int
duf_collect_ent_flt_uni( unsigned long long pathid_unused, duf_depthinfo_t * pdi, const char *dfname )
{
  int r = 0;
  int items = 0;

  struct stat st_dir;
  const struct stat *pst_dir;
  const char *real_path = NULL;

  /* char *path = NULL; */

  const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi );
  int noopenat = !pdhi;

  if ( noopenat )
  {
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );
    /* char *path = NULL; */
    /* duf_dirhandle_t dh; */

    /* if ( !path )                                            */
    /*   path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r ); */
    DUF_TEST_R( r );
    if ( r >= 0 )
      r = stat( real_path, &st_dir );
    DUF_TEST_R( r );
    pst_dir = &st_dir;
    /* duf_close_dh( &dh ); */
    /* mas_free( path ); */
    /* path = NULL; */
  }
  else
  {
    pst_dir = &pdhi->st;
  }

/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_dir || !( S_ISDIR( pst_dir->st_mode ) ) )
  {
    if ( !real_path )
      real_path = duf_levinfo_path( pdi );
    /* char *rp = duf_pathid_to_path_s( dirid, pdi, &r ); */

/* no such entry */
    DUF_ERROR( "No such entry %s", real_path );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( collect, 1, "No such entry %s", noopenat ? real_path : dfname );
    /* mas_free( rp ); */
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    unsigned long long dirid = duf_levinfo_dirid( pdi );

    DEBUG_START(  );
    DUF_TRACE( collect, 1, "dirid=%llu; scandir dfname:[%s]", dirid, dfname );
    if ( noopenat )
      nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
    else
      nlist = scandirat( pdhi->dfd, ".", &list, duf_direntry_filter, alphasort );
    DUF_TRACE( collect, 1, "dirid=%llu; nlist=%d", dirid, nlist );
    if ( nlist < 0 )
    {
      /* char serr[512] = "";                                                                                            */
      /*                                                                                                                 */
      /* perror( "scandir" );                                                                                            */
      /* strerror_r( errno, serr, sizeof( serr ) - 1 );                                                                  */
      /* DUF_ERROR( "%s; nlist= %d; for %s; dfd:%d", serr, nlist, noopenat ? real_path : dfname, pdhi ? pdhi->dfd : -33 ); */
      int e = errno;

      /* if ( !real_path )                                      */
      /*   real_path = duf_pathid_to_path_s( dirid, pdi, &r ); */

      if ( !real_path )
        real_path = duf_levinfo_path( pdi );
      if ( noopenat )
        DUF_ERRSYSE( e, "%s / %s; dfd:%d", real_path ? real_path : "...", dfname, pdhi ? pdhi->dfd : -33 );
      else
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

          DUF_TRACE( collect, 1, "dirid=%llu; entry='%s'", dirid, list[il]->d_name );
          if ( noopenat )
            itemid = duf_collect_file_or_dir_info_by_rp_and_de_and_pdi( real_path, list[il], dirid, pdi, 1 /*need_id */ , &r );
          else
            itemid = duf_collect_file_or_dir_info_by_pdh_and_pdi( list[il], dirid, pdi, 1 /*need_id */ , &r );
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

  /* if ( noopenat )        */
  /*   duf_close_dh( &dh ); */
  /* mas_free( path ); */
  DUF_TEST_R( r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, items );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
collect_scan_node_before( unsigned long long pathid_unused, /* const duf_dirhandle_t * pdh_unused, */ duf_depthinfo_t * pdi,
                          duf_record_t * precord )
{
  int r = 0;
  unsigned long long dirid = duf_levinfo_dirid( pdi );


  DEBUG_START(  );
  /* const duf_dirhandle_t *pdhi = duf_levinfo_pdh( pdi ); */

  /* if ( pdh_unused )                                                */
  /*   DUF_TRACE( collect, 0, "pdh_unused:%p", ( void * ) pdh_unused ); */
  DUF_TRACE( collect, 1, "dirid=%llu", dirid );

  /* DUF_TRACE( current, 0, "%p ? %p", ( void * ) pdh_unused, ( void * ) pdhi ); */

  /* dirid needless? */
  assert( dirid == pdi->levinfo[pdi->depth].dirid );
  {
    /* char *path = NULL; */
    const char *real_path = NULL;

    if ( DUF_IF_TRACE( collect ) )
    {
      const char *real_path = NULL;

      if ( !real_path )
        real_path = duf_levinfo_path( pdi );
      /* path = duf_pathid_to_path_s( dirid, pdi, &r ); */

      DUF_TRACE( collect, 1, "real_path=%s", real_path );
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
      DUF_TRACE( collect, 1, "dirid=%llu; scandir dfname:[%s]", dirid, dfname );
      /* r = duf_collect_pathid_filter_uni( dirid, pdh_unused, pdi ); */
      /* r = duf_collect_pi_flt_uni( dirid, pdh_unused, pdi ); */
      /* r = duf_collect_entries_filter_uni( dirid, pdh_unused, pdi ); */
      r = duf_collect_ent_flt_uni( dirid, /* pdh_unused, */ pdi, dfname );
    }

    if ( DUF_IF_TRACE( collect ) )
    {
      DUF_TRACE( collect, 1, "real_path=%s", real_path );
    }
    /* mas_free( path ); */
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}






 /* *INDENT-OFF*  */

static char *final_sql[] = {
  "INSERT OR IGNORE INTO duf_pathtot_files (Pathid, numfiles, minsize, maxsize) " /*	*/
        " SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /*	*/
	  " FROM duf_filenames AS fn " /*	*/
	      " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	" GROUP BY fn.Pathid",
  "DELETE FROM duf_sizes",
  "INSERT OR IGNORE INTO duf_sizes (size, dupcnt) " /*	*/
        "SELECT size, COUNT(*) " /*	*/
          " FROM duf_filedatas AS fd GROUP BY fd.size",
  "UPDATE duf_pathtot_files SET " /*	*/
      " minsize=(SELECT min(size) AS minsize " /*	*/
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	     " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /*	*/
     ", maxsize=(SELECT max(size) AS maxsize " /*	*/
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	     " WHERE duf_pathtot_files.Pathid=fn.Pathid) " /*	*/
     ", numfiles=(SELECT COUNT(*) AS numfiles " /*	*/
          " FROM duf_filenames AS fn JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /*	*/
	     " WHERE duf_pathtot_files.Pathid=fn.Pathid)",
  "INSERT OR IGNORE INTO duf_pathtot_dirs (Pathid, numdirs) " /*	*/
        " SELECT p.id AS Pathid, COUNT(*) AS numdirs " /*	*/
	  " FROM duf_paths AS p " /*	*/
            " LEFT JOIN duf_paths AS sp ON (sp.parentid=p.id) " /*	*/
	" GROUP BY sp.parentid",
  "UPDATE duf_pathtot_dirs SET " /*	*/
      " numdirs=(SELECT COUNT(*) AS numdirs " /*	*/
                  " FROM duf_paths AS p " /*	*/
                      " WHERE p.parentid=duf_pathtot_dirs.Pathid )",
  /* "DELETE FROM duf_keydata", */
  "INSERT OR REPLACE INTO duf_keydata (md5id, filenameid, dataid, Pathid) " /*	*/
      "SELECT md.id AS md5id, fn.id AS filenameid, fd.id AS dataid, p.id AS Pathid " /*	*/
	  " FROM duf_filenames AS fn " /*	*/
	    " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id)" /*	*/
	      " JOIN duf_paths AS p ON (fn.Pathid=p.id)" /*	*/
	        " JOIN duf_md5 AS md ON (fd.md5id=md.id)",

 /* *INDENT-ON*  */

NULL,};



duf_scan_callbacks_t duf_collect_noopenat_callbacks = {
  .title = "collect n/o",
  .init_scan = NULL,
  .opendir = 0,
  .node_scan_before = collect_scan_node_before,
  .leaf_scan = scan_leaf,
  /* filename for debug only */
  .fieldset = " fn.Pathid AS dirid, " /*	*/
    " fn.name AS filename, fd.size AS filesize " /*	*/
        ", uid, gid, nlink, inode, mtim AS mtime " /*	*/
        ", fd.mode AS filemode " /*	*/
	", fn.id AS filenameid " /*	*/
	", md.dupcnt AS nsame, md.md5sum1, md.md5sum2 ",
  .leaf_selector =
        " SELECT %s FROM duf_filenames AS fn " /*	*/
        " LEFT JOIN duf_filedatas AS fd ON ( fn.dataid = fd.id ) " /*	*/
        " LEFT JOIN duf_md5 AS md ON ( md.id = fd.md5id ) " /*	*/
	" WHERE " /*	*/
        /* " fd.size >= %llu AND fd.size < %llu "                      */
        /* " AND( md.dupcnt IS NULL OR( md.dupcnt >= %llu AND md.dupcnt < %llu ) ) AND " */
        " fn.Pathid = '%llu' ",
  .node_selector =
        " SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid " /*	*/
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /*	*/
        /* ", ( SELECT count( * )FROM duf_paths AS subpaths WHERE subpaths.parentid = duf_paths.id ) AS ndirs "        */
        /* ", ( SELECT count( * )FROM duf_filenames AS sfn "                                                           */
        /* " LEFT JOIN duf_filedatas AS sfd ON( sfn.dataid = sfd.id ) "                                                     */
        /* " JOIN duf_md5 AS smd ON( sfd.md5id = smd.id ) "                                                            */
        /* " WHERE sfn.Pathid = duf_paths.id "                                                                         */
        /* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                */
        /* " AND( smd.dupcnt IS NULL OR( smd.dupcnt >= %llu AND smd.dupcnt < %llu ) ) "                                */
        /* " ) AS nfiles "                                                                                             */
        /* ", ( SELECT min( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = duf_paths.id ) AS minsize "                                                            */
        /* ", ( SELECT max( sfd.size ) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON( sfn.dataid = sfd.id ) " */
        /* " WHERE sfn.Pathid = duf_paths.id ) AS maxsize "                                                            */
        " FROM duf_paths " /*	*/
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=duf_paths.id) " /*	*/
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=duf_paths.id) " /*	*/
	" WHERE duf_paths.parentid = '%llu' ",
  .final_sql_argv = final_sql,
};
