#ifndef DUF_PDI_CREDEL_H
#  define DUF_PDI_CREDEL_H

#  include "duf_levinfo_types.h"

duf_depthinfo_t *duf_pdi_create( const char *name );
void duf_pdi_delete( duf_depthinfo_t * pdi );
void duf_pdi_kill( duf_depthinfo_t ** ppdi );

duf_depthinfo_t *duf_pdi_clone( duf_depthinfo_t * pdisrc );
void duf_pdi_copy( duf_depthinfo_t * pdidst, duf_depthinfo_t * pdisrc );

#endif
