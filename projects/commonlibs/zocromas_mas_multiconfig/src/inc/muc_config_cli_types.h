#ifndef MAS_MUC_CONFIG_CLI_TYPES_H
# define MAS_MUC_CONFIG_CLI_TYPES_H
# include <stdio.h>                                                  /* FILE */

# include <mastar/trace/mas_trace_types.h>                           /* mas_config_trace_t */
# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t */
# include <mastar/tools/mas_arg_tools.h>                             /* mas_arg_get_cb_arg_t */

# include "muc_base_output_types.h"                                  /* mas_base_output_t */
# include "muc_fun_types.h"
# include "muc_option_types.h"
# include "muc_option_data_types.h"                                  /* muc_option_adata_t */

/* muc_config_cli_t
 * options/parameters
 *   used for getting, parsing options
 * */
/* style verified 20160302.170208 */
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
  const mas_config_trace_t *ptracecfg;
};
typedef struct muc_config_cli_s muc_config_cli_t;

#endif

/*
vi: ft=c
*/
