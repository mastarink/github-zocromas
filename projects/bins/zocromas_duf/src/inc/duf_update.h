#ifndef MAS_DUF_UPDATE_H
#  define MAS_DUF_UPDATE_H

sqlite3_int64 update_path( const char *path, sqlite3_int64 rs_up, int recurse, int dofiles );
void update_mdline( void );

#endif
