#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_config.h"

#include "duf_path.h"

#include "duf_sql.h"
#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_print_md5_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
static int
duf_file_scan_print_md5_uni( void *str_cb_udata, duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --print --md5  --files   -FF
   *                   ^^^^^^^        ^^^^^^^
   * */


  printf( "%6llu [%10llu] file: %s\n", pdi->seq, filesize, filename );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_directory_scan_print_md5_uni( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi,
                                  struct duf_scan_callbacks_s *sccb, duf_record_t * precord )
{
  DUF_SFIELD( dirname );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  {
    /* char *path = duf_pathid_to_path( pathid ); */

    /* fprintf( stderr, "print_md5 path: %s\n", path ); */
    printf( "------------------------------------- md5: %s\n", dirname );


    /* mas_free( path ); */
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

duf_scan_callbacks_t duf_print_md5_callbacks = {
  .title = __FILE__,
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_print_md5_uni,
  .file_scan = duf_file_scan_print_md5_uni,
  .fieldset =
        "duf_md5.id as dirid, "
        " duf_filenames.name as filename, duf_filenames.pathid as hid, "
        "printf('%016x%016x',md5sum1,md5sum2) as dirname, duf_filedatas.size as filesize ",
  .file_selector = "SELECT %s FROM duf_filenames "
        " LEFT JOIN duf_filedatas ON (duf_filedatas.id=duf_filenames.dataid) "
        " LEFT JOIN  duf_md5  ON (duf_filedatas.md5id=duf_md5.id)" " WHERE duf_filedatas.md5id='%llu'",
  .dir_selector =
        "SELECT duf_md5.id as dirid, printf('%%016x%%016x',md5sum1,md5sum2) as dirname "
	" ,0 as ndirs"
        " ,(SELECT count(*) FROM duf_filenames as subfilenames "
        "           LEFT JOIN duf_filedatas ON (duf_filedatas.id=subfilenames.dataid) "
        "                            WHERE duf_filedatas.md5id=duf_md5.id) as nfiles "
        " FROM duf_md5 "
        " WHERE %llu<1 ORDER BY md5sum1,md5sum2 "
        ,
};

