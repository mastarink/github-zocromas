#ifndef MAS_LOAD_MODULE_H
#  define MAS_LOAD_MODULE_H

#include <mastar/types/mas_modules_types.h>

void *mas_modules_load_module( const char *libname );
mas_cmd_fun_t mas_modules_load_func( const char *libname, const char *funname );
mas_cmd_t *mas_modules_load_subtable( const char *libname );


#  undef MAS_WEAK
#endif

