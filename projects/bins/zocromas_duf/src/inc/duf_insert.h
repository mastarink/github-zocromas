#ifndef MAS_DUF_INSERT_H
#  define MAS_DUF_INSERT_H

#  include <sys/stat.h>
#  include <dirent.h>

unsigned long long insert_filedata( unsigned long long file_inode, struct stat *pst_dir, struct stat *pst_file );
unsigned long long insert_path( const char *base_name, struct stat *pst_dir, unsigned long long rs_up, int added );
unsigned long long insert_filename( const char *fname, unsigned long long res, unsigned long long resd );
unsigned long long insert_md5( unsigned long long *md64, size_t fsize );
unsigned long long insert_keydata( unsigned long long pathid, unsigned long long nameid, ino_t inode, unsigned long long resmd );

#endif
