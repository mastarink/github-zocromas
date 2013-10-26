#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H


char *join_path( const char *path, const char *fname );
char *single_quotes_2( const char *s );

char *path_id_to_path( sqlite3_int64 pathid );
sqlite3_int64 path_to_path_id( const char *path );
sqlite3_int64 file_to_path_id( const char *path, const char *name, sqlite3_int64 * pmd5id );

sqlite3_int64 md5id_of_file( const char *path, const char *name );



#endif
