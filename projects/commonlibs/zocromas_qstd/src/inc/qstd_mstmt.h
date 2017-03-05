#ifndef MAS_QSTD_MSTMT_H
# define MAS_QSTD_MSTMT_H

# include "qstd_types.h"

void mas_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port );
mas_qstd_t *mas_qstd_create( void );
mas_qstd_t *mas_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port );
void mas_qstd_reset( mas_qstd_t * qstd );
void mas_qstd_delete( mas_qstd_t * qstd );

/***/

mysqlpfs_mstmt_t **mas_qstd_mstmt_create_array( void );
void mas_qstd_mstmt_reset_array( mysqlpfs_mstmt_t ** mstmts );
void mas_qstd_mstmt_delete_array( mysqlpfs_mstmt_t ** mstmts );

mysqlpfs_mstmt_t *mas_qstd_mstmt_init( mas_qstd_t * qstd, mas_qstd_id_t stdid );
mysqlpfs_mstmt_t *mas_qstd_mstmt_get( mas_qstd_t * qstd, mas_qstd_id_t stdid );

mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id, const char *sdetype );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                          const char *sdetype );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                          const char *sdetype );

#endif
