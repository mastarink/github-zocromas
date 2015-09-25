#ifndef MAS_DUF_UFILTER_H
#  define MAS_DUF_UFILTER_H


duf_ufilter_t *duf_ufilter_create( void );
void duf_ufilter_delete( duf_ufilter_t * pu );
duf_ufilter_t *duf_ufilter_clone( const duf_ufilter_t * pu );


/* duf_ufilter_t *duf_ufilter_create_from( const duf_ufilter_t * pusrc ); */

#endif
