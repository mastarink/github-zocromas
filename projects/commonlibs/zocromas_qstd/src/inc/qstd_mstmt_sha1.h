#ifndef MAS_QSTD_MSTMT_SHA1_H
# define MAS_QSTD_MSTMT_SHA1_H

# include "qstd_types.h"

unsigned long long mas_qstd_mstmt_selget_sha1_id( mas_qstd_t * qstd, const const unsigned char *sha1 );
unsigned long long mas_qstd_mstmt_insget_sha1_id( mas_qstd_t * qstd, unsigned long long data_id, const unsigned char *sha1 );
unsigned long long mas_qstd_mstmt_selinsget_sha1_id( mas_qstd_t * qstd, unsigned long long data_id, const unsigned char *sha1 );
unsigned long long mas_qstd_mstmt_insselget_sha1_id( mas_qstd_t * qstd, unsigned long long data_id, const unsigned char *sha1 );
unsigned long long mas_qstd_mstmt_selinsget_sha1dref( mas_qstd_t * qstd, unsigned long long data_id, unsigned long long sha1_id );

#endif
