#ifndef MAS_DUF_PDI_TYPES_H
# define MAS_DUF_PDI_TYPES_H
# include <stddef.h>                                                 /* size_t */

struct duf_sccb_handle_s;
typedef struct duf_pdi_context_s duf_pdi_context_t;
typedef struct duf_modcnts_s duf_modcnts_t;
typedef struct duf_depthinfo_s duf_depthinfo_t;
typedef int ( *duf_pdi_cb_t ) ( const duf_depthinfo_t * pdi );
typedef int ( *duf_pdi_scb_t ) ( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, int use_row, const struct duf_sccb_handle_s *sccbh, size_t * pwidth );
typedef struct seq_s seq_t;

#endif

/*
vi: ft=c
*/
