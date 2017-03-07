#ifndef MAS_MYSQLPFS_TYPES_MY_H
# define MAS_MYSQLPFS_TYPES_MY_H
# include <mysql.h>

typedef MYSQL mysqlpfs_s_mysql_t;
typedef MYSQL_RES mysqlpfs_s_result_t;
typedef MYSQL_STMT mysqlpfs_s_stmt_t;
typedef MYSQL_ROW mysqlpfs_s_row_t;
typedef MYSQL_BIND mysqlpfs_s_bind_t;

typedef my_bool mysqlpfs_s_bool_t;
typedef my_ulonglong mysqlpfs_s_ulonglong_t;

typedef int ( *mysqlpfs_row_cb_t ) ( mysqlpfs_s_row_t row, int num );

#endif
