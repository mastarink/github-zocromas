#ifndef MAS_DUF_PATH2DIRID_H
# define MAS_DUF_PATH2DIRID_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

unsigned long long duf_path2dirid( const char *path, int *pr );
char *duf_dirid2path( unsigned long long dirid, int *pr );

char *duf_dirid2name_existed( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid, int *pr );

#endif
