#ifndef MAS_QSTD_STRUCTS_H
# define MAS_QSTD_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>


/* TODO rename mas_qstd_id_e */
enum mas_qstd_id_e
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

