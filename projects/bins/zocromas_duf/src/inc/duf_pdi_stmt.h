#ifndef MAS_DUF_PDI_STMT_H
# define MAS_DUF_PDI_STMT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_levinfo_stmt_types.h"                                 /* duf_stmt_ident_t; duf_idstmt_t; ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

duf_stmnt_t *duf_pdi_find_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /*, const int *pindex  */  );

/* duf_stmnt_t *duf_pdi_prepare_statement_by_id( duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid (*, const int *pindex *) , int *pr ); */
DRP( PDI, duf_stmnt_t *, pstmt_arg, NULL, pdi_prepare_statement_by_id, duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid );
DR( PDI, pdi_finalize_idstmt, duf_depthinfo_t * pdi, int i );

DR( PDI, pdi_finalize_statement_by_id, duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  );
DR( PDI, pdi_finalize_statement_by_stmt, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg );

#endif
