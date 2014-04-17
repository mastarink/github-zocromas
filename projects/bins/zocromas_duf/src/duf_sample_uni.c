#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_utils.h"
#include "duf_config.h"

#include "duf_path.h"
#include "duf_file.h"

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_sample_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_sample_uni( void *str_cb_udata, duf_dirinfo_t * pdi,  duf_record_t * precord )
{
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --sample   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */



  {
    DUF_UFIELD( filenameid );
    char *fpath = filenameid_to_filepath( filenameid );

    printf( "#%4llu: sample fpath %s\n", pdi->seq, fpath );

    DUF_TRACE( sample, 1, "fpath=%s", fpath );
    DUF_TRACE_SAMPLE( 1, "fpath=%s", fpath );

    mas_free( fpath );
  }
  DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth - 1].context );

  DUF_TRACE( sample, 1, "filename=%s", filename );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_sample_uni( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi,
                                duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE_SAMPLE( 1, "T1 pathid=%llu", pathid );
  /* DUF_TRACE_SAMPLE( 0, "T0 pathid=%llu", pathid ); */
  {
    char *path = duf_pathid_to_path( pathid );

    printf( "#%4llu: sample BEFORE dPATH %s\n", pdi->seq, path );
    DUF_TRACE_SAMPLE( 1, "path=%s", path );
    mas_free( path );
  }

  {
    char *test = mas_strdup( "HELLO" );

    pdi->levinfo[pdi->depth].context = ( void * ) test;
  }
  DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

static int
duf_directory_scan_sample_uni_after( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi,
                                      duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE_SAMPLE( 2, "T2 pathid=%llu", pathid );
  {
    char *path = duf_pathid_to_path( pathid );

    printf( "#%4llu: sample AFTER dPATH %s\n", pdi->seq, path );
    DUF_TRACE_SAMPLE( 1, "path=%s", path );
    mas_free( path );
  }

  DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, pdi->levinfo[pdi->depth].context );
  mas_free( pdi->levinfo[pdi->depth].context );
  pdi->levinfo[pdi->depth].context = NULL;
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}


duf_scan_callbacks_t duf_sample_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_sample_uni,
  .directory_scan_after = duf_directory_scan_sample_uni_after,
  .file_scan = duf_file_scan_sample_uni,
  .fieldset = "duf_filenames.pathid as dirid " " ,duf_filenames.name as filename, duf_filedatas.size as filesize" " ,duf_filenames.id as filenameid",
  .file_selector =
        "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_filenames.pathid='%llu' ",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.items, duf_paths.parentid "
        " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "
        " ,(SELECT count(*) FROM duf_filenames as subfilenames WHERE subfilenames.pathid=duf_paths.id) as nfiles "
        " FROM duf_paths "
        " WHERE duf_paths.parentid='%llu' "
        ,
};
