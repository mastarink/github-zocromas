#ifndef MAS_QSTD_MSTMT_DATAS_H
# define MAS_QSTD_MSTMT_DATAS_H

/* # include <mastar/levinfo/masxfs_levinfo_types.h> */

# include "qstd_types.h"

unsigned long long mas_qstd_mstmt_selget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat );
unsigned long long mas_qstd_mstmt_insget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat );
unsigned long long mas_qstd_mstmt_selinsget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat );
unsigned long long mas_qstd_mstmt_insselget_datas_id( mas_qstd_t * qstd, const mas_qstd_stat_t * stat );

#endif
