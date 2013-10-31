#ifndef MAS_DUF_UPDATE_H
#  define MAS_DUF_UPDATE_H

unsigned long long duf_update_path( const char *path, unsigned long long rs_up, int recurse, int dofiles, int added );
int duf_update_mdpaths( void );

#endif
