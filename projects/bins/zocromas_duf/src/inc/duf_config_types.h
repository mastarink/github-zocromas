#ifndef MAS_DUF_CONFIG_TYPES_H
#  define MAS_DUF_CONFIG_TYPES_H

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
  char *help_string;
  double loadtime;
  duf_ufilter_t *pu;
  duf_option_t *longopts_table;
  duf_config_cli_t cli;
  duf_dbs_config_t db;
  /* char *group; */
  int cargc;
  char *const *cargv;
  int targc;
  char **targv;
  char *config_path;

  struct duf_depthinfo_s *pdi;
  unsigned nopen;
  unsigned nclose;
} duf_config_t;


#endif
