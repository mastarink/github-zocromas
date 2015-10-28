#ifndef MAS_DUF_PSTMT_LEVINFO_H
#  define MAS_DUF_PSTMT_LEVINFO_H

int duf_pstmt_levinfo_godown_dbopenat_dh( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_node_type_t node_type );

#  ifdef DUF_NO_NUMS
int duf_levinfo_make_childs_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_make_childs( duf_depthinfo_t * pdi );
int duf_levinfo_make_childs_up( duf_depthinfo_t * pdi );
#  endif

#endif
