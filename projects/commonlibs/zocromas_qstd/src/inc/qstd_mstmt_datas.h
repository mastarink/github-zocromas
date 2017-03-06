#ifndef MAS_QSTD_MSTMT_DATAS_H
# define MAS_QSTD_MSTMT_DATAS_H

# include "qstd_types.h"

mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_datas_id( mas_qstd_t * qstd, const masxfs_stat_t * stat );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_datas_id( mas_qstd_t * qstd, const masxfs_stat_t * stat );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_datas_id( mas_qstd_t * qstd, const masxfs_stat_t * stat );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_datas_id( mas_qstd_t * qstd, const masxfs_stat_t * stat );

#endif
