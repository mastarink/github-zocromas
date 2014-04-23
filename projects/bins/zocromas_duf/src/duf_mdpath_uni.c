#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_path.h"
#include "duf_file.h"

#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_mdpath_uni.h" */
/* ###################################################################### */

/* run  --db-name=test`datem`.db  --uni-scan --fill --mdpath -dfR  /mnt/new_media/media/down/  --trace-mdpath=1 --trace-new=1 --trace-stdout */


/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_mdpath_uni( void *str_cb_udata, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( pdi->depth <= 0 )
  {
    DUF_ERROR( "depth should not be %d at this point", pdi->depth );
    assert( pdi->depth > 0 );
  }
  if ( pdi->depth > 0 )
  {
    {
      DUF_UFIELD( filenameid );
      char *fpath = filenameid_to_filepath( filenameid, pdi, &r );

      DUF_TRACE( mdpath, 2, "#%4llu: mdpath fpath=%s", pdi->seq, fpath );

      mas_free( fpath );
    }
    {
      MD5_CTX *pctx;

      /* unsigned long long pathid; */

      /* pathid = va_arg( args, unsigned long long );                                                */
      /* fprintf( stderr, "(duf_sel_cb_update_mdpaths_path) parthid:%lld / %s\n", pathid, precord->presult[0] ); */
      pctx = ( MD5_CTX * ) pdi->levinfo[pdi->depth - 1].context;
      DUF_TRACE( mdpath, 1, "(%p) L%u context=%p", ( void * ) pdi, pdi->depth, pdi->levinfo[pdi->depth].context );
      if ( precord->presult[5] && precord->presult[6] )
      {
        DUF_UFIELD( md5sum1 );
        DUF_UFIELD( md5sum2 );

        /* fprintf( stderr, "%lld:%lld\n", md5s1, md5s2 ); */
        MD5_Update( pctx, &md5sum1, sizeof( md5sum1 ) );
        MD5_Update( pctx, &md5sum2, sizeof( md5sum2 ) );
      }

    }
    DUF_TRACE( mdpath, 1, "(%p) L%u context=%p", ( void * ) pdi, pdi->depth, pdi->levinfo[pdi->depth].context );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_mdpath_uni_before( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi,
                                      duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    DUF_TRACE( mdpath, 1, "#%4llu: BEFORE dPATH=%s", pdi->seq, path );
    mas_free( path );
  }


  MD5_CTX *pctx;

  pctx = mas_malloc( sizeof( MD5_CTX ) );
  memset( pctx, 0, sizeof( MD5_CTX ) );
  MD5_Init( pctx );
  pdi->levinfo[pdi->depth].context = pctx;

  DUF_TRACE( mdpath, 1, "(%p) L%u context=%p", ( void * ) pdi, pdi->depth, pdi->levinfo[pdi->depth].context );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/*
 * this is callback of type: duf_scan_hook_dir_t (second range; ; sel_cb): 
 * */
static int
duf_sel_cb_insert_mdpath_uni( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb,
                              void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  int r = 0;
  unsigned long long *presmd;

  presmd = ( unsigned long long * ) sel_cb_udata;
  *presmd = strtoll( precord->presult[0], NULL, 10 );
  r = duf_sql( "UPDATE duf_mdpath SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", ( int * ) NULL, *presmd );
  return r;
}

static unsigned long long
duf_insert_mdpath_uni( unsigned long long *md64 )
{
  unsigned long long resmd = DUF_ERROR_INSERT_MDPATH;
  int r = 0;

  r = duf_sql_c( "INSERT INTO duf_mdpath (mdpathsum1,mdpathsum2,ucnt,now) values ('%lld','%lld',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, ( int * ) NULL, md64[1], md64[0] );
  if ( r == DUF_SQL_CONSTRAINT )
  {
    r = duf_sql_select( duf_sel_cb_insert_mdpath_uni, &resmd, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT id as md5id " " FROM duf_mdpath " " WHERE mdpathsum1='%lld' and mdpathsum2='%lld'", md64[1], md64[0] );
  }
  else if ( !r /* assume SQLITE_OK */  )
    resmd = duf_sql_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_mdpath %d\n", r );

  return resmd;
}

static int
duf_directory_scan_mdpath_uni_after( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  {
    char *path = duf_pathid_to_path_s( pathid, pdi, &r );


    DUF_TRACE( mdpath, 1, "#%4llu: mdpath AFTER dPATH=%s", pdi->seq, path );
    mas_free( path );
  }

  MD5_CTX *pctx = pdi->levinfo[pdi->depth].context;
  unsigned long long *md64 = NULL;

  DUF_TRACE( mdpath, 1, "(%p) L%u context=%p", ( void * ) pdi, pdi->depth, pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;
  if ( pctx )
  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    MD5_Final( md, pctx );
    md64 = ( unsigned long long * ) md;
  }
  unsigned long long mdpathid = duf_insert_mdpath_uni( md64 );

  DUF_TRACE( mdpath, 0, "[%016llx%016llx] UPDATE duf_paths " " SET md5dir1='%lld', md5dir2='%lld', mdpathid='%lld' " " WHERE id='%llu'",
             md64[1], md64[0], md64[1], md64[0], mdpathid, pathid );

  duf_sql( "UPDATE duf_paths " " SET md5dir1='%lld', md5dir2='%lld', mdpathid='%lld' " " WHERE id='%lu'", ( int * ) NULL, md64[1], md64[0],
           mdpathid, pathid );


  mas_free( pctx );
  DUF_TRACE( mdpath, 1, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  mas_free( pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}


duf_scan_callbacks_t duf_fill_mdpath_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_mdpath_uni_before,
  .directory_scan_after = duf_directory_scan_mdpath_uni_after,
  .file_scan = duf_file_scan_mdpath_uni,
  .fieldset =
        " duf_filenames.pathid as dirid " " ,duf_filenames.name as filename, duf_filedatas.size as filesize"
        " , uid, gid, nlink, inode, mtim as mtime " " , dupcnt as nsame"
        " , duf_filenames.id as filenameid" " , duf_filedatas.mode as filemode, md.md5sum1, md.md5sum2",
  .file_selector =
        "SELECT %s FROM duf_filenames "
        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 as md on (md.id=duf_filedatas.md5id)"
        "    WHERE "
        "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "
        "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) "
        "       AND duf_filenames.pathid='%llu' " " ORDER BY duf_filenames.name ",
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
  /* .final_sql_argv = final_sql, */
};
