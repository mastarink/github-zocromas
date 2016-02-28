#ifndef MAS_DUF_CONFIG_CLI_TYPES_H
# define MAS_DUF_CONFIG_CLI_TYPES_H
# include <stdio.h>                                                  /* FILE */

# include <mastar/tools/mas_argvc_types.h>
# include <mastar/tools/mas_arg_tools.h>

/* # include "duf_flags_types.h" */
# include "duf_fun_types.h"
/* duf_format_t */
# include "duf_option_types.h"
# include "duf_option_data_types.h"
# include "duf_config_trace_types.h"
/* # include "duf_config_output_types.h" */

/* duf_config_cli_t
 * options/parameters
 *   used for getting, parsing options
 *   TODO: verbose to move elsewhere
 * */
struct duf_config_cli_s
{
  char *shorts;
  duf_option_gen_code_t maxcodeval;
  mas_cargvc_t carg;
  mas_argvc_t targ;
  int targ_offset;

  duf_option_t *longopts_table;
  duf_longval_extended_vtable_t **xvtable_multi;
/* long limit; */
  char *history_filename;
  char option_delimiter;
  char *config_dir;
  char *cmds_dir;
  duf_option_adata_t aod;

  mas_arg_get_cb_arg_t varfunc;
  const mas_config_trace_t  *ptracecfg;
};
typedef struct duf_config_cli_s duf_config_cli_t;

#endif

/*
vi: ft=c
*/
