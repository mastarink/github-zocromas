#ifndef MAS_MUC_OPTION_DESCR_H
# define MAS_MUC_OPTION_DESCR_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */

/* char *..._loption_description_d( int longindex, const char *delimh, const char *delim ); */
char *muc_xoption_description_d( muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *delimh, const char *delim );

const char *muc_xoption_description_tmp( muc_config_cli_t * cli, int tmp_index, const muc_longval_extended_t * extended );
const char *muc_coption_xfind_desc_at_stdx( muc_config_cli_t * cli, muc_option_gen_code_t codeval, int *pr );

#endif
