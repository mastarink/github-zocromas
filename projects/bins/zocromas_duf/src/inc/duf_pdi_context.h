#ifndef DUF_PDI_CONTEXT_H
#  define DUF_PDI_CONTEXT_H

#  include "duf_levinfo_types.h"

void duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr );
void *duf_pdi_context( const duf_depthinfo_t * pdi );


#endif
