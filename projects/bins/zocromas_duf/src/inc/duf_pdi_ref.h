#ifndef MAS_DUF_PDI_REF_H
#  define MAS_DUF_PDI_REF_H

#  include "duf_levinfo_types.h"

duf_depthinfo_t *duf_pdi_root( duf_depthinfo_t * pdi );

const duf_ufilter_t *duf_pdi_pu( const duf_depthinfo_t * pdi );
const duf_yfilter_t *duf_pdi_py( const duf_depthinfo_t * pdi );

int duf_pdi_max_filter( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_seq( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );






int duf_pdi_recursive( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_allow_dirs( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_linear( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_opendir( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od );



int duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta ) __attribute__ ( ( warn_unused_result ) );


void duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes );



#endif
