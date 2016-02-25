#ifndef MAS_DUF_OPTION_NAMES_H
#  define MAS_DUF_OPTION_NAMES_H

# include "duf_options_enum.h"                                       /* duf_option_code_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: optstage
 *   3.1. entity-details:
 * 4. result/action: 
 * */

const char *duf_coption_cnames_tmp( duf_config_cli_t * cli,int index, duf_option_code_t codeval, const char *delim );

/* char *duf_coption_names( duf_option_code_t codeval ); */
char *duf_coption_names_d( duf_config_cli_t * cli,duf_option_code_t codeval, const char *delim );


#  define DUF_OPT_NAME_D(_cli, _n,_d) duf_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_ ## _n, _d )
#  define DUF_OPT_NAME(_cli,_n)  DUF_OPT_NAME_D(_cli,_n,NULL)

#  define DUF_OPT_FLAG_NAME_D(_cli, _n, _d) duf_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_FLAG_ ## _n,_d )
#  define DUF_OPT_FLAG_NAME(_cli,_n)  DUF_OPT_FLAG_NAME_D(_cli, _n,NULL)

#  define DUF_OPT_NAME1(_cli,_n) duf_coption_cnames_tmp( _cli,1, DUF_OPTION_VAL_ ## _n )

/* #  define DUF_OPT_FLAG_NAME1_D(_n,_d) duf_coption_cnames_tmp(_cli, 1, DUF_OPTION_FLAG_ ## _n, _d ) */
/* #  define DUF_OPT_FLAG_NAME1(_n) DUF_OPT_FLAG_NAME1_D(_n,NULL) */

/* #  define DUF_OPT_NAME2_D(_n,_d) duf_coption_cnames_tmp(_cli, 2, DUF_OPTION_VAL_ ## _n,_d ) */
/* #  define DUF_OPT_NAME2(_n) DUF_OPT_NAME2_D(_n,NULL) */

#  define DUF_OPT_FLAG_NAME2_D(_cli,_n,_d) duf_coption_cnames_tmp(_cli, 2, DUF_OPTION_VAL_FLAG_ ## _n,_d )
#  define DUF_OPT_FLAG_NAME2(_cli,_n) DUF_OPT_FLAG_NAME2_D(_cli,_n,NULL)

#  define DUF_OPTION_NAME_D(_cli,_n,_d)  duf_coption_cnames_tmp(_cli,-1,_n,_d)
#  define DUF_OPTION_NAME(_cli,_n)  DUF_OPTION_NAME_D(_cli,_n,NULL)

#  define DUF_OPTION_LINDEX_NAME_D(_cli,_n,_d)  duf_coption_cnames_tmp(_cli, -1, duf_longindex_extended_codeval( _n ), _d )
#  define DUF_OPTION_LINDEX_NAME(_cli,_n)  DUF_OPTION_LINDEX_NAME_D(_cli,_n,NULL)


#endif
