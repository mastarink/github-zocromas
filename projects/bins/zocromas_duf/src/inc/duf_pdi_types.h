#ifndef MAS_DUF_PDI_TYPES_H
#  define MAS_DUF_PDI_TYPES_H

typedef int ( *duf_pdi_cb_t ) ( duf_depthinfo_t * pdi );
typedef int ( *duf_pdi_scb_t ) ( char *pbuffer, size_t bfsz, duf_depthinfo_t * pdi, size_t *pwidth );

#endif

/*
vi: ft=c
*/
