#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */

#include "duf_types.h"
#include "duf_config.h"

#include "duf_utils.h"

#include "duf_path.h"
#include "duf_file_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_pathid.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid (i.e. children of pathid) with corresponding args
 * */
static int
duf_scan_files_by_di( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                      const duf_dirhandle_t * pdhu )
{
  int r = DUF_ERROR_NO_FILE_SELECTOR;

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  {
    if ( sccb && sccb->file_selector )
      r = duf_scan_items_sql( DUF_NODE_LEAF, str_cb, pdi, pdi, sccb, pdhu, sccb->file_selector, /* ... */ sccb->fieldset, pdi->u.minsize,
                              pdi->u.maxsize ? pdi->u.maxsize : ( unsigned long long ) -1, pdi->u.minsame,
                              pdi->u.maxsame ? pdi->u.maxsame : ( unsigned long long ) -1, dirid );
    else
      DUF_ERROR( "sccb->file_selector must be set for %s", sccb->title );
  }
  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid with corresponding args
 * */
int
duf_scan_files_by_dirid( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                         const duf_dirhandle_t * pdhu )
{
  return ( duf_config->cli.act.files ) ? duf_scan_files_by_di( dirid, str_cb, pdi, sccb, pdhu ) : 0;
}
