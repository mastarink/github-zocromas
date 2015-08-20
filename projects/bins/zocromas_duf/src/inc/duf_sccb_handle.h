#ifndef MAS_DUF_SCCB_HANDLE_H
#  define MAS_DUF_SCCB_HANDLE_H



duf_sccb_handle_t *duf_open_sccb_handle( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv,
                                         const duf_ufilter_t * pu, int *pr );
int duf_close_sccb_handle( duf_sccb_handle_t * sccbh );



#endif

/*
vi: ft=c
*/
