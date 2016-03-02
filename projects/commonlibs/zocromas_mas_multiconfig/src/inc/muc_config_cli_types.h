#ifndef MAS_MUC_CONFIG_CLI_TYPES_H
# define MAS_MUC_CONFIG_CLI_TYPES_H
# include <stdio.h>                                                  /* FILE */

# include <mastar/tools/mas_argvc_types.h>
# include <mastar/tools/mas_arg_tools.h>

/* # include "muc_flags_types.h" */
# include "muc_fun_types.h"
/* muc_format_t */
# include "muc_option_types.h"
# include "muc_option_data_types.h"
# include "muc_config_trace_types.h"
/* # include "muc_config_output_types.h" */

/* muc_config_cli_t
 * options/parameters
 *   used for getting, parsing options
 *   TODO: verbose to move elsewhere
 * */
struct muc_config_cli_s
{
  unsigned mandatory_config;
  char *shorts;
  muc_option_gen_code_t maxcodeval;
  mas_cargvc_t carg;
  mas_argvc_t targ;
  int targ_offset;

  muc_option_t *longopts_table;
  muc_longval_extended_vtable_t **xvtable_multi;
/* long limit; */
  char *history_filename;
  char option_delimiter;
  char *config_dir;
  char *cmds_dir;
  muc_option_adata_t aod;

  mas_arg_get_cb_arg_t varfunc;
  const mas_config_trace_t  *ptracecfg;
};
typedef struct muc_config_cli_s muc_config_cli_t;

#endif

/*
vi: ft=c
*/
