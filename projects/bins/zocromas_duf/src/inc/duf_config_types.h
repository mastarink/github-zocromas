#ifndef MAS_DUF_CONFIG_TYPES_H
# define MAS_DUF_CONFIG_TYPES_H

# include "duf_base_types.h"
# include "duf_ufilter_types.h"
# include "duf_levinfo_types.h"
# include "duf_config_cli_types.h"
# include "duf_expand_types.h"
# include "duf_config_db_types.h"
# include "duf_config_opt_types.h"

typedef struct
{
  char *path;
} duf_config_save_t;

typedef struct
{
  unsigned nopen;
  unsigned nclose;
} duf_status_dh_t;

typedef struct
{
/* struct duf_depthinfo_s *pdi; */
  duf_depthinfo_t *pdi;
} duf_status_scanner_t;

typedef struct
{
  char *config_dir;
  char *cmds_dir;
  char *config_file_path;
} duf_config_cfg_t;

typedef struct
{
  duf_itemtag_t tag;
  int dir_priority;
  duf_ufilter_t *puz;
} duf_config_vars_t;

typedef struct
{
  char *help_string;
/* double loadtime; */
  duf_config_cli_t cli; /* options/parameters used for parsing cli options */
  duf_config_opt_t opt; /* misc. options/parameters */
  duf_config_db_t db; /* options/parameters fpr DB opening / tuning */
  duf_config_save_t save;
  duf_config_vars_t vars;
/* char *group; */
  duf_config_cfg_t conf;
  unsigned long testnum;
} duf_config_t;

#endif

/*
vi: ft=c
*/
