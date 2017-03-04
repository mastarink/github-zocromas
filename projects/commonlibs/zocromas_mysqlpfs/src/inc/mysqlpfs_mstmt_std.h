#ifndef MAS_MYSQLPFS_MSTMT_STD_H
# define MAS_MYSQLPFS_MSTMT_STD_H

void mysqlpfs_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port );
mas_qstd_t *mysqlpfs_qstd_create( void );
mas_qstd_t *mysqlpfs_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port );
void mysqlpfs_qstd_reset( mas_qstd_t * qstd );
void mysqlpfs_qstd_delete( mas_qstd_t * qstd );

/***/

mysqlpfs_mstmt_t **mysqlpfs_mstmt_std_create_array( void );
void mysqlpfs_mstmt_std_reset_array( mysqlpfs_mstmt_t ** mstmts );
void mysqlpfs_mstmt_std_delete_array( mysqlpfs_mstmt_t ** mstmts );

mysqlpfs_mstmt_t *mysqlpfs_mstmt_std_init( mas_qstd_t * qstd, mysqlpfs_std_id_t stdid );
mysqlpfs_mstmt_t *mysqlpfs_mstmt_std_get( mas_qstd_t * qstd, mysqlpfs_std_id_t stdid );

mysqlpfs_s_ulonglong_t mysqlpfs_mstmt_std_selget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id );
mysqlpfs_s_ulonglong_t mysqlpfs_mstmt_std_insget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                           const char *sdetype );
mysqlpfs_s_ulonglong_t mysqlpfs_mstmt_std_selinsget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                              const char *sdetype );
mysqlpfs_s_ulonglong_t mysqlpfs_mstmt_std_insselget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                              const char *sdetype );

#endif
