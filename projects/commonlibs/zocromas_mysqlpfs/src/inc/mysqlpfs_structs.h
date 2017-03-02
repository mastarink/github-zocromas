#ifndef MAS_MYSQLPFS_STRUCTS_H
# define MAS_MYSQLPFS_STRUCTS_H

# include <mysql.h>
# include "mysqlpfs_types.h"

struct mysqlpfs_s
{
  MYSQL mysql;
  mysqlpfs_s_result_t *result;
};
struct mysqlpfs_mbind_s
{
  int nbind;
  mysqlpfs_s_length_t *length;
  mysqlpfs_s_bool_t *is_null;
  void **allocated_buffers;
  mysqlpfs_s_bind_t *bind;
};
struct mysqlpfs_mstmt_s
{
  mysqlpfs_s_stmt_t *stmt;
  struct
  {
    mysqlpfs_mbind_t param;
    mysqlpfs_mbind_t result;
  } binds;
};

#endif
