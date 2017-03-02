#ifndef MAS_MYSQLPFS_STMT_BASE_H
# define MAS_MYSQLPFS_STMT_BASE_H

# include "mysqlpfs_types.h"

void mas_mysqlpfs_mstmt_init( mysqlpfs_t * pfs, mysqlpfs_mstmt_t * mstmt, int nparams );
mysqlpfs_mstmt_t *mas_mysqlpfs_mstmt_create( void );
mysqlpfs_mstmt_t *mas_mysqlpfs_mstmt_create_setup( mysqlpfs_t * pfs, int nparams, const char *sqlop );

int mas_mysqlpfs_mstmt_reset( mysqlpfs_mstmt_t * mstmt );
void mas_mysqlpfs_mstmt_delete( mysqlpfs_mstmt_t * mstmt );


#endif

