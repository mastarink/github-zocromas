#ifndef MAS_DUF_SERVICE_H
#  define MAS_DUF_SERVICE_H

int duf_print_file_info( FILE * f, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format );
int duf_check_field( const char *name, int have );
const char *duf_dbg_funname( duf_anyhook_t p );

int duf_openat_dh( duf_dirhandle_t * pdh, const duf_dirhandle_t * pdhu, const char *name );
int duf_open_dh( duf_dirhandle_t * pdh, const char *path );
int duf_close_dh( duf_dirhandle_t * pdh );
int duf_check_dh( const char *msg );
int duf_statat_dh( duf_dirhandle_t * pdh, const duf_dirhandle_t * pdhu, const char *name );

#endif
