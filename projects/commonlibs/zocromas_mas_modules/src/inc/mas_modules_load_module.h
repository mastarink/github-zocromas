#ifndef MAS_MODULES_LOAD_MODULE_H
#  define MAS_MODULES_LOAD_MODULE_H

#  include <mastar/types/mas_modules_types.h>

void *mas_modules_load_module_from( const char *libname, const char *modpath, int noerr );

/* void *mas_modules_load_module( const char *libname ); */
/* void *mas_modules_load_proto( const char *libname ); */
mas_any_fun_t mas_modules_load_func_from( const char *libname, const char *funname, const char *modpath );

mas_cmd_fun_t mas_modules_load_cmd_func( const char *libname, const char *funname );

/* mas_transaction_fun_t mas_modules_load_proto_func( const char *libname, const char *funname ); */

mas_transaction_fun_t mas_modules_load_transaction_func( const char *libname, const char *funname );

/* mas_cmd_t *mas_modules_load_subtable_from( const char *libname, const char *modpath ); */
void *mas_modules_load_symbol_from( const char *libname, const char *symname, const char *modpath );

#endif
