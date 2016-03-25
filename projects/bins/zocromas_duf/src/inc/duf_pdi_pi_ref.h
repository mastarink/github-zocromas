#ifndef MAS_DUF_PDI_PI_REF_H
# define MAS_DUF_PDI_PI_REF_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */

int duf_pdi_depth( const duf_depthinfo_t * pdi );
int duf_pdi_reldepth_d( const duf_depthinfo_t * pdi, int d );
int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );
void duf_pdi_set_topdepth( duf_depthinfo_t * pdi );
int duf_pdi_maxdepth( const duf_depthinfo_t * pdi );
duf_levinfo_t *duf_pdi_levinfo( const duf_depthinfo_t * pdi );

#endif
