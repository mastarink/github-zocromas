#ifndef MAS_DUF_FILEDATA_H
#  define MAS_DUF_FILEDATA_H


unsigned long long duf_stat2file_dataid_existed( duf_depthinfo_t * pdi, const struct stat *pst_file, int *pr );
unsigned long long duf_stat2file_dataid( duf_depthinfo_t * pdi, const struct stat *pst_file, int need_id, int *pr );

#endif
