#ifndef DUF_LEVINFO_UPDOWN_H
#  define DUF_LEVINFO_UPDOWN_H

#  include "duf_levinfo_types.h"



int duf_levinfo_godown( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                        unsigned long long nfiles, int is_leaf );
int duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                                  unsigned long long nfiles, int is_leaf );
int duf_levinfo_goup( duf_depthinfo_t * pdi );


#endif

