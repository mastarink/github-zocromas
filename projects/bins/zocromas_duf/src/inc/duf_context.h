#ifndef DUF_CONTEXT_H
#  define DUF_CONTEXT_H

void duf_clear_context( duf_levinfo_context_t * pcontext );
void *duf_context( const duf_levinfo_context_t * pcontext );
void duf_set_context( duf_levinfo_context_t * pcontext, void *ptr );
void duf_set_context_destructor( duf_levinfo_context_t * pcontext, duf_void_voidp_t destr );


#endif
