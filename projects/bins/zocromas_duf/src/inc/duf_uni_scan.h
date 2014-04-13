#ifndef MAS_DUF_UNI_SCAN_H
#  define MAS_DUF_UNI_SCAN_H

int duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb );

int duf_uni_scan_targ( duf_config_t * config, duf_scan_callbacks_t * sccb );

int duf_uni_scan_all( duf_config_t * config );

/*
 * if recursive, call duf_uni_scan_dir + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 * */
int duf_uni_scan_dir( unsigned long long pathid, unsigned long long filenameid, const char *name, void *str_cb_udata,
                          duf_dirinfo_t * pdi1, duf_scan_callbacks_t * sccb, duf_record_t * precord );

#endif
