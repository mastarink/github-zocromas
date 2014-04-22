#ifndef DUF_LEVINFO_H
#  define DUF_LEVINFO_H


int duf_levinfo_create( duf_depthinfo_t * pdi );
int duf_levinfo_delete( duf_depthinfo_t * pdi );

int duf_levinfo_down( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                      unsigned long long nfiles );
void duf_levinfo_up( duf_depthinfo_t * pdi );

void duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi );

int duf_levinfo_openat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_closeat_dh( duf_depthinfo_t * pdi );

#endif
