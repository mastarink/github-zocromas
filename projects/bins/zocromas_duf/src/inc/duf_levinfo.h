#ifndef DUF_LEVINFO_H
#  define DUF_LEVINFO_H

#  include "duf_levinfo_types.h"

/* void duf_li_clear( duf_levinfo_t * pli ); */
void duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d );
void duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, unsigned long long ndirs,
                               unsigned long long nfiles, int is_leaf, int d );

#endif
