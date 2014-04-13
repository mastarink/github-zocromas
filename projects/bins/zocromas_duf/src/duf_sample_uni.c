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
#include "duf_sample_uni.h"
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
static int
duf_sample_file_uni( unsigned long long pathid, unsigned long long filenameid,
                     const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --sample   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */
  fprintf( stderr, "sample file: %s\n", name );

  if ( duf_config->cli.trace.scan )
  {
    fprintf( stderr, "[SAMP ] %20s: %s\n", __func__, name );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
static int
duf_sample_dir_uni( unsigned long long pathid, const char *name, unsigned long long items, duf_dirinfo_t * pdi,
                    struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( duf_config->cli.trace.scan > 1 )
    fprintf( stderr, "[SAMP ] %20s: pathid=%llu\n", __func__, pathid );
  {
    char *path = duf_pathid_to_path( pathid );

    fprintf( stderr, "sample path: %s\n", path );

    if ( duf_config->cli.trace.scan > 1 )
      fprintf( stderr, "[SAMP ] %20s: path=%s\n", __func__, path );

    mas_free( path );
  }
  return 0;
}

duf_scan_callbacks_t duf_sample_callbacks = {
  .fieldset="duf_filenames.pathid, duf_filenames.name as filename",
  .init_scan = NULL,
  .directory_scan = duf_sample_dir_uni,
  .file_scan = duf_sample_file_uni,
};
