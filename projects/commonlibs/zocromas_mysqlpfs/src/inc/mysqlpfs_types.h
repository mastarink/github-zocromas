#ifndef MAS_MYSQLPFS_TYPES_H
# define MAS_MYSQLPFS_TYPES_H
# include <mysql.h>

typedef struct mysqlpfs_s mysqlpfs_t;
typedef MYSQL_RES mysqlpfs_result_t;
typedef MYSQL_ROW mysqlpfs_row_t;

typedef int ( *mysqlpfs_row_cb_t ) ( mysqlpfs_row_t row, int num );

#endif
