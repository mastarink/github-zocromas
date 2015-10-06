#ifndef MAS_DUF_CONFIG_TYPES_H
#  define MAS_DUF_CONFIG_TYPES_H

#  include "duf_base_types.h"
#  include "duf_cli_types.h"
#  include "duf_option_types.h"

typedef struct
{
  int expandable:1;
  char *expanded;
  char *value;
} duf_config_string_t;

typedef struct
{
  duf_config_string_t name;
  char *fpath;
} duf_db_config_t;

typedef struct
{
  /* unsigned opened:1; */
  duf_config_string_t dir;
  duf_config_string_t subdir;
  char* path;
  duf_db_config_t main;
  duf_db_config_t adm;
  duf_db_config_t tempo;
  duf_db_config_t selected;
  /* char *opened_name; */
} duf_dbs_config_t;

typedef struct
{
  char *path;
} duf_save_config_t;

typedef struct
{
  char *help_string;
  double loadtime;
  duf_ufilter_t *puz;
  duf_option_t *longopts_table;
  duf_config_cli_t cli;
  duf_dbs_config_t db;
  duf_save_config_t save;
  /* char *group; */
  duf_cargvc_t carg;
  duf_argvc_t targ;
  int targ_offset;
  char *config_dir;
  char *cmds_dir;
  char *config_file_path;

  struct duf_depthinfo_s *pdi;
  unsigned nopen;
  unsigned nclose;

  duf_itemtag_t tag;
} duf_config_t;


#endif

/*
vi: ft=c
*/
