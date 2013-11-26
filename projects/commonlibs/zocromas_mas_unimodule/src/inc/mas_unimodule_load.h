#ifndef MAS_MODULES_LOAD_H
#  define MAS_MODULES_LOAD_H

#  include <mastar/types/mas_unimodule_types.h>

void *mas_unimodule_load_module_from( const char *libname, const char *modpath, int noerr );
mas_any_fun_t mas_unimodule_load_func_from( const char *libname, const char *funname, const char *modpath );

mas_transaction_fun_t mas_unimodule_load_transaction_func( const char *libname, const char *funname );

void *mas_unimodule_load_symbol_from( const char *libname, const char *symname, const char *modpath );


#endif
