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

int duf_li_calc_depth( const duf_levinfo_t * pli );
char *duf_li_path( const duf_levinfo_t * pli, int count );

void duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );

int duf_nameid2li_existed( duf_depthinfo_t * pdi, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid );
int duf_dirid2li_existed( duf_depthinfo_t * pdi, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid );



#endif
