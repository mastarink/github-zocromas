#ifndef MAS_DUF_FILEDATA_H
# define MAS_DUF_FILEDATA_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

unsigned long long duf_pdistat2file_dataid_existed( duf_depthinfo_t * pdi, /* const struct stat *pst_file, */ int *pr );
unsigned long long duf_pdistat2file_dataid( duf_depthinfo_t * pdi, /* const struct stat *pst_file, */ int need_id, int *pr );
int duf_pdistat2file( duf_depthinfo_t * pdi );

#endif
