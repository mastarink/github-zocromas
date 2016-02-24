#ifndef MAS_DUF_OPTION_CLARIFY_H
# define MAS_DUF_OPTION_CLARIFY_H

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t ; mas_cargvc_t */
# include "duf_option_types.h"                                       /* duf_longval_extended_t ; duf_longval_extended_vtable_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>_<action-result>
 * xtable ~ duf_longval_extended_vtable_t
 * xarr ~ array of `const duf_longval_extended_t *`, (xarr = xtable->table), limited by xtended->o.name == NULL
 * multix ~  multitable ~ array of xtable's, limited by NULL
 * stdx ~ standard multix (multitable ~ array of xtable's, limited by NULL)
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   const char *ostring			=> s  >> ostring is: name + tail
 *   const char *osbatch			=> b  >> osbatch is: several ostrings, separated with `separator`
 *   const char *oname				=> n
 *   duf_option_code_t codeval			=> c
 *   int longindex				=> l
 *   int longindex, duf_option_code_t codeval	=> lc
 *   duf_longval_extended_t * extended		=> x
 * 3. entity: option
 * 4. action: clarify, clarify_full
 * */

int duf_xoption_clarify( duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
                         const duf_longval_extended_vtable_t * xtable, unsigned noo, duf_option_stage_t stage,
                         duf_option_source_t source ) __attribute__ ( ( warn_unused_result ) );

int duf_argv_clarify( duf_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos );

#endif
