#ifndef MAS_QSTD_MSTMT_SIZES_H
# define MAS_QSTD_MSTMT_SIZES_H

# include "qstd_types.h"

mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_sizes_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t size );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_sizes_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t size );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_sizes_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t size );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_sizes_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t size );

#endif
