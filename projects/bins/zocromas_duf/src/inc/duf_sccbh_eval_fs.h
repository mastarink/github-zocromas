#ifndef MAS_DUF_SCCBH_EVAL_FS_H
#  define MAS_DUF_SCCBH_EVAL_FS_H


/* int duf_sccbh_eval_fs_with2scanners( duf_sccb_handle_t * sccbh (*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 *)  ); */
int DUF_WRAPPED( duf_sccbh_eval_fs ) ( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
      __attribute__ ( ( warn_unused_result ) );

#endif
