#ifndef MAS_DUF_CONFIG_STRUCTS_H
# define MAS_DUF_CONFIG_STRUCTS_H

# include <mastar/multiconfig/muc_config_cli_types.h>                /* muc_config_cli_t */

# include "duf_base_types.h"                                         /* duf_itemtag_t */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

# include "duf_config_opt_types.h"                                   /* duf_config_opt_t */
# include "duf_config_opt_structs.h"

# include "duf_config_db_types.h"                                    /* duf_config_db_t */
# include "duf_config_db_structs.h"                                  /* duf_config_db_t */

# include "duf_config_types.h"                                       /* duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t ✗ */

struct duf_config_save_s
{
  char *path;
};

struct duf_config_cfg_s
{
  char *config_dir;
  char *cmds_dir;
  char *config_file_path;
};

struct duf_config_vars_s
{
  duf_itemtag_t tag;
  int dir_priority;
  duf_ufilter_t *puz;
};

struct duf_config_s
{
  char *help_string;
/* double loadtime; */
  muc_config_cli_t *pcli;                                            /* options/parameters used for parsing cli options */
  duf_config_opt_t opt;                                              /* misc. options/parameters */
  duf_config_db_t db;                                                /* options/parameters fpr DB opening / tuning */
  struct duf_config_save_s save;
  struct duf_config_vars_s vars;
/* char *group; */
  duf_config_cfg_t conf;
  unsigned long testnum;
};

#endif

/*
vi: ft=c
*/
