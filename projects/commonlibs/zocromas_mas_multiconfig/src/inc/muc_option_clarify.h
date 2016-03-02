#ifndef MAS_MUC_OPTION_CLARIFY_H
# define MAS_MUC_OPTION_CLARIFY_H

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t ; mas_cargvc_t */
# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ♠ */
# include "muc_option_source_types.h"                                /* muc_option_source_t; muc_option_source_code_t ♠ */
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>_<action-result>
 * xtable ~ muc_longval_extended_vtable_t
 * xarr ~ array of `const muc_longval_extended_t *`, (xarr = xtable->table), limited by xtended->o.name == NULL
 * multix ~  multitable ~ array of xtable's, limited by NULL
 * stdx ~ standard multix (multitable ~ array of xtable's, limited by NULL)
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   const char *ostring			=> s  >> ostring is: name + tail
 *   const char *osbatch			=> b  >> osbatch is: several ostrings, separated with `separator`
 *   const char *oname				=> n
 *   muc_option_code_t codeval			=> c
 *   int longindex				=> l
 *   int longindex, muc_option_code_t codeval	=> lc
 *   muc_longval_extended_t * extended		=> x
 * 3. entity: option
 * 4. action: clarify, clarify_full
 * */

muc_DR( OPTIONS, xoption_clarify, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
    const muc_longval_extended_vtable_t * xtable, unsigned noo, muc_option_stage_t stage,
    muc_option_source_t source ) __attribute__ ( ( warn_unused_result ) );

muc_DR( OPTIONS, argv_clarify, muc_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos );

#endif
