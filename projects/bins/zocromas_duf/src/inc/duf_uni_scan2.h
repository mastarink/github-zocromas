#ifndef MAS_DUF_UNI_SCAN2_H
#  define MAS_DUF_UNI_SCAN2_H

#  include "duf_hook_types.h"
/* int duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb ); */


/*
 * if recursive, call duf_str_cb(1?)_uni_scan_dir + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 * */
int duf_str_cb2_uni_scan_dir( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );

#endif
