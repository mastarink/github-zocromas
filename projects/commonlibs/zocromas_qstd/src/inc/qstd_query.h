#ifndef MAS_QSTD_QUERY_H
# define MAS_QSTD_QUERY_H

# include "qstd_types.h"

int mas_qstd_query( mas_qstd_t * qstd, const char *op );

/* mysqlpfs_s_mysql_t *mas_qstd_mysql( mas_qstd_t * qstd ); */
const char *mas_qstd_mysql_error( mas_qstd_t * qstd );
int mas_qstd_update_summary( mas_qstd_t * qstd );

int mas_qstd_start_transaction( mas_qstd_t * qstd );
int mas_qstd_end_transaction( mas_qstd_t * qstd );


#endif

