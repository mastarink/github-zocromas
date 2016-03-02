#ifndef MAS_MUC_OPTION_TYPED_GEN_H
# define MAS_MUC_OPTION_TYPED_GEN_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t */
# include "muc_option_source_types.h"                                /* muc_option_source_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t */
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

muc_DR( OPTIONS, xoption_clarify_typed_gen, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg, unsigned noo,
    muc_option_stage_t istage, muc_option_source_t source );

#endif
