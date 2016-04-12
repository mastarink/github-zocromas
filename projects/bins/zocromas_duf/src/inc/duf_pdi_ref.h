#ifndef MAS_DUF_PDI_REF_H
# define MAS_DUF_PDI_REF_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

duf_depthinfo_t *duf_pdi_root( duf_depthinfo_t * pdi );

int duf_pdi_recursive( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_allow_dirs( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_linear( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_opendir( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
int duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od );

void duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes );

const char *duf_pdi_name( const duf_depthinfo_t * pdi );

unsigned long long duf_pdi_seq( const duf_depthinfo_t * pdi );
unsigned long long duf_pdi_seq_leaf( const duf_depthinfo_t * pdi );
void duf_pdi_seq_leaf_plus( duf_depthinfo_t * pdi );
unsigned long long duf_pdi_seq_node( const duf_depthinfo_t * pdi );
void duf_pdi_seq_node_plus( duf_depthinfo_t * pdi );

void duf_pdi_set_each_stmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg );
duf_stmnt_t *duf_pdi_each_stmt( const duf_depthinfo_t * pdi, int up );

#endif
