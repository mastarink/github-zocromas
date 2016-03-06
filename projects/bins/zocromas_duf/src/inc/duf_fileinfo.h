#ifndef MAS_DUF_FILEINFO_H
# define MAS_DUF_FILEINFO_H

# include "duf_fileinfo_types.h"
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

int duf_fileinfo( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi );

#endif
