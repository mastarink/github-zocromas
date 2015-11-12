#ifndef MAS_DUF_PATHINFO_CREDEL_H
#  define MAS_DUF_PATHINFO_CREDEL_H

int duf_pi_levinfo_calc_depth( duf_pathinfo_t * pi );
int duf_pi_levinfo_set( duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t count );

int duf_pi_levinfo_create( duf_pathinfo_t * pi, size_t count );
void duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li );

int duf_pi_shut( duf_pathinfo_t * pi );
int duf_pi_set_max_rel_depth( duf_pathinfo_t * pi, const char *real_path, int max_rd );


#endif
