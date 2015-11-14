#ifndef MAS_DUF_PDI_DEPTH_H
#  define MAS_DUF_PDI_DEPTH_H

#  include "duf_levinfo_types.h"



int duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta ) __attribute__ ( ( warn_unused_result ) );


#endif
