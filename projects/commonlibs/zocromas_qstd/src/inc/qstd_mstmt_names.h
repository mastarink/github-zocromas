#ifndef MAS_QSTD_MSTMT_NAMES_H
# define MAS_QSTD_MSTMT_NAMES_H

# include "qstd_types.h"



mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id, const char *sdetype );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_selinsget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                          const char *sdetype );
mysqlpfs_s_ulonglong_t mas_qstd_mstmt_insselget_names_id( mas_qstd_t * qstd, const char *name, mysqlpfs_s_ulonglong_t parent_id,
                                                          const char *sdetype );

#endif

