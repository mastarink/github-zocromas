#ifndef MAS_DUF_CONFIG_TYPES_H
#  define MAS_DUF_CONFIG_TYPES_H

#  include "duf_base_types.h"
#  include "duf_cli_types.h"

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
} duf_config_db_t;

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
  duf_ufilter_t *puz;
  /* struct duf_depthinfo_s *pdi; */
  duf_depthinfo_t *pdi;
} duf_status_scanner_t;
typedef struct
{
  char *help_string;
  double loadtime;
  duf_config_cli_t cli;
  duf_config_opt_t opt;
  duf_config_db_t db;
  duf_config_save_t save;
  /* char *group; */
  char *config_dir;
  char *cmds_dir;
  char *config_file_path;

  duf_status_scanner_t scn;

  duf_status_dh_t dh;
  duf_itemtag_t tag;
  int dir_priority;
} duf_config_t;


#endif

/*
vi: ft=c
*/
