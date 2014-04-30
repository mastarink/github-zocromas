#ifndef DUF_PDI_H
#  define DUF_PDI_H


int duf_pdi_max_filter( const duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );
int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d );


#endif
