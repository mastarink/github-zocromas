#ifndef MAS_DUF_PSTMT_LEVINFO_H
#  define MAS_DUF_PSTMT_LEVINFO_H

int duf_pstmt_levinfo_godown_dbopenat_dh( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_node_type_t node_type );

unsigned long long duf_levinfo_count_childs_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_count_childs( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_count_childs_up( const duf_depthinfo_t * pdi );

unsigned long long duf_levinfo_count_gfiles_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_count_gfiles( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_count_gfiles_up( const duf_depthinfo_t * pdi );

#endif
