#ifndef DUF_PDI_H
#  define DUF_PDI_H

#  include "duf_levinfo_types.h"

duf_depthinfo_t *duf_pdi_create( void );

int duf_pdi_reset( duf_depthinfo_t * pdi, int frecursive, int opendir );

int DUF_WRAPPED( duf_pdi_init ) ( duf_depthinfo_t * pdi, const char *real_path, int caninsert,
                                  const duf_sql_set_t * sql_set /* const char *node_selector2 */ , int recursive,
                                  int opendir );

#  if 0
int duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, int caninsert, const duf_ufilter_t * pu, const char *node_selector2, int recursive,
                    int opendir );
#  endif
int duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set /* const char *node_selector2 */ , int recursive, int opendir );
int duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, int caninsert,
                            const duf_sql_set_t * sql_set /* const char *node_selector2 */  );
int duf_pdi_close( duf_depthinfo_t * pdi );





#endif
