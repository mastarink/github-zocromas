#ifndef MAS_DUF_OPTIMPL_FS_H
# define MAS_DUF_OPTIMPL_FS_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ▤ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SNIPPET_OPTION, optimpl_O_fs_ls, const char *arg );
DR( SNIPPET_OPTION, optimpl_O_fs_rm, const char *arg, long v );
DR( SNIPPET_OPTION, optimpl_O_fs_cp, const char *arg, long v );
DR( SNIPPET_OPTION, optimpl_O_fs_mv, const char *arg, long v );

#endif
