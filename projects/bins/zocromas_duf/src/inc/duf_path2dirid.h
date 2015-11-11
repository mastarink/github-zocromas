#ifndef MAS_DUF_PATH2DIRID_H
#  define MAS_DUF_PATH2DIRID_H

unsigned long long duf_path2dirid( const char *path, int *pr );
char *duf_dirid2path( unsigned long long dirid, int *pr );

char *duf_dirid2name_existed( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid, int *pr );

int duf_dirid2li_existed( duf_depthinfo_t * pdi, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid );

#endif
