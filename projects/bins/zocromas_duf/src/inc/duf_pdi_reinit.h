#ifndef MAS_DUF_PDI_REINIT_H
#  define MAS_DUF_PDI_REINIT_H

#  include "duf_levinfo_types.h"



int duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu,
                            const duf_sql_set_t * sql_set, int caninsert, int frecursive, int fallow_dirs, int flinear );
int duf_pdi_reinit_defflags_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu,
                            const duf_sql_set_t * sql_set );

/* reinit with no sql_set, no caninsert and recursive:auto, preserving real_path, ufilter and opendir flag */
int duf_pdi_reinit_min( duf_depthinfo_t * pdi );


#endif
