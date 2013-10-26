#ifndef MAS_DUF_INSERT_H
#  define MAS_DUF_INSERT_H

#  include <sys/stat.h>
#  include <dirent.h>

sqlite3_int64 insert_filedata( sqlite3_int64 file_inode, struct stat *pst_dir, struct stat *pst_file );
sqlite3_int64 insert_path( const char *base_name, struct stat *pst_dir, sqlite3_int64 rs_up );
sqlite3_int64 insert_filename( const char *fname, sqlite3_int64 res, sqlite3_int64 resd );
sqlite3_int64 insert_md5( unsigned long long *md64 );
sqlite3_int64 insert_keydata( sqlite3_int64 pathid, sqlite3_int64 nameid, ino_t inode, sqlite3_int64 resmd );

#endif
