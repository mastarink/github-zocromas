#ifndef MAS_DUF_PATHINFO_DEPTH_H
# define MAS_DUF_PATHINFO_DEPTH_H

# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ♠ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ♠ */

int duf_pi_calc_depth( duf_pathinfo_t * pi );

int duf_pi_godown( duf_pathinfo_t * pi, duf_node_type_t node_type, unsigned frecursive, unsigned flinear );
void duf_pi_countdown_dirs( duf_pathinfo_t * pi );

int duf_pi_is_good_depth_d( const duf_pathinfo_t * pdi, int delta, unsigned frecursive, unsigned flinear, int d );
int duf_pi_is_good_depth( const duf_pathinfo_t * pdi, int delta, unsigned frecursive, unsigned flinear );

int duf_pi_check_depth( const duf_pathinfo_t * pi, duf_node_type_t node_type, unsigned frecursive, unsigned flinear );

#endif
