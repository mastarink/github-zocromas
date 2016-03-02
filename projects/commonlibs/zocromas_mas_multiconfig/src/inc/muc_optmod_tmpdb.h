#ifndef MAS_MUC_OPTMOD_TMPDB_H
# define MAS_MUC_OPTMOD_TMPDB_H

# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */
# include "muc_se.h"                                   
/* # include "muc_se.h"                                                 (* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ *) */

muc_DR( OPTIONS, tmpdb_add, muc_config_cli_t * cli, int oval, const char *optname, const char *optargg ) __attribute__ ( ( warn_unused_result, weak ) );

#endif
