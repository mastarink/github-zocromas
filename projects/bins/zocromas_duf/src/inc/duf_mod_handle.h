#ifndef MAS_DUF_MOD_HANDLE_H
# define MAS_DUF_MOD_HANDLE_H

# include "duf_mod_types.h"

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* 20160422.160939 */
/* char **duf_liblist( const char *libpat, int *psize ); */
DRX( MOD, char **, liblist, NULL, liblist, const char *pat, int *psize );

/* void duf_delete_liblist( char **liblist ); */
DRN( MOD, void, delete_liblist, char **liblist );

/* void *duf_load_symbol( const char *libname, const char *symbol, void **plibhan ); */
DRP( MOD, void *, psym, NULL, load_symbol, const char *libname, const char *symbol, void **plibhan );

/* const duf_mod_handler_t *duf_load_mod_handler_symbol( const char *libname, void **plibhan ); */
DRP( MOD, const duf_mod_handler_t *, mhan, NULL, load_mod_handler_symbol, const char *libname, void **plibhan );

/* void *duf_load_mod_handler_symbol_find( const char *libname, const char *masmodsymbol ); */
DRP( OTHER, void *, ptr, NULL, load_mod_handler_symbol_find, const char *libname, const char *masmodsymbol );

#endif
