#ifndef MULCONF_SOURCE_LIST_BASE_H
# define MULCONF_SOURCE_LIST_BASE_H

# include "mulconfnt_types.h"

void mulconfnt_source_list_init( config_source_list_t * list );
config_source_list_t *mulconfnt_source_list_create( void );

void mulconfnt_source_list_close( config_source_list_t * list );
void mulconfnt_source_list_delete( config_source_list_t * source_list );


#endif

