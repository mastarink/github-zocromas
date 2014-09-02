#ifndef DUF_PDI_H
#  define DUF_PDI_H

#  include "duf_levinfo_types.h"

int duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int ifadd );
int duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int ifadd );

int duf_pdi_close( duf_depthinfo_t * pdi );

int duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od );

void duf_clear_context( duf_context_t * pcontext );
void *duf_context( duf_context_t * pcontext );
void duf_set_context( duf_context_t * pcontext, void *ptr );
void duf_set_context_destructor( duf_context_t * pcontext, duf_void_voidp_t destr );

void duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr );
void *duf_pdi_context( duf_depthinfo_t * pdi );

int duf_pdi_max_filter( const duf_depthinfo_t * pdi );

int duf_pdi_seq( const duf_depthinfo_t * pdi );

int duf_pdi_depth( const duf_depthinfo_t * pdi );

void duf_pdi_set_topdepth( duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );

int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d );

int duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex );
int duf_pdi_finalize( duf_depthinfo_t * pdi, int i );
duf_sqlite_stmt_t *duf_pdi_statement( duf_depthinfo_t * pdi, int i );

void duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes );

#endif
