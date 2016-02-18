#ifndef MAS_DUF_CLI_TYPES_H
# define MAS_DUF_CLI_TYPES_H
# include <stdio.h>/* FILE */

# include <mastar/tools/mas_argvc_types.h>

# include "duf_flags_types.h"
# include "duf_fun_types.h"
/* duf_format_t */
# include "duf_option_types.h"
# include "duf_trace_types.h"
# include "duf_config_output_types.h"


/* duf_config_cli_t
 * options/parameters
 *   used for getting, parsing options
 *   TODO: verbose to move elsewhere
 * */
typedef struct
{
  char *shorts;
  mas_cargvc_t carg;
  mas_argvc_t targ;
  int targ_offset;

  duf_option_t *longopts_table;
  const duf_longval_extended_vtable_t *const *xvtable_multi;
/* long limit; */
  char *history_filename;
  char option_delimiter;
  int verbose;
} duf_config_cli_t;


#endif

/*
vi: ft=c
*/
