#ifndef MAS_DUF_DIR_SCAN2_STAGES_H
#  define MAS_DUF_DIR_SCAN2_STAGES_H

#  define DUF_HOOK_AT(_sccb, _typ)    ( ( duf_scan_hook2_dir_t * ) ( ( ( char * ) _sccb ) + ( offsetof( duf_scan_callbacks_t, _typ ) ) ) )
#  define DUF_SCANNER_AT(_sccb, _typ) ( ( duf_scanner_t  )         ( ( ( char * ) _sccb ) + ( offsetof( duf_scan_callbacks_t, _typ ) ) ) )

int duf_sccbh_eval_db_node_before( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_node_middle( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_node_after( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh );

const char *duf_scanstage_name( duf_scanstage_t scanstage, duf_node_type_t nt );
duf_scanner_t duf_scanstage_scanner( const duf_scan_callbacks_t * sccb, duf_scanstage_t scanstage, int deleted, duf_node_type_t nt );

#endif
