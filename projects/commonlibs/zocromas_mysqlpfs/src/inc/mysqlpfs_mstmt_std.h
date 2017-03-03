#ifndef MAS_MYSQLPFS_MSTMT_STD_H
# define MAS_MYSQLPFS_MSTMT_STD_H

mysqlpfs_mstmt_t **mysqlpfs_mstmt_std_create_array( void );
void mysqlpfs_mstmt_std_reset_array( mysqlpfs_mstmt_t ** mstmts );
void mysqlpfs_mstmt_std_delete_array( mysqlpfs_mstmt_t ** mstmts );

mysqlpfs_mstmt_t *mysqlpfs_mstmt_std_init( mysqlpfs_t * pfs, mysqlpfs_std_id_t stdid );
mysqlpfs_mstmt_t *mysqlpfs_mstmt_std_get( mysqlpfs_t * pfs, mysqlpfs_std_id_t stdid );

#endif
