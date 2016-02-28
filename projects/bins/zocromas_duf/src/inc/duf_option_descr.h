#ifndef MAS_DUF_OPTION_DESCR_H
# define MAS_DUF_OPTION_DESCR_H

/* # include "duf_options_enum.h"                                       (* duf_option_code_t ♠ *) */
# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t ♠ */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */

/* char *duf_loption_description_d( int longindex, const char *delimh, const char *delim ); */
char *duf_xoption_description_d( duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *delimh, const char *delim );

const char *duf_xoption_description_tmp( duf_config_cli_t * cli, int tmp_index, const duf_longval_extended_t * extended );
const char *duf_coption_xfind_desc_at_stdx( duf_config_cli_t * cli, duf_option_gen_code_t codeval, int *pr );

#endif
