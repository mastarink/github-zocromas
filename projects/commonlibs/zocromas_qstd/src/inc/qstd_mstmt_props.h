#ifndef MAS_QSTD_MSTMT_PROPS_H
# define MAS_QSTD_MSTMT_PROPS_H

# include "qstd_types.h"

mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype,
                                                       const masxfs_stat_t * stat );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype,
                                                          const masxfs_stat_t * stat );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_props_id( mas_qstd_t * qstd, mysqlpfs_s_ulonglong_t data_id, const char *sdetype,
                                                          const masxfs_stat_t * stat );

#endif
