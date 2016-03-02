#ifndef MAS_MUC_OPTION_CFIND_H
# define MAS_MUC_OPTION_CFIND_H

# include "muc_option_types.h"                                       /* ..._longval_extended_t; ..._longval_extended_vtable_t ♠ */
# include "muc_config_cli_types.h"                                   /* ..._config_cli_t ♠ */

/* codeval + std.xvtables => extended; codeval2extended_by_multitable */
const muc_longval_extended_t *muc_coption_xfind_at_stdx( muc_config_cli_t * cli, muc_option_gen_code_t codeval,
                                                         const muc_longval_extended_vtable_t ** result_pxvtable, int *pr );


const char *muc_lcoption_find_name_at_std( muc_config_cli_t * cli, muc_option_gen_code_t codeval, muc_option_gen_code_t maxcodeval, int *plongindex, int *pr );

#endif
