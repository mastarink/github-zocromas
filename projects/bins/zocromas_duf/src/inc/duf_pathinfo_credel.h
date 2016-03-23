#ifndef MAS_DUF_PATHINFO_CREDEL_H
# define MAS_DUF_PATHINFO_CREDEL_H

# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

void duf_pi_levinfo_delete( duf_pathinfo_t * pi );

void duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li );

void duf_pi_init_level_d( duf_pathinfo_t * pi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d );
void duf_pi_dbinit_level_d( duf_pathinfo_t * pi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d );

DR( PI, pi_shut, duf_pathinfo_t * pi );
DR( PI, pi_levinfo_create, duf_pathinfo_t * pi, size_t count );

#endif
