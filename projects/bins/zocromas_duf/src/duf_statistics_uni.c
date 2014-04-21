#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_path.h"
#include "duf_file.h"

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_statistics_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_statistics_uni( void *str_cb_udata, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --statistics   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */

  {
    DUF_UFIELD( filenameid );
    char *fpath = filenameid_to_filepath( filenameid );

    printf( "#%4llu: statistics fpath %s\n", pdi->seq, fpath );

    DUF_TRACE( statistics, 1, "fpath=%s", fpath );

    mas_free( fpath );
  }


  DUF_TRACE( statistics, 1, "filename=%s", filename );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_statistics_uni( unsigned long long pathid, const duf_dirhandle_t *pdh, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  {
     char *path = duf_pathid_to_path_s( pathid );


    printf( "#%4llu: statistics dPATH %s\n", pdi->seq, path );
    DUF_TRACE( statistics, 1, "path=%s", path );
    mas_free( path );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

duf_scan_callbacks_t duf_statistics_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_statistics_uni,
  .file_scan = duf_file_scan_statistics_uni,
  .fieldset =
        "duf_filenames.pathid as dirid " " ,duf_filenames.name as filename, duf_filedatas.size as filesize"
        " ,duf_filenames.id as filenameid",
  .file_selector =
        "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_filenames.pathid='%llu' ",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname, duf_paths.items, duf_paths.parentid "
        " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "
        " ,(SELECT count(*) FROM duf_filenames as subfilenames WHERE subfilenames.pathid=duf_paths.id) as nfiles " " FROM duf_paths "
        " WHERE duf_paths.parentid='%llu' ",
};
