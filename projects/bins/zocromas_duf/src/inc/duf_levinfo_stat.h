#ifndef MAS_DUF_LEVINFO_STAT_H
#  define MAS_DUF_LEVINFO_STAT_H

#  include "duf_levinfo_types.h"

int duf_levinfo_if_statat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_if_statat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_if_statat_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_statat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_statat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_statat_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_if_deleted_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_if_deleted( duf_depthinfo_t * pdi );
int duf_levinfo_if_deleted_up( duf_depthinfo_t * pdi );


#endif
