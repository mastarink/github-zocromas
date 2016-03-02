#ifndef MAS_MUC_OPTION_LFIND_H
# define MAS_MUC_OPTION_LFIND_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */

const muc_longval_extended_t *muc_loption_xfind_at_stdx( muc_config_cli_t * cli, int longindex,
                                                         const muc_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo );
const muc_option_t *muc_loption_find_at_std( muc_config_cli_t * cli, int longindex );

#endif
