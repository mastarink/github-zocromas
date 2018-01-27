#ifndef MAS_DUFNX_STRUCTS_H
# define MAS_DUFNX_STRUCTS_H

# include "types.h"
# include <mastar/mulconfnt/structs.h>
# include <mastar/qstd/qstd_types.h>

struct mas_dufnx_mysql_data_s
{
  char *server;
  char *user;
  char *password;
  char *db;
  unsigned port;
  char *table_prefix;
  mas_qstd_t *qstd;
};
enum mas_dufnx_flag_e
{
  MASDUFNX_DUMMY_NUM,
  MASDUFNX_COLLECT_NUM,
  MASDUFNX_LIST_NUM,
};
enum mas_dufnx_flag_bit_e
{
  MASDUFNX_DUMMY = 1UL << MASDUFNX_DUMMY_NUM,
  MASDUFNX_COLLECT = 1UL << MASDUFNX_COLLECT_NUM,
  MASDUFNX_LIST = 1UL << MASDUFNX_LIST_NUM,
};
typedef unsigned long mas_dufnx_flags_t;

struct mas_dufnx_data_s
{
  masxfs_levinfo_flags_t levinfo_flags;
  mas_dufnx_flags_t dufnx_flags;
  mas_argvc_t targv;
  mas_dufnx_mysql_data_t mysql;
//  masxfs_depth_t max_depth;                                          /* maxdepth OR 0 for all */
  masxfs_entry_filter_t entry_filter;
  char *savefname;
};

#endif
