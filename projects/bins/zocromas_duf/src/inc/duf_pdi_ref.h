#ifndef DUF_PDI_REF_H
#  define DUF_PDI_REF_H

#  include "duf_levinfo_types.h"



int duf_pdi_max_filter( const duf_depthinfo_t * pdi );
int duf_pdi_seq( const duf_depthinfo_t * pdi );

int duf_pdi_depth( const duf_depthinfo_t * pdi );

void duf_pdi_set_topdepth( duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );
int duf_pdi_maxdepth( const duf_depthinfo_t * pdi );

int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d );
int duf_pdi_recursive( const duf_depthinfo_t * pdi );
int duf_pdi_opendir( const duf_depthinfo_t * pdi );
int duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od );



int duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d );
int duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta );


void duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes );



#endif

