#ifndef MAS_DUF_LI_H
# define MAS_DUF_LI_H

#include "duf_defs.h"

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ♠ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ♠ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ♠ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

# ifndef  MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# elif defined( DUF_DO_NUMS)
void duf_li_set_nums( duf_levinfo_t * pli, unsigned long long ndirs, unsigned long long nfiles );
# else
#  if 0
void duf_li_set_childs( duf_levinfo_t * pli, unsigned long long childs );
#  endif
# endif

int duf_li_calc_depth( const duf_levinfo_t * pli );
char *duf_li_path( const duf_levinfo_t * pli, int count );

void duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );

int duf_nameid2li_existed( duf_depthinfo_t * pdi, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid );
int duf_dirid2li_existed( duf_depthinfo_t * pdi, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid );

#endif
