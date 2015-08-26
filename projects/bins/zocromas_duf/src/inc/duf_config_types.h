#ifndef MAS_DUF_CONFIG_TYPES_H
#  define MAS_DUF_CONFIG_TYPES_H

#  include "duf_base_types.h"
#  include "duf_cli_types.h"
#  include "duf_option_types.h"

typedef struct
{
  char *name;
  char *fpath;
} duf_db_config_t;

typedef struct
{
  unsigned opened:1;
  char *dir;
  duf_db_config_t main;
  duf_db_config_t adm;
  char *opened_name;
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
  duf_cargvc_t carg;
  duf_argvc_t targ;
  int targ_offset;
  char *config_path;

  struct duf_depthinfo_s *pdi;
  unsigned nopen;
  unsigned nclose;

  duf_itemtag_t tag;
} duf_config_t;


#endif

/*
vi: ft=c
*/
