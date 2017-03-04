#ifndef MAS_QSTD_STRUCTS_H
# define MAS_QSTD_STRUCTS_H

# include <mysql.h>
# include "mysqlpfs_types.h"


/* TODO rename mpfs_std_id_e */
enum mysqlpfs_std_id_e
{
  STD_MSTMT_INSERT_NAMES,
  STD_MSTMT_SELECT_NAMES_ID,
  STD_MSTMT_MAX
};
struct mas_qstd_s
{
  mysqlpfs_t *pfs;
  mysqlpfs_mstmt_t **std_mstmts;
};

#endif

