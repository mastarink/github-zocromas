#ifndef MAS_DUF_PDI_FILTERS_H
# define MAS_DUF_PDI_FILTERS_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

const duf_ufilter_t *duf_pdi_pu( const duf_depthinfo_t * pdi );
const duf_yfilter_t *duf_pdi_py( const duf_depthinfo_t * pdi );

int duf_pdi_max_filter( const duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );

#endif
