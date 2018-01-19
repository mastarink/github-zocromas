#ifndef MAS_MYSQLPFS_STRUCTS_H
# define MAS_MYSQLPFS_STRUCTS_H

# include "mysqlpfs_types_my.h"
# include "mysqlpfs_types.h"

/* TODO rename myq_mfield_type_s */
enum mysqlpfs_mfield_type_e
{
  PFS_STRING = MYSQL_TYPE_STRING,
  PFS_LONGLONG = MYSQL_TYPE_LONGLONG,
  PFS_TIMESTAMP = MYSQL_TYPE_TIMESTAMP,
  PFS_BINARY = MYSQL_TYPE_STRING,
};

/* TODO rename myq_s */
struct mysqlpfs_s
{
  MYSQL mysql;
  mysqlpfs_s_result_t *result;
  char *table_prefix;
  int inited;
};

/* TODO rename myq_bind_s */
struct mysqlpfs_mbind_s
{
  int nbind;
  mysqlpfs_s_length_t *length;
  mysqlpfs_s_bool_t *is_null;
  void **allocated_buffers;
  mysqlpfs_s_bind_t *bind;
};

/* TODO rename myq_mstmt_s */
struct mysqlpfs_mstmt_s
{
  mysqlpfs_s_stmt_t *stmt;
  struct
  {
    mysqlpfs_mbind_t param;
    mysqlpfs_mbind_t result;
  } binds;
  const mysqlpfs_t *pfs;
  int ret_code;
};

#endif
