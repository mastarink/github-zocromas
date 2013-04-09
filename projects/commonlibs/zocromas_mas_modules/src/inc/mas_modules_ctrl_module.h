#ifndef MAS_MODULES_CTRL_MODULE_H
#  define MAS_MODULES_CTRL_MODULE_H

#  include <mastar/types/mas_modules_types.h>

void mas_modules_register_module( const char *name, void *module_handle );
void mas_modules_unregister( void );

#endif
