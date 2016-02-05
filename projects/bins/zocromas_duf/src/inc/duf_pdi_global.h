#ifndef MAS_DUF_PDI_GLOBAL_H
#  define MAS_DUF_PDI_GLOBAL_H




int duf_pdi_init_global( void );

DR( PDI, pdi_create_global, const char *name );
DR( PDI, pdi_kill_global, void );

duf_depthinfo_t *_duf_pdi_global( int *pr );
duf_depthinfo_t *duf_pdi_global( void );

const char *_duf_pdi_global_name( int *pr );
const char *duf_pdi_global_name( void );

const duf_ufilter_t *_duf_pdi_global_ufilter( int *pr );
const duf_ufilter_t *duf_pdi_global_ufilter( void );


#endif
