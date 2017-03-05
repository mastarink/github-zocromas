#ifndef MAS_QSTD_MSTMT_PARENTS_H
# define MAS_QSTD_MSTMT_PARENTS_H

# include "qstd_types.h"

mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_parents_id( mas_qstd_t * qstd,  mysqlpfs_s_ulonglong_t dir_id);
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_parents_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t dir_id);

#endif
