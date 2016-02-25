#ifndef MAS_DUF_PDI_CONTEXT_H
# define MAS_DUF_PDI_CONTEXT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_fun_types.h"                                          /* duf_void_voidp_func_t etc. etc. ... ♠ */

void duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_func_t destr );
void *duf_pdi_context( const duf_depthinfo_t * pdi );

#endif
