#ifndef MAS_DUF_LEVINFO_CREDEL_H
# define MAS_DUF_LEVINFO_CREDEL_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ♠ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */

int duf_levinfo_set_li( duf_depthinfo_t * pdi, duf_levinfo_t * pli, size_t count );

int duf_levinfo_create( duf_depthinfo_t * pdi );
int duf_levinfo_delete( duf_depthinfo_t * pdi );

#endif
