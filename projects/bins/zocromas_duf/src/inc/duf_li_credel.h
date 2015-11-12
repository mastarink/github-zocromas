#ifndef MAS_DUF_LI_CREDEL_H
#  define MAS_DUF_LI_CREDEL_H

void duf_li_clear( duf_levinfo_t * pli );
void duf_li_clear_n( duf_levinfo_t * pli, unsigned maxcount );
/* void duf_li_delete( duf_levinfo_t * pli, unsigned maxcount ); */

duf_levinfo_t *duf_li_create( int maxcount );
duf_levinfo_t *duf_li_clone( const duf_levinfo_t * lisrc, unsigned maxcount );

duf_levinfo_t *duf_dirid2li( unsigned long long dirid, const char *itemname, unsigned maxcount, int *pr );
duf_levinfo_t *duf_nameid2li( unsigned long long dirid, unsigned maxcount, int *pr );

#endif
