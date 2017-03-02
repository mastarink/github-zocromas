#ifndef MAS_MYSQLPFS_STMT_D_H
# define MAS_MYSQLPFS_STMT_D_H

# include "mysqlpfs_types.h"

int mas_mysqlpfs_mstmt_set_direct_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *str );

#endif
