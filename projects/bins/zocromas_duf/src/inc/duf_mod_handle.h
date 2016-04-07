#ifndef MAS_DUF_MOD_HANDLE_H
# define MAS_DUF_MOD_HANDLE_H

# include "duf_mod_types.h"

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

char **duf_liblist( int *psize );
void duf_delete_liblist( char **liblist );

void *duf_load_symbol( const char *libname, const char *symbol );
const duf_mod_handler_t *duf_load_mod_handler_symbol( const char *libname );
void *duf_load_mod_handler_symbol_find( const char *libname, const char *masmodsymbol );

#endif
