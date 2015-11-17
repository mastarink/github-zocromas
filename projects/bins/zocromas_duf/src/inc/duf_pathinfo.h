#ifndef MAS_DUF_PATHINFO_H
#  define MAS_DUF_PATHINFO_H

int duf_pi_levinfo_calc_depth( duf_pathinfo_t * pi );
int duf_pi_levinfo_set( duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t count );

int duf_pi_set_max_rel_depth( duf_pathinfo_t * pi, const char *real_path, int max_rd );


#endif
