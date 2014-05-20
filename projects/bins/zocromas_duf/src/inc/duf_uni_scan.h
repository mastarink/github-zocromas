#ifndef MAS_DUF_UNI_SCAN_H
#  define MAS_DUF_UNI_SCAN_H

#include "duf_hook_types.h"
/* int duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb ); */

int duf_uni_scan_targ( duf_scan_callbacks_t * sccb );

int duf_uni_scan_all( void );

/*
 * if recursive, call duf_str_cb_uni_scan_dir + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 * */
int duf_str_cb_uni_scan_dir( void *str_cb_udata, duf_depthinfo_t * pdi1, duf_scan_callbacks_t * sccb,
                      duf_record_t * precord   );

#endif
