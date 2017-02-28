#ifndef MAS_MYSQLPFS_STRUCTS_H
# define MAS_MYSQLPFS_STRUCTS_H

# include <mysql.h>
# include "mysqlpfs_types.h"

struct mysqlpfs_s
{
  MYSQL mysql;
  mysqlpfs_result_t *result;
};

#endif
