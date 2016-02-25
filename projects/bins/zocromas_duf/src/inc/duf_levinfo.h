#ifndef MAS_DUF_LEVINFO_H
# define MAS_DUF_LEVINFO_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */

int duf_levinfo_calc_depth( duf_depthinfo_t * pdi );

void duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d );

/*
 * duf_levinfo_ refers functions working with levinfo array at pdi
 * duf_li_ refers functions working with levinfo array itself
 * */

#endif
