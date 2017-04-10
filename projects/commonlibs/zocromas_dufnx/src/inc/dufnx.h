#ifndef MAS_DUFNX_H
# define MAS_DUFNX_H

# include <mastar/levinfo/masxfs_levinfo_types.h>
# include <mastar/tools/mas_argvc_types.h>

typedef struct mas_dufnx_mysql_data_s mas_dufnx_mysql_data_t;
typedef struct mas_dufnx_data_s mas_dufnx_data_t;

struct mas_dufnx_mysql_data_s
{
  char *server;
  char *user;
  char *password;
  char *db;
  unsigned port;
};

struct mas_dufnx_data_s
{
  masxfs_levinfo_flags_t levinfo_flags;
  mas_argvc_t targv;
  mas_dufnx_mysql_data_t mysql;
};

int dufnx( int argc, char *argv[] );

#endif
