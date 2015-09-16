#ifndef DUF_LEVINFO_UPDOWN_H
#  define DUF_LEVINFO_UPDOWN_H

#  include "duf_levinfo_types.h"



int duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, int is_leaf );
int duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, const char *itemname, int is_leaf );
int duf_levinfo_godown_dbopenat_dh( duf_depthinfo_t * pdi, int is_leaf, duf_stmnt_t * pstmt );

int duf_levinfo_goup( duf_depthinfo_t * pdi );


#endif
