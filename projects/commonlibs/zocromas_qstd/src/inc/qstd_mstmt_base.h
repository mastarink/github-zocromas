#ifndef MAS_QSTD_MSTMT_BASE_H
# define MAS_QSTD_MSTMT_BASE_H

# include "qstd_types.h"

void mas_qstd_init( mas_qstd_t * qstd, const char *host, const char *user, const char *passwd, const char *db, int port );
mas_qstd_t *mas_qstd_create( void );
mas_qstd_t *mas_qstd_create_setup( const char *host, const char *user, const char *passwd, const char *db, int port );
mas_qstd_t *mas_qstd_instance_setup( const char *host, const char *user, const char *passwd, const char *db, int port );
mas_qstd_t *mas_qstd_instance( void );

void mas_qstd_reset( mas_qstd_t * qstd );
void mas_qstd_delete( mas_qstd_t * qstd );
void mas_qstd_instance_delete( void );

mysqlpfs_mstmt_t *mas_qstd_mstmt_create_setup( mas_qstd_t * qstd, int nparams, int nresults, const char *sqlop );
mysqlpfs_mstmt_t *mas_qstd_instance_mstmt_create_setup( int nparams, int nresults, const char *sqlop );
void mas_qstd_mstmt_delete( mysqlpfs_mstmt_t * mstmt );

/***/
int mas_qstd_create_tables( mas_qstd_t * qstd );
int mas_qstd_drop_tables( mas_qstd_t * qstd );

mysqlpfs_mstmt_t **mas_qstd_mstmt_create_array( void );
void mas_qstd_mstmt_reset_array( mysqlpfs_mstmt_t ** mstmts );
void mas_qstd_mstmt_delete_array( mysqlpfs_mstmt_t ** mstmts );

mysqlpfs_mstmt_t *mas_qstd_mstmt_init( mas_qstd_t * qstd, mas_qstd_id_t stdid );
mysqlpfs_mstmt_t *mas_qstd_mstmt_get( mas_qstd_t * qstd, mas_qstd_id_t stdid );

#endif
