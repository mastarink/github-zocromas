#ifndef DUF_PDI_STMT_H
#  define DUF_PDI_STMT_H

#  include "duf_levinfo_types.h"



duf_stmnt_t *duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex, int *pr );
int duf_pdi_finalize_idstmt( duf_depthinfo_t * pdi, int i );
duf_stmnt_t *duf_pdi_find_statement( duf_depthinfo_t * pdi, int *pindex );
int duf_pdi_finalize_statement( duf_depthinfo_t * pdi, int *pindex );


#endif

