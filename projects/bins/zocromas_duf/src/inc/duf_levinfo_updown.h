#ifndef MAS_DUF_LEVINFO_UPDOWN_H
#  define MAS_DUF_LEVINFO_UPDOWN_H

#  include "duf_levinfo_types.h"



int duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type );
int duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, const char *itemname, duf_node_type_t node_type ); /* XXX equal XXX to duf_levinfo_godown ? XXX */
int duf_levinfo_godown_dbopenat_dh( duf_depthinfo_t * pdi, duf_node_type_t node_type, duf_stmnt_t * pstmt );

int duf_levinfo_goup( duf_depthinfo_t * pdi );


#endif
