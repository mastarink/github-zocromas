#ifndef MAS_DUF_SERVICE_H
#  define MAS_DUF_SERVICE_H

int duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format );
int duf_check_field( const char *name, int have );
const char *duf_dbg_funname( duf_anyhook_t p );

#endif
