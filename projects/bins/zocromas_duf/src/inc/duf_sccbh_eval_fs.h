#ifndef MAS_DUF_SCCBH_EVAL_FS_H
# define MAS_DUF_SCCBH_EVAL_FS_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* int duf_sccbh_eval_fs_with2scanners( duf_sccb_handle_t * sccbh (*, duf_scanner_t scanner_dirent_reg2, duf_scanner_t scanner_dirent_dir2 *)  ); */
DR( SCCBH, sccbh_eval_fs, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage ) __attribute__ ( ( warn_unused_result ) );

#endif
