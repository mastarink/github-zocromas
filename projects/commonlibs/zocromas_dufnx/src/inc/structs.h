#ifndef MAS_DUFNX_STRUCTS_H
# define MAS_DUFNX_STRUCTS_H

# include "types.h"
# include <mastar/mulconfnt/structs.h>
#include <mastar/qstd/qstd_types.h>

struct mas_dufnx_mysql_data_s
{
  char *server;
  char *user;
  char *password;
  char *db;
  unsigned port;
  mas_qstd_t *qstd;
};

struct mas_dufnx_data_s
{
  masxfs_levinfo_flags_t levinfo_flags;
  mas_argvc_t targv;
  mas_dufnx_mysql_data_t mysql;
  masxfs_depth_t max_depth;                                          /* maxdepth OR 0 for all */
};

#endif
