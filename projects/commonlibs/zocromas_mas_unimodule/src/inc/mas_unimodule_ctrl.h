#ifndef MAS_UNIMODULE_CTRL_H
#  define MAS_MODULES_CTRL_H

#  include <mastar/types/mas_unimodule_types.h>

void mas_unimodule_register_module( const char *name, void *module_handle );
void mas_unimodule_unregister( void );


#endif
