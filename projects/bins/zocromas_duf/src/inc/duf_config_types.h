#ifndef MAS_DUF_CONFIG_TYPES_H
#  define MAS_DUF_CONFIG_TYPES_H

#  define  DUF_TMP_PATH_MAX 6
#  define  DUF_TMP_EXPLANATION_MAX 6

#  include "duf_cli_types.h"
#  include "duf_item_types.h"
#  include "duf_ufilter_types.h"

#  include "duf_option_types.h"
#  include "duf_flags_types.h"
/* #include "duf_types.h" */

typedef struct
{
  char *name;
  char *fpath;
} duf_db_config_t;

typedef struct
{
  char *dir;
  duf_db_config_t main;
  duf_db_config_t adm;
} duf_dbs_config_t;

typedef struct
{
  char *path[DUF_TMP_PATH_MAX];
  unsigned explanation_index;
  char *option_explanation[DUF_TMP_EXPLANATION_MAX];
} duf_tmp_t;

typedef struct
{
  char *help_string;
  double loadtime;
  int actions_done;
  duf_ufilter_t u;
  duf_option_t *longopts_table;
  duf_config_cli_t cli;
  duf_dbs_config_t db;
  /* char *group; */
  int targc;
  char **targv;
  char *config_path;


  unsigned nopen;
  unsigned nclose;
  duf_tmp_t *tmp;
} duf_config_t;

#endif