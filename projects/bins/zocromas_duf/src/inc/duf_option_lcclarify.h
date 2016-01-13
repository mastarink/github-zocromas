#ifndef MAS_DUF_OPTION_LCCLARIFY_H
#  define MAS_DUF_OPTION_LCCLARIFY_H

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>_<action-result>
 * xtable ~ duf_longval_extended_table_t
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

#  include "duf_options_enum.h" /* duf_option_code_t :: TODO duf_option_code_t => duf_option_code_general_t i.e. `unsigned` */


DR( OPTIONS, lcoption_clarify, int longindex, duf_option_code_t codeval, const char *optargg, duf_option_stage_t istage, duf_option_source_t source )
      __attribute__ ( ( warn_unused_result ) );

#endif

