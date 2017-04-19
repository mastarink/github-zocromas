#ifndef MAS_QSTD_STRUCTS_H
# define MAS_QSTD_STRUCTS_H

# include <mastar/mysqlpfs/mysqlpfs_types.h>

struct mas_qstd_s
{
  mysqlpfs_t *pfs;
  mysqlpfs_mstmt_t **std_mstmts;
};

#endif
