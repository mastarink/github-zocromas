#ifndef MAS_MODULES_LOAD_H
#  define MAS_MODULES_LOAD_H

/* mas_void_fun_t @ */
#  include <mastar/types/mas_unimodule_types.h>

/* build full path to library `libname` and load dl */
void *mas_unimodule_load_module_from( const char *libname, const char *modpath, int noerr );

/* load dl.symbol `symname` from library `libname`; look at `modpath` */
void *mas_unimodule_load_symbol_from( const char *libname, const char *symname, const char *modpath );

/* load dl.symbol `funname` of type mas_void_fun_t from library `libname`; look at `modpath` */
mas_void_fun_t mas_unimodule_load_func_from( const char *libname, const char *funname, const char *modpath );


#endif
