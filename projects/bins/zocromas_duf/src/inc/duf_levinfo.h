#ifndef DUF_LEVINFO_H
#  define DUF_LEVINFO_H

int duf_levinfo_is_leaf( duf_depthinfo_t * pdi );
int duf_levinfo_is_leaf_d( duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_itemname_d( duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_itemname( duf_depthinfo_t * pdi );


unsigned long long duf_levinfo_dirid_d( duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_dirid_up( duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_dirid( duf_depthinfo_t * pdi );

int duf_levinfo_eod_d( const duf_depthinfo_t * pdi, int d );
void duf_levinfo_set_eod_d( const duf_depthinfo_t * pdi, int d );
void duf_levinfo_set_eod( const duf_depthinfo_t * pdi );

int duf_levinfo_numdir_d( const duf_depthinfo_t * pdi, int d );

void duf_levinfo_clear_li( duf_levinfo_t * pli );
void duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d );

int duf_levinfo_create( duf_depthinfo_t * pdi, const char *path );
int duf_levinfo_delete( duf_depthinfo_t * pdi );

const char *duf_levinfo_path( const duf_depthinfo_t * pdi );

void duf_levinfo_set_context( duf_depthinfo_t * pdi, void *ctx );
void *duf_levinfo_context( duf_depthinfo_t * pdi );
void *duf_levinfo_context_up( duf_depthinfo_t * pdi );

int duf_levinfo_down( duf_depthinfo_t * pdi, unsigned long long dirid, const char *dirname, unsigned long long ndirs,
                      unsigned long long nfiles, int is_leaf );
void duf_levinfo_up( duf_depthinfo_t * pdi );

void duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi );

int duf_levinfo_open_dh( duf_depthinfo_t * pdi, const char *path );
int duf_levinfo_openat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_closeat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d );

int duf_levinfo_udfd( duf_depthinfo_t * pdi );
int duf_levinfo_dfd( duf_depthinfo_t * pdi );
struct stat *duf_levinfo_stat( duf_depthinfo_t * pdi );


duf_dirhandle_t *duf_levinfo_pdh( duf_depthinfo_t * pdi );

#endif