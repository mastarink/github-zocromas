#ifndef MAS_DUF_OPTIMPL_SET_HELP_H
# define MAS_DUF_OPTIMPL_SET_HELP_H

# include <mastar/error/mas_error_types.h>                           /* mas_error_code_t ▤ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */
# include "duf_optimpl_class_types.h"

/* duf_optimpl_class_t duf_help_option2class( duf_option_code_t code ); */

DR( SNIPPET_OPTION, optimpl_O_set_help, const char *arg );

#endif

