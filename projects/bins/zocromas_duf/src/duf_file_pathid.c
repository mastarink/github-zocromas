#include <stdio.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_types.h"
#include "duf_config.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_path.h"
#include "duf_item_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_pathid.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid (i.e. children of pathid) with corresponding args
 * */
static int
duf_scan_files_by_di( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi,
                      duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_off */  )
{
  int r = DUF_ERROR_NO_FILE_SELECTOR;

  DUF_TRACE( scan, 0, "scan files by:%llu", dirid );

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  /* assert( duf_config->cli.noopenat                                                                                         */
  /*         || ( duf_config->nopen - ( pdi->levinfo[pdi->depth].lev_dh.dfd ? 1 : 0 ) - duf_config->nclose == pdi->depth ) ); */
  DUF_OINV( pdi-> );

  if ( sccb && sccb->leaf_selector )
  {
    /* assert( duf_config->cli.noopenat                                                                                         */
    /*         || ( duf_config->nopen - ( pdi->levinfo[pdi->depth].lev_dh.dfd ? 1 : 0 ) - duf_config->nclose == pdi->depth ) ); */
    DUF_OINV( pdi-> );

    DUF_TRACE( scan, 0, "scan leaves by:%llu", dirid );
    r = duf_scan_items_sql( DUF_NODE_LEAF, str_cb, pdi, pdi, sccb /*, pdhu_off */ , sccb->leaf_selector, /* ... */ sccb->fieldset,
                            /*   pdi->u.minsize,                                                            */
                            /* pdi->u.maxsize ? pdi->u.maxsize : ( unsigned long long ) -1, pdi->u.minsame, */
                            /* pdi->u.maxsame ? pdi->u.maxsame : ( unsigned long long ) -1,                 */
                            dirid );
    /* assert( duf_config->cli.noopenat                                                                                         */
    /*         || ( duf_config->nopen - ( pdi->levinfo[pdi->depth].lev_dh.dfd ? 1 : 0 ) - duf_config->nclose == pdi->depth ) ); */
    DUF_OINV( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( "sccb->leaf_selector must be set for %s", sccb->title );
  }

  /* assert( duf_config->cli.noopenat                                                                                         */
  /*         || ( duf_config->nopen - ( pdi->levinfo[pdi->depth].lev_dh.dfd ? 1 : 0 ) - duf_config->nclose == pdi->depth ) ); */
  DUF_OINV( pdi-> );
  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid with corresponding args
 * */
int
duf_scan_files_by_dirid( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi,
                         duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_off */  )
{
  int r = 0;

  if ( duf_config->cli.act.files )
    r = duf_scan_files_by_di( dirid, str_cb, pdi, sccb /*, pdhu_off */  );
  else
    DUF_TRACE( scan, 0, "skip scan leaves by:%llu : no '--files'", dirid );
  return r;
}
