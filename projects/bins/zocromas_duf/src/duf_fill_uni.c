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



/* #include "duf_sql.h" */
#include "duf_sql_field.h"
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */

#include "duf_add.h"


#include "duf_filedata.h"
/* #include "duf_filename.h" */
#include "duf_dirent.h"
#include "duf_file_scan.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_fill_uni.h" */
/* ###################################################################### */

/* run  --db-name=test20140412   --uni-scan -R --fill --files --dirs /mnt/new_media/media/down/ */
/*                                                    ^^^^^^^ ^^^^^^                            */



/* static unsigned long long                                                                                                            */
/* duf_insert_filename_uni( const char *fname, unsigned long long dir_id, unsigned long long resd )                                     */
/* {                                                                                                                                    */
/*   unsigned long long resf = 0;                                                                                                       */
/*   int r;                                                                                                                             */
/*   char *qfname;                                                                                                                      */
/*   int changes = 0;                                                                                                                   */
/*                                                                                                                                      */
/*   qfname = duf_single_quotes_2( fname );                                                                                             */
/*   r = duf_sql_c( "INSERT OR IGNORE INTO duf_filenames (pathid, dataid, name, ucnt, now) " " VALUES ('%lu','%lu','%s',0,datetime())", */
/*                  DUF_CONSTRAINT_IGNORE_YES, &changes, dir_id, resd, qfname ? qfname : fname );                                       */
/*                                                                                                                                      */
/*   DUF_TRACE( current, 0, "<changes> : %d", changes );                                                                                */
/*   (* if ( !r (* assume SQLITE_OK *)  ) *)                                                                                            */
/*   if ( changes || !r )                                                                                                               */
/*   {                                                                                                                                  */
/*     resf = duf_sql_last_insert_rowid(  );                                                                                            */
/*     (* fprintf( stderr, "INSERT INTO duf_filenames :: %llu. [%s]\x1b[K\n", resf, qfname ? qfname : fname ); *)                       */
/*   }                                                                                                                                  */
/*   else if ( r != DUF_SQL_CONSTRAINT )                                                                                                */
/*     DUF_ERROR( "!!! %d\n", r );                                                                                                      */
/*   mas_free( qfname );                                                                                                                */
/*   return resf;                                                                                                                       */
/* }                                                                                                                                    */

static unsigned long long
duf_insert_filename_uni( const char *fname, unsigned long long dir_id, unsigned long long resd, int *pr )
{
  int r = 0;
  unsigned long long resf = 0;

  if ( fname && dir_id )
  {
    int r;
    int changes = 0;
    char *qbase_name = NULL;
    const char *qfname;

    duf_dbgfunc( DBG_START, __func__, __LINE__ );
    qbase_name = duf_single_quotes_2( fname );
    qfname = qbase_name ? qbase_name : fname;
    {
      /* r = duf_sql_c( "INSERT OR IGNORE INTO duf_filenames (pathid, dataid, name, ucnt, now) " " VALUES ('%lu','%lu','%s',0,datetime())", */
      /*                DUF_CONSTRAINT_IGNORE_YES, &changes, dir_id, resd, qfname );                                                        */
      r = duf_sql( "INSERT OR IGNORE INTO duf_filenames (pathid, dataid, name, ucnt, now) " " VALUES ('%lu','%lu','%s',0,datetime())",
                   &changes, dir_id, resd, qfname );
    }
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      duf_scan_callbacks_t sccb = {.fieldset = "resf" };
      r = duf_sql_select( duf_sel_cb_field_by_sccb, &resf, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                          &sccb, ( const duf_dirhandle_t * ) NULL,
                          "SELECT id as resf " " FROM duf_filenames " " WHERE pathid='%lu' and name='%lu'", dir_id, qfname );
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      resf = duf_sql_last_insert_rowid(  );
      DUF_TRACE( fill, 1, "inserted (SQLITE_OK) pathid=%llu:'%s'", dir_id, fname );
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
  }
  if ( pr )
    *pr = r;
  DUF_TRACE( current, 0, "%llu : %s @ %llu", dir_id, fname, dir_id );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, dir_id );
  return resf;
}


/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_fill_uni( void *str_cb_udata, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */


/* static int                                                                                                                        */
/* duf_sql_update_filedatas(  duf_record_t * precord, va_list args, void *sel_cb_udata,                          */
/*                           duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb ) */


  /* 
   * --uni-scan   -R   --fill   --files  --dirs  -FF
   *                   ^^^^^^   ^^^^^^^  ^^^^^^
   * */


  if ( DUF_IF_TRACE( fill ) )
  {
    DUF_SFIELD( filename );
    DUF_UFIELD( dirid );
    /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
    duf_dirhandle_t dh;
    char *path = duf_pathid_to_path_dh( dirid, &dh, pdi, &r );

    DUF_TEST_R( r );

    DUF_TRACE( fill, 1, "path=%s/%s", path, filename );
    duf_close_dh( &dh );
    mas_free( path );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

static unsigned long long
duf_update_realpath_file_name_inode_filter_uni( const char *real_path, const char *fname, unsigned long long pathid, duf_depthinfo_t * pdi,
                                                int *pr )
{
  int r = 0;
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file, *pst_file = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
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
    /* fprintf( stderr, "duf_insert_filedata %lu %lu\n", file_inode, pst_file->st_dev ); */
    resd = duf_insert_filedata_uni( pst_file, &r );
    resf = duf_insert_filename_uni( fname, pathid, resd, &r );
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

static unsigned long long
duf_update_dfd_file_name_inode_filter_uni( const duf_dirhandle_t * pdh, const char *fname, unsigned long long pathid, duf_depthinfo_t * pdi,
                                           int *pr )
{
  int r = 0;
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file;
  const struct stat *pst_file = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  r = fstatat( pdh->dfd, fname, &st_file, 0 );
  if ( r < 0 )
  {
    r = DUF_ERROR_STAT;
  }
  else
    pst_file = &st_file;
  if ( pst_file && st_file.st_size >= pdi->u.minsize && ( !pdi->u.maxsize || st_file.st_size < pdi->u.maxsize ) )
  {
    /* fprintf( stderr, "duf_insert_filedata %lu %lu\n", file_inode, pst_file->st_dev ); */
    resd = duf_insert_filedata_uni( pst_file, &r );
    resf = duf_insert_filename_uni( fname, pathid, resd, &r );
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

/* ------------------------------------------------------------------- */
static unsigned long long
duf_fill_rfl_flt_uni( const char *real_path, struct dirent *de, unsigned long long pathid, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long itemid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  switch ( de->d_type )
  {
  case DT_REG:
    {
      DUF_TRACE( fill, 1, "regfile='%s/%s'", real_path, de->d_name );
      itemid = duf_update_realpath_file_name_inode_filter_uni( real_path, de->d_name, pathid, pdi, &r );
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
        ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, pathid, &r );
      }
      mas_free( fpath );

      DUF_TRACE( fill, 1, "dir='%s/%s'", real_path, de->d_name );

      /* don't return itemid ??? */
      /* itemid = */
    }
    break;
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, itemid );
  return itemid;
}

static unsigned long long
duf_fill_dfl_flt_uni( const duf_dirhandle_t * pdh, struct dirent *de, unsigned long long pathid, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long itemid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  switch ( de->d_type )
  {
  case DT_REG:
    {
      DUF_TRACE( fill, 1, "regfile='.../%s'", de->d_name );
      itemid = duf_update_dfd_file_name_inode_filter_uni( pdh, de->d_name, pathid, pdi, &r );
    }
    break;
  case DT_DIR:
    {
      struct stat st, *pst = NULL;

      r = fstatat( pdh->dfd, de->d_name, &st, 0 );
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
        ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, pathid, &r );
      }

      DUF_TRACE( fill, 1, "dir='.../%s'", de->d_name );

      /* don't return itemid ??? */
      /* itemid = */
    }
    break;
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, itemid );
  return itemid;
}


/* to replace duf_update_realpath_entries_filter */
/* TODO scan for removed files; mark as absent or remove from db */
static int
duf_fill_ent_flt_uni( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi, const char *dfname )
{
  int r = 0;
  int items = 0;

  struct stat st_dir;
  const struct stat *pst_dir;
  char *real_path = NULL;
  duf_dirhandle_t dh;
  int noopenat = duf_config->cli.noopenat || !pdh;

  /* if ( pdh->dfd )                                                                                                                */
  /* {                                                                                                                              */
  /*   struct stat stt;                                                                                                             */
  /*                                                                                                                                */
  /*   if ( 0 != fstat( pdh->dfd, &stt ) || pdh->st.st_ino != stt.st_ino )                                                          */
  /*   {                                                                                                                            */
  /*     DUF_ERROR( "Transitional error 1 : dfd:%d ( %ld ? %ld ) %s %s", pdh->dfd, pdh->st.st_ino, stt.st_ino, dfname, real_path ); */
  /*   }                                                                                                                            */
  /*   if ( 0 != fstatat( pdh->dfd, dfname, &stt, 0 ) )                                                                             */
  /*   {                                                                                                                            */
  /*     char serr[1024] = "";                                                                                                      */
  /*     char *s = strerror_r( errno, serr, sizeof( serr ) );                                                                       */
  /*                                                                                                                                */
  /*     DUF_ERROR( "Transitional error 2 : %s %s {%s}", dfname, real_path, s );                                                    */
  /*   }                                                                                                                            */
  /* }                                                                                                                              */


  if ( noopenat )
  {
    real_path = duf_pathid_to_path_dh( pathid, &dh, pdi, &r );
    DUF_TEST_R( r );
    r = stat( real_path, &st_dir );
    DUF_TEST_R( r );
    pst_dir = &st_dir;
  }
  else
  {
    pst_dir = &pdh->st;
  }

/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !pst_dir || !( S_ISDIR( pst_dir->st_mode ) ) )
  {
    char *rp = duf_pathid_to_path_s( pathid, pdi, &r );

/* no such entry */
    DUF_ERROR( "No such entry %s", rp );
    /* TODO mark as absent or remove from db */

    DUF_TRACE( fill, 1, "No such entry %s", noopenat ? real_path : dfname );
    mas_free( rp );
    r = DUF_ERROR_STAT;
  }
  else
  {
    int nlist;
    struct dirent **list = NULL;

    duf_dbgfunc( DBG_START, __func__, __LINE__ );
    /* fprintf( stderr, "Update path entries %s\n", noopenat?real_path:dfname ); */
    DUF_TRACE( fill, 1, "pathid=%llu; scandir dfname:[%s]", pathid, dfname );
    if ( noopenat )
      nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
    else
      nlist = scandirat( pdh->dfd, ".", &list, duf_direntry_filter, alphasort );
    if ( nlist < 0 )
    {
      char serr[512] = "";

      perror( "scandir" );
      strerror_r( errno, serr, sizeof( serr ) - 1 );
      fprintf( stderr, "ERROR %s; nlist= %d; for %s; dfd:%d\n", serr, nlist, noopenat ? real_path : dfname, pdh ? pdh->dfd : -33 );
    }


    DUF_TRACE( fill, 1, "pathid=%llu; nlist=%d", pathid, nlist );
    if ( nlist >= 0 )
    {
      for ( int il = 0; il < nlist; il++ )
      {
        {
          unsigned long long itemid = 0;


          DUF_TRACE( fill, 1, "pathid=%llu; entry='%s'", pathid, list[il]->d_name );
          if ( noopenat )
            itemid = duf_fill_rfl_flt_uni( real_path, list[il], pathid, pdi, &r );
          else
            itemid = duf_fill_dfl_flt_uni( pdh, list[il], pathid, pdi, &r );
          DUF_TEST_R( r );
          if ( itemid )
            items++;
        }
        if ( list[il] )
          free( list[il] );
      }
      if ( list )
        free( list );
      /* fprintf( stderr, "Update path entries %s done\n", real_path ); */


      /* if ( r >= 0 ) */
      /*   r = items;  */


      DUF_TEST_R( r );
    }
    else
    {
      r = DUF_ERROR_SCANDIR;
      DUF_TEST_R( r );
    }
  }

  if ( noopenat )
    duf_close_dh( &dh );
  mas_free( real_path );
  DUF_TEST_R( r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, items );
  return r;
}

/* static int                                                                                                   */
/* duf_fill_entries_filter_uni( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi ) */
/* {                                                                                                            */
/*   return duf_fill_ent_flt_uni( pathid, pdh, pdi );                                                           */
/* }                                                                                                            */

/* to replace duf_update_pathid_down_filter */
/* duf_fill_pi_flt_uni:
 * update (collected below) information for path
 * */
/* static int                                                                                                         */
/* duf_fill_pi_flt_uni( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi )               */
/* {                                                                                                                  */
/*   int r = 0;                                                                                                       */
/*                                                                                                                    */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                    */
/*                                                                                                                    */
/*   DUF_TRACE( fill, 1, "pathid=%llu\n", pathid );                                                                   */
/*   r = duf_fill_entries_filter_uni( pathid, pdh, pdi );                                                             */
/*                                                                                                                    */
/*   DUF_TRACE( fill, 1, "pathid=%llu; items=%u\n", pathid, r );                                                      */
/*   (* if ( r >= 0 )                                                                                              *) */
/*   (* {                                                                                                          *) */
/*   (*   duf_sql( "UPDATE duf_paths " " SET items='%u', last_updated=datetime() " " WHERE id='%lu'", r, pathid ); *) */
/*   (*   (* TODO group is really TAG *)                                                                           *) */
/*   (*   r = duf_pathid_group( "updated", pathid, +1 );                                                           *) */
/*   (* }                                                                                                          *) */
/*   duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );                                                           */
/*   return r;                                                                                                        */
/* }                                                                                                                  */

/* to replace duf_update_pathid_down_filter */
/* duf_fill_pi_flt_uni:
 * update (collected below) information for path
 * */

/* static int                                                                                                  */
/* duf_fill_pathid_filter_uni( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi ) */
/* {                                                                                                           */
/*   return duf_fill_pi_flt_uni( pathid, pdh, pdi );                                                           */
/* }                                                                                                           */


/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_fill_uni( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( pdh )
    DUF_TRACE( fill, 0, "pdh:%p", ( void * ) pdh );
  DUF_TRACE( fill, 1, "pathid=%llu", pathid );
  if ( pathid != pdi->levinfo[pdi->depth].dirid )
  {
    DUF_ERROR( "@@@@@@@@@@@@@@ %llu -- %llu", pathid, pdi->levinfo[pdi->depth].dirid );
  }


  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );

    DUF_TRACE( fill, 1, "path=%s", path );

/* TODO additionally 
 * for each direntry:
 *  -- 
 * */

/* stat */
    if ( !pdi->u.noself_dir )
    {
      /* pathid = duf_update_realpath_self_up( real_path, NULL, !pfilter->u.noupper_dirs ); */
    }
/* duf_fill_pathid_filter_uni:
 * update (collected below) information for path
 * */
    {
      DUF_SFIELD( dfname );
      DUF_TRACE( fill, 1, "pathid=%llu; scandir dfname:[%s]", pathid, dfname );
      /* r = duf_fill_pathid_filter_uni( pathid, pdh, pdi ); */
      /* r = duf_fill_pi_flt_uni( pathid, pdh, pdi ); */
      /* r = duf_fill_entries_filter_uni( pathid, pdh, pdi ); */
      r = duf_fill_ent_flt_uni( pathid, pdh, pdi, dfname );
    }


    DUF_TRACE( fill, 1, "path=%s", path );
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}




static char *final_sql[] = {
  "UPDATE duf_md5 set dupcnt=(SELECT count(*) " " FROM duf_filedatas as fd " " JOIN duf_md5 as md ON (fd.md5id=md.id) "
        "              WHERE duf_md5.md5sum1=md.md5sum1 and duf_md5.md5sum2=md.md5sum2)",
  NULL,
};


duf_scan_callbacks_t duf_fill_callbacks = {
  .title = __FILE__,.init_scan = NULL,
  .directory_scan_before = duf_directory_scan_fill_uni,
  .file_scan = duf_file_scan_fill_uni,
  /* filename for debug only */
  .fieldset = "duf_filenames.pathid as dirid, " " duf_filenames.name as filename, duf_filedatas.size as filesize "
        " , uid, gid, nlink, inode, mtim as mtime "
        " , duf_filedatas.mode as filemode " " , duf_filenames.id as filenameid" " , md.dupcnt as nsame, md.md5sum1, md.md5sum2 ",
  .file_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 as md on (md.id=duf_filedatas.md5id)"
        "    WHERE "
        "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "
        "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) " "       AND duf_filenames.pathid='%llu' ",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname, duf_paths.items, duf_paths.parentid "
        " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "
        " ,(SELECT count(*) FROM duf_filenames as sfn "
        "          JOIN duf_filedatas as sfd ON (sfn.dataid=sfd.id) "
        "          JOIN duf_md5 as smd ON (sfd.md5id=smd.id) "
        "          WHERE sfn.pathid=duf_paths.id "
        "              AND   sfd.size >= %llu AND sfd.size < %llu "
        "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "
        " ) as nfiles "
        " ,(SELECT min(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) "
        "           WHERE sfn.pathid=duf_paths.id) as minsize "
        " ,(SELECT max(sfd.size) FROM duf_filedatas as sfd JOIN duf_filenames as sfn ON (sfn.dataid=sfd.id) "
        "           WHERE sfn.pathid=duf_paths.id) as maxsize " " FROM duf_paths " " WHERE duf_paths.parentid='%llu' ",
  .final_sql_argv = final_sql,
};
