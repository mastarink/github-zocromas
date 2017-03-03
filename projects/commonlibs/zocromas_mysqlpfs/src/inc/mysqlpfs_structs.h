#ifndef MAS_MYSQLPFS_STRUCTS_H
# define MAS_MYSQLPFS_STRUCTS_H

# include <mysql.h>
# include "mysqlpfs_types.h"

enum mysqlpfs_mfield_type_e
{
  PFS_STRING = MYSQL_TYPE_STRING,
  PFS_LONGLONG = MYSQL_TYPE_LONGLONG,
  PFS_TIMESTAMP = MYSQL_TYPE_TIMESTAMP,
};

enum mysqlpfs_std_id_e
{
  STD_MSTMT_INSERT_NAMES,
  STD_MSTMT_SELECT_NAMES_ID,
  STD_MSTMT_MAX
};

struct mysqlpfs_s
{
  MYSQL mysql;
  mysqlpfs_s_result_t *result;
  mysqlpfs_mstmt_t **std_mstmts;
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
