#ifndef DUF_LEVINFO_H
#  define DUF_LEVINFO_H

#  include "duf_levinfo_types.h"

void duf_levinfo_clear_li( duf_levinfo_t * pli );
void duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d );

int duf_levinfo_create( duf_depthinfo_t * pdi, int pathdepth );
int duf_levinfo_delete( duf_depthinfo_t * pdi );


void duf_levinfo_set_context_up_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t fun );

int duf_levinfo_godown( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                        unsigned long long nfiles, int is_leaf );
int duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                                  unsigned long long nfiles, int is_leaf );
int duf_levinfo_goup( duf_depthinfo_t * pdi );

void duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi );


int duf_levinfo_open_dh( duf_depthinfo_t * pdi, const char *path );
int duf_levinfo_openat_dh( duf_depthinfo_t * pdi );

#  include "duf_levinfo_ref.h"



#endif
