#ifndef MAS_DUF_FILEINFO_H
# define MAS_DUF_FILEINFO_H

# include "duf_fileinfo_types.h"
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( OTHER, fileinfo, /* duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, */ duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi );

#endif
