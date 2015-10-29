#ifndef MAS_DUF_LEVINFO_INIT_H
#  define MAS_DUF_LEVINFO_INIT_H

#  include "duf_levinfo_types.h"

void duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d );
void duf_levinfo_init_level( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type );


#endif

