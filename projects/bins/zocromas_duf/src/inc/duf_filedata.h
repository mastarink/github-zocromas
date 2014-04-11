#ifndef MAS_DUF_FILEDATA_H
#  define MAS_DUF_FILEDATA_H

int duf_update_filedatas( void );
int duf_zero_filedatas( void );

unsigned long long duf_insert_filedata( unsigned long long file_inode, dev_t dev_id, const struct stat *pst_file );

#endif
