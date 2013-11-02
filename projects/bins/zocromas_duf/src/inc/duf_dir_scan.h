#ifndef MAS_DUF_DIR_SCAN_H
#  define MAS_DUF_DIR_SCAN_H

int duf_print_dirs_by_parentid( unsigned long long parentid, duf_dirinfo_t * pdi );
int duf_print_dirs( const char *path, int recursive );

#endif
