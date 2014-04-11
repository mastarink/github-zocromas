#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

/* #include "duf_sql.h" */
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"

#include "duf_dbg.h"

#include "duf_dir_scan.h"

/* ###################################################################### */
#include "duf_dir_scan_uni.h"
/* ###################################################################### */



/* callback of type duf_scan_callback_dir_t */
int
duf_test_scan_directory_cb( unsigned long long pathid, const char *name, unsigned long long items,
                            duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  printf( "[%s]\n", name );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_test_scan_file_cb( unsigned long long pathid, unsigned long long filenameid, const char *name,
                       void *str_cb_udata, duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  char *path = duf_pathid_to_path( pathid );

  printf( ">>>> %s %s\n", path, name );
  mas_free( path );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

duf_scan_callbacks_t def_scan_callbacks = {
  .directory_scan = duf_test_scan_directory_cb,
  .file_scan = duf_test_scan_file_cb,
};
