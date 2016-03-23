#ifndef MAS_DUF_DH_H
# define MAS_DUF_DH_H

/* duf_dirhandle_t */
# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_levinfo_dirhandle_types.h"                            /* duf_dirhandle_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( OTHER, openat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile );
DR( OTHER, open_dh, duf_dirhandle_t * pdhandle, const char *path );

/* returns handle >0 */
DR( OTHER, opened_dh, duf_dirhandle_t * pdhandle );

DR( OTHER, close_dh, duf_dirhandle_t * pdhandle );
DR( OTHER, check_dh, const char *msg );

DR( OTHER, statat_dh, duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name );
DR( OTHER, stat_dh, duf_dirhandle_t * pdhandle, const char *path );

#endif
