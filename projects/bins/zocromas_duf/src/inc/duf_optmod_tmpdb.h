#ifndef MAS_DUF_OPTMOD_TMPDB_H
# define MAS_DUF_OPTMOD_TMPDB_H

# include <mastar/multiconfig/muc_config_cli_types.h>                                   /* muc_config_cli_t ♠ */
# include <mastar/multiconfig/muc_se.h>                                   
/* # include "duf_se.h"                                                 (* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ *) */

muc_DR( OPTIONS, tmpdb_add, muc_config_cli_t * cli, int oval, const char *optname, const char *optargg ) __attribute__ ( ( warn_unused_result, weak ) );

#endif
