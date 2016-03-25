#ifndef MAS_DUF_CONFIG_DB_STRUCTSS_H
# define MAS_DUF_CONFIG_DB_STRUCTSS_H

# include <mastar/tools/mas_expandable.h>                            /* mas_expandable_string_t */

struct duf_db_config_s
{
  mas_expandable_string_t name_x;
  char *fpath;
};

/* options/parameters fpr DB opening / tuning */
struct duf_config_db_s
{
/* unsigned opened:1; */
  mas_expandable_string_t dir_x;
  mas_expandable_string_t subdir_x;
  char *path;
  duf_db_config_t main;
  duf_db_config_t adm;
  duf_db_config_t tempo;
  duf_db_config_t selected;
/* char *opened_name; */
};

#endif
