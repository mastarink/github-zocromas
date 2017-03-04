#ifndef MAS_MYSQLPFS_TYPES_H
# define MAS_MYSQLPFS_TYPES_H
# include <mysql.h>

typedef enum mysqlpfs_std_id_e mysqlpfs_std_id_t;
typedef struct mas_qstd_s mas_qstd_t;

typedef enum mysqlpfs_mfield_type_e mysqlpfs_mfield_type_t;
typedef struct mysqlpfs_s mysqlpfs_t;
typedef struct mysqlpfs_mstmt_s mysqlpfs_mstmt_t;
typedef unsigned long mysqlpfs_s_length_t;
typedef my_bool mysqlpfs_s_bool_t;
typedef my_ulonglong mysqlpfs_s_ulonglong_t;
typedef struct mysqlpfs_mbind_s mysqlpfs_mbind_t;

typedef MYSQL mysqlpfs_s_mysql_t;
typedef MYSQL_RES mysqlpfs_s_result_t;
typedef MYSQL_STMT mysqlpfs_s_stmt_t;
typedef MYSQL_ROW mysqlpfs_s_row_t;
typedef MYSQL_BIND mysqlpfs_s_bind_t;

typedef int ( *mysqlpfs_row_cb_t ) ( mysqlpfs_s_row_t row, int num );

#endif
