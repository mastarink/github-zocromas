#ifndef MAS_DUF_LI_H
#  define MAS_DUF_LI_H
#  include "duf_levinfo_types.h"


#  ifndef DUF_NO_NUMS
void duf_li_set_nums( duf_levinfo_t * pli, unsigned long long ndirs, unsigned long long nfiles );
#  else
#    if 0
void duf_li_set_childs( duf_levinfo_t * pli, unsigned long long childs );
#    endif
#  endif

int duf_li_count( const duf_levinfo_t * pli );
char *duf_li_path( const duf_levinfo_t * pli, int count );

#endif
