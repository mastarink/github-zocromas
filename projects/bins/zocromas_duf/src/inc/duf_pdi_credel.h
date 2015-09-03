#ifndef DUF_PDI_CREDEL_H
#  define DUF_PDI_CREDEL_H

#  include "duf_levinfo_types.h"

duf_depthinfo_t *duf_pdi_create( const char *name );
void duf_pdi_delete( duf_depthinfo_t * pdi );
void duf_pdi_kill( duf_depthinfo_t ** ppdi );


#endif
