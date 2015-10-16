#ifndef MAS_DUF_PDI_REINIT_H
#  define MAS_DUF_PDI_REINIT_H

#  include "duf_levinfo_types.h"


#  if 0
int duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu, const char *node_selector2, int caninsert, int recursive,
                    int opendir );
#  endif


/* int duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set, int recursive, int opendir ); */

int duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu,
                            const duf_sql_set_t * sql_set, int caninsert, int frecursive );

/* reinit with no sql_set, no caninsert and recursive:auto, preserving real_path, ufilter and opendir flag */
int duf_pdi_reinit_min( duf_depthinfo_t * pdi );


#endif
