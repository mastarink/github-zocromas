#ifndef MAS_DUF_FILEDATA_H
#  define MAS_DUF_FILEDATA_H

unsigned long long duf_file_dataid_by_stat( duf_depthinfo_t * pdi, const struct stat *pst_file, int *pr );

unsigned long long duf_insert_filedata_uni1( const struct stat *pst_file, int need_id, int *pr );
unsigned long long duf_insert_filedata_uni1a( const struct stat *pst_file, int need_id, int *pr );

/* unsigned long long duf_insert_filedata( unsigned long long file_inode, dev_t dev_id, const struct stat *pst_file ); */
unsigned long long duf_insert_filedata_uni( duf_depthinfo_t * pdi, const struct stat *pst_file, int need_id, int *pr );

#endif
