#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H


char *join_path( const char *path, const char *fname );
char *single_quotes_2( const char *s );

char *path_id_to_path( unsigned long long pathid );
unsigned long long path_to_path_id( const char *path );
unsigned long long file_to_path_id( const char *path, const char *name, unsigned long long *pmd5id );

unsigned long long md5id_of_file( const char *path, const char *name );

int print_paths( void );


#endif
