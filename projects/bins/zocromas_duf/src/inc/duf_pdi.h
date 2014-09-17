#ifndef DUF_PDI_H
#  define DUF_PDI_H

#  include "duf_levinfo_types.h"

int duf_pdi_init( duf_depthinfo_t * pdi, const char *real_path, int ifadd, int recursive );
int duf_pdi_init_wrap( duf_depthinfo_t * pdi, const char *real_path, int ifadd, int recursive );
int duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu, int recursive );
int duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, int recursive );
int duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, int recursive );
int duf_pdi_close( duf_depthinfo_t * pdi );

int duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od );

int duf_pdi_max_filter( const duf_depthinfo_t * pdi );

void duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr );
void *duf_pdi_context( duf_depthinfo_t * pdi );

int duf_pdi_seq( const duf_depthinfo_t * pdi );

int duf_pdi_depth( const duf_depthinfo_t * pdi );

void duf_pdi_set_topdepth( duf_depthinfo_t * pdi );
int duf_pdi_topdepth( const duf_depthinfo_t * pdi );
int duf_pdi_maxdepth( const duf_depthinfo_t * pdi );

int duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d );
int duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta );

int duf_pdi_reldepth( const duf_depthinfo_t * pdi );
int duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d );
int duf_pdi_recursive( const duf_depthinfo_t * pdi );

duf_sqlite_stmt_t *duf_pdi_prepare_statement( duf_depthinfo_t * pdi, const char *sql, int *pindex, int *pr );
int duf_pdi_finalize_idstmt( duf_depthinfo_t * pdi, int i );
duf_sqlite_stmt_t *duf_pdi_find_statement( duf_depthinfo_t * pdi, int *pindex );
int duf_pdi_finalize_statement( duf_depthinfo_t * pdi, int *pindex );

void duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes );

#endif
