#ifndef MAS_DUF_LEVINFO_COUNT_H
# define MAS_DUF_LEVINFO_COUNT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ♠ */

unsigned long long duf_levinfo_count_childs_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_count_childs( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_count_childs_up( const duf_depthinfo_t * pdi );

unsigned long long duf_levinfo_count_gfiles_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_count_gfiles( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_count_gfiles_up( const duf_depthinfo_t * pdi );

#endif
