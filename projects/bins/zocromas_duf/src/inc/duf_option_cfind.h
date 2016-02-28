#ifndef MAS_DUF_OPTION_CFIND_H
# define MAS_DUF_OPTION_CFIND_H

/* # include "duf_options_enum.h"                                       (* duf_option_code_t ♠ *) */
# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t ♠ */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */

/* codeval + std.xvtables => extended; codeval2extended_by_multitable */
const duf_longval_extended_t *duf_coption_xfind_at_stdx( duf_config_cli_t * cli, duf_option_gen_code_t codeval,
                                                         const duf_longval_extended_vtable_t ** result_pxvtable, int *pr );

/* const char *duf_coption_find_name_at_std( duf_option_gen_code_t codeval, int *pr ); */

/* const duf_option_t *duf_lcoption_find_at_std( duf_option_gen_code_t codeval, int *plongindex, int *pr ); */
const char *duf_lcoption_find_name_at_std( duf_config_cli_t * cli, duf_option_gen_code_t codeval, duf_option_gen_code_t maxcodeval, int *plongindex, int *pr );

#endif
