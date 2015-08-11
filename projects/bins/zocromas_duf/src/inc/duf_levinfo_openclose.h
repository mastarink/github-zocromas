#ifndef DUF_LEVINFO_OPENCLOSE_H
#  define DUF_LEVINFO_OPENCLOSE_H

#  include "duf_levinfo_types.h"

int duf_levinfo_statat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_statat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_statat_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_open_dh( duf_depthinfo_t * pdi, const char *path );

int duf_levinfo_openat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_openat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_openat_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_dbopenat_dh( duf_depthinfo_t * pdi, duf_sqlite_stmt_t * pstmt, int is_leaf );


int duf_levinfo_opened_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_opened_dh( duf_depthinfo_t * pdi );
int duf_levinfo_opened_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_closeat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_closeat_dh_up( duf_depthinfo_t * pdi );



#endif
