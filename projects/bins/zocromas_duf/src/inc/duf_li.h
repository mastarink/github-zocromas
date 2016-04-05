#ifndef MAS_DUF_LI_H
# define MAS_DUF_LI_H

# include "duf_defs.h"

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* int duf_li_calc_depth( const duf_levinfo_t * pli ); */
DRX( LI, int, d, -1, li_calc_depth, const duf_levinfo_t * pli );

/* char *duf_li_path( const duf_levinfo_t * pli, int count ); */
DRX( LI, char *, path, NULL, li_path, const duf_levinfo_t * pli, int count );

/* void duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d ); */
DRN( LI, void, li_dbinit, duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d );

/* int duf_nameid2li_existed( duf_depthinfo_t * pdi, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid ); */
DR( LI, nameid2li_existed, duf_depthinfo_t * pditemp, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid );

/* int duf_dirid2li_existed( duf_depthinfo_t * pdi, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid ); */
DR( LI, dirid2li_existed, duf_depthinfo_t * pditemp, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid );

/* const char *duf_li_itemname( duf_levinfo_t * pli ); */
DRX( LI, const char *, itemname, NULL, li_itemname, duf_levinfo_t * pli );

#endif
