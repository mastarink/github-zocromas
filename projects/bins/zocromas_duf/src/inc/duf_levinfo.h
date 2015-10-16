#ifndef MAS_DUF_LEVINFO_H
#  define MAS_DUF_LEVINFO_H

#  include "duf_levinfo_types.h"

void duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d );
void duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs,
                               unsigned long long nfiles, duf_node_type_t node_type, int d );
void duf_levinfo_init_level( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs,
                             unsigned long long nfiles, duf_node_type_t node_type );

int duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );


#endif
