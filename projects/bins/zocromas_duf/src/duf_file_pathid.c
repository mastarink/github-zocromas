#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

/* #include "duf_sql.h" */
#include "duf_utils.h"

#include "duf_path.h"
/* #include "duf_file.h" */
#include "duf_file_scan.h"

#include "duf_dir_scan.h"
#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_pathid.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid (i.e. children of pathid) with corresponding args
 * */
static int
duf_scan_files_by_di( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                      duf_dirhandle_t * pdhu )
{
  int r = 0;

  DUF_TRACE( scan, 0, "L%u >duf_scan_items_sql FIELDSET:%s; str_cb=%p", pdi->depth, sccb->fieldset ? sccb->fieldset : "-none-",
             ( void * ) ( unsigned long long ) str_cb );

  /* DUF_TRACE_SCAN( pdi->depth, ( void * ) ( unsigned long long ) str_cb ); */
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  {
    DUF_TRACE( current, 0, "+dirid: %llu : %d", dirid, pdhu ? ( pdhu->dfd ? 2 : 1 ) : 0 );
    if ( sccb && sccb->file_selector )
      r = duf_scan_items_sql( DUF_NODE_LEAF, str_cb, pdi, pdi, sccb, pdhu, sccb->file_selector, /* ... */ sccb->fieldset, dirid );
    else
    {
      fprintf( stderr, "sccb->file_selector must be set for %s\n", sccb->title );
      exit( 4 );
    }
  }
  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid with corresponding args
 * */
int
duf_scan_files_by_dirid( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                         duf_dirhandle_t * pdhu )
{
  return ( duf_config->cli.act.files ) ? duf_scan_files_by_di( dirid, str_cb, pdi, sccb, pdhu ) : 0;
}
