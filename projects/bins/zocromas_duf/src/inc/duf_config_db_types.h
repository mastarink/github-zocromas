#ifndef MAS_DUF_CONFIG_DB_TYPES_H
# define MAS_DUF_CONFIG_DB_TYPES_H

# include "duf_expand_types.h"                                       /* duf_expandable_string_t */

typedef struct
{
  duf_expandable_string_t name;
  char *fpath;
} duf_db_config_t;

/* options/parameters fpr DB opening / tuning */
typedef struct
{
/* unsigned opened:1; */
  duf_expandable_string_t dir;
  duf_expandable_string_t subdir;
  char *path;
  duf_db_config_t main;
  duf_db_config_t adm;
  duf_db_config_t tempo;
  duf_db_config_t selected;
/* char *opened_name; */
} duf_config_db_t;

#endif
