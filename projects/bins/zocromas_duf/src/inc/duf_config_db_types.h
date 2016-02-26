#ifndef MAS_DUF_CONFIG_DB_TYPES_H
# define MAS_DUF_CONFIG_DB_TYPES_H

#include <mastar/tools/mas_expandable.h>

/* # include "duf_expand_types.h"                                       (* duf_expandable_string_t *) */

typedef struct
{
  mas_expandable_string_t name_x;
  char *fpath;
} duf_db_config_t;

/* options/parameters fpr DB opening / tuning */
typedef struct
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
} duf_config_db_t;

#endif
