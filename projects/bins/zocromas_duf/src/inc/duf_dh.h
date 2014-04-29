#ifndef MAS_DUF_DH_H
#  define MAS_DUF_DH_H

int duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile );
int duf_open_dh( duf_dirhandle_t * pdhandle, const char *path );
int duf_close_dh( duf_dirhandle_t * pdhandle );
int duf_check_dh( const char *msg );
int duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name );

#endif
