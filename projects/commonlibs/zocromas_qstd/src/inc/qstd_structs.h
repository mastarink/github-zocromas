#ifndef MAS_QSTD_STRUCTS_H
# define MAS_QSTD_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>


/* TODO rename mas_qstd_id_e */
enum mas_qstd_id_e
{
  STD_MSTMT_INSERT_NAMES,
  STD_MSTMT_SELECT_NAMES_ID,
  STD_MSTMT_INSERT_PARENTS,
  STD_MSTMT_SELECT_PARENTS_ID,
  STD_MSTMT_INSERT_SIZES,
  STD_MSTMT_SELECT_SIZES_ID,
  STD_MSTMT_INSERT_DATAS,
  STD_MSTMT_SELECT_DATAS_ID,
  STD_MSTMT_INSERT_PROPS,
  STD_MSTMT_SELECT_PROPS_ID,
  STD_MSTMT_SELECT_NODES_ID,
  STD_MSTMT_MAX
};

struct mas_qstd_s
{
  mysqlpfs_t *pfs;
  mysqlpfs_mstmt_t **std_mstmts;
};

#endif

