#ifndef MULCONF_SOURCE_LIST_BASE_H
# define MULCONF_SOURCE_LIST_BASE_H

# include "mulconfnt_types.h"

void mucs_source_list_init( mucs_source_list_t * list );
mucs_source_list_t *mucs_source_list_create( void );

void mucs_source_list_close( mucs_source_list_t * list );
void mucs_source_list_delete( mucs_source_list_t * source_list );


#endif

