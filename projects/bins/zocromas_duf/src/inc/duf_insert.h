#ifndef MAS_DUF_INSERT_H
#  define MAS_DUF_INSERT_H

#  include <sys/stat.h>
#  include <dirent.h>

unsigned long long duf_insert_filedata( unsigned long long file_inode, struct stat *pst_dir, struct stat *pst_file );
unsigned long long duf_insert_path( const char *base_name, struct stat *pst_dir, unsigned long long rs_up );
unsigned long long duf_insert_filename( const char *fname, unsigned long long res, unsigned long long resd );
unsigned long long duf_insert_md5( unsigned long long *md64, size_t fsize );
unsigned long long duf_insert_keydata( unsigned long long pathid, unsigned long long nameid, ino_t inode, unsigned long long resmd );
unsigned long long duf_insert_group( const char *name );
unsigned long long duf_insert_path_group( unsigned long long groupid, unsigned long long pathid );

#endif
