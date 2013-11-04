#ifndef MAS_DUF_FILEDATA_H
#  define MAS_DUF_FILEDATA_H

int duf_update_filedatas( void );
int duf_zero_filedatas( void );

unsigned long long duf_insert_filedata( unsigned long long file_inode, const struct stat *pst_dir, const struct stat *pst_file );

#endif
