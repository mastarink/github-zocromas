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
#include "duf_update_pathid.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_dir_fill_uni.h"
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
int
duf_sql_uni_scan_fill_files_uni( unsigned long long pathid, unsigned long long filenameid,
                                 const char *name, void *str_cb_udata, duf_dirinfo_t * pdi,
                                 struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */
  printf( " FILL > %s\n", precord->presult[2] );
  duf_dbgfunc( DBG_END, __func__, __LINE__ );


/* static int                                                                                                                        */
/* duf_sql_update_filedatas(  duf_record_t * precord, va_list args, void *sel_cb_udata,                          */
/*                           duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb ) */

  printf( "/FILL > %s\n", precord->presult[2] );
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
int
duf_sql_scan_fill_dir_uni( unsigned long long pathid, const char *name, unsigned long long items, duf_dirinfo_t * pdi,
                           struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( pdi && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) )
  {
    char *path = duf_pathid_to_path( pathid );

    printf( " FILL %s\n", path );
/* stat */
    if ( !pdi->u.noself_dir )
    {
      /* pathid = duf_update_realpath_self_up( real_path, NULL, !pfilter->u.noupper_dirs ); */
    }
    duf_fill_pathid_filter_uni( pathid, pdi );
    printf( "/FILL %s\n", path );
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}
