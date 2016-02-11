#ifndef MAS_DUF_SCCB_HANDLE_H
#  define MAS_DUF_SCCB_HANDLE_H

duf_sccb_handle_t *duf_sccb_handle_create( void );


duf_sccb_handle_t *duf_sccb_handle_open( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc,
                                         char *const *targv /*, const duf_ufilter_t * pu */ , int *pr );
int duf_sccb_handle_close( duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );

const duf_sql_set_t *duf_sccbh_get_sql_set_f( duf_sccb_handle_t * sccbh, duf_node_type_t node_type );


#endif

/*
vi: ft=c
*/
