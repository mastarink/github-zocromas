#ifndef MAS_DUF_OPTION_TMPDB_H
# define MAS_DUF_OPTION_TMPDB_H

# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

DR(OPTIONS, tmpdb_add, duf_config_cli_t * cli, int oval, const char *optname, const char *optargg ) __attribute__ ( ( warn_unused_result, weak ) );

#endif
