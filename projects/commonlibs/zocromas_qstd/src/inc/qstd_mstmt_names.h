#ifndef MAS_QSTD_MSTMT_NAMES_H
# define MAS_QSTD_MSTMT_NAMES_H

# include "qstd_types.h"

unsigned long long mas_qstd_mstmt_selget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id );
unsigned long long mas_qstd_mstmt_insget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id,
                                                   const char *sdetype );
unsigned long long mas_qstd_mstmt_selinsget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id,
                                                      const char *sdetype );
unsigned long long mas_qstd_mstmt_insselget_names_id( mas_qstd_t * qstd, const char *name, unsigned long long updir_id, unsigned long long data_id,
                                                      const char *sdetype );

#endif
