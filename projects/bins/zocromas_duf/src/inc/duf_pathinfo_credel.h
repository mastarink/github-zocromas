#ifndef MAS_DUF_PATHINFO_CREDEL_H
#  define MAS_DUF_PATHINFO_CREDEL_H

# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ♠ */

void duf_pi_levinfo_delete( duf_pathinfo_t * pi );
int duf_pi_levinfo_create( duf_pathinfo_t * pi, size_t count );
void duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li );

void duf_pi_init_level_d( duf_pathinfo_t * pi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d );
void duf_pi_dbinit_level_d( duf_pathinfo_t * pi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );

int duf_pi_shut( duf_pathinfo_t * pi );


#endif
