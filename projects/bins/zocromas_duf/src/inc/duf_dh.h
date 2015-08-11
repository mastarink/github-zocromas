#ifndef MAS_DUF_DH_H
#  define MAS_DUF_DH_H

/* duf_dirhandle_t */
#include "duf_levinfo_types.h"

int duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile );
int duf_open_dh( duf_dirhandle_t * pdhandle, const char *path );

/* returns handle >0 */
int duf_opened_dh( duf_dirhandle_t * pdhandle );

int duf_close_dh( duf_dirhandle_t * pdhandle );
int duf_check_dh( const char *msg );

int duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name );
int duf_stat_dh( duf_dirhandle_t * pdhandle, const char *path );

#endif
