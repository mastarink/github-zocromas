#ifndef MAS_QSTD_MSTMT_PARENTS_H
# define MAS_QSTD_MSTMT_PARENTS_H

# include "qstd_types.h"

unsigned long long mas_qstd_mstmt_selget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id );
unsigned long long mas_qstd_mstmt_insget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id );
unsigned long long mas_qstd_mstmt_selinsget_parents_id( mas_qstd_t * qstd,  unsigned long long dir_id);
unsigned long long mas_qstd_mstmt_insselget_parents_id( mas_qstd_t * qstd, unsigned long long dir_id);

#endif
