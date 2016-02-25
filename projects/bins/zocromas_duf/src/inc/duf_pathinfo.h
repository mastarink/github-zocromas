#ifndef MAS_DUF_PATHINFO_H
# define MAS_DUF_PATHINFO_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ♠ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ♠ */

int duf_pi_levinfo_set( duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t count );

int duf_pi_set_max_rel_depth( duf_pathinfo_t * pi, const char *real_path, int max_rd );

void duf_pi_clear_d( duf_pathinfo_t * pi, int d );
void duf_pi_clear_up( duf_pathinfo_t * pi );
void duf_pi_clear( duf_pathinfo_t * pi );

void duf_pi_clear_all( duf_pathinfo_t * pi );

#endif
