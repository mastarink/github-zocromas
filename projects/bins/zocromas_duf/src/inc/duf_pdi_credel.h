#ifndef MAS_DUF_PDI_CREDEL_H
#  define MAS_DUF_PDI_CREDEL_H

#  include "duf_levinfo_types.h"

duf_depthinfo_t *duf_pdi_create( const char *name );
int duf_pdi_delete( duf_depthinfo_t * pdi );
int duf_pdi_kill( duf_depthinfo_t ** ppdi );

duf_depthinfo_t *duf_pdi_clone( duf_depthinfo_t * pdisrc, int no_li );
void duf_pdi_copy( duf_depthinfo_t * pdidst, duf_depthinfo_t * pdisrc, int no_li );

#endif
