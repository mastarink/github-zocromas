#ifndef MAS_DUF_PDI_STMT_H
#  define MAS_DUF_PDI_STMT_H

#  include "duf_levinfo_types.h"


duf_stmnt_t *duf_pdi_find_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /*, const int *pindex  */  );

duf_stmnt_t *duf_pdi_prepare_statement_by_id( duf_depthinfo_t * pdi, const char *sql, duf_stmt_ident_t stmtid /*, const int *pindex */ , int *pr );
int duf_pdi_finalize_idstmt( duf_depthinfo_t * pdi, int i );
int duf_pdi_finalize_statement_by_id( duf_depthinfo_t * pdi, duf_stmt_ident_t stmtid /* const int *pindex */  );
int duf_pdi_finalize_statement_by_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt );


#endif
