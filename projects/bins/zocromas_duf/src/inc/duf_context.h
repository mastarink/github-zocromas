#ifndef MAS_DUF_CONTEXT_H
# define MAS_DUF_CONTEXT_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_fun_types.h"                                          /* duf_void_voidp_func_t etc. etc. ... ✗ */

void duf_clear_context( duf_pdi_context_t * pcontext );
void *duf_context( const duf_pdi_context_t * pcontext );
void duf_set_context( duf_pdi_context_t * pcontext, void *ptr );
void duf_set_context_destructor( duf_pdi_context_t * pcontext, duf_void_voidp_func_t destr );

#endif
