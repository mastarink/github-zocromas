#ifndef MULCONF_SOURCE_BASE_H
# define MULCONF_SOURCE_BASE_H

# include "mulconfnt_types.h"

void mucs_source_init( config_source_desc_t * osrc );
config_source_desc_t *mucs_source_create( void );
config_source_desc_t *mucs_source_create_setup( config_source_t source_type, int count, const void *data_ptr, const char *delims,
                                                   const char *eq, const config_prefix_encoder_t * pref_ids );

void mucs_source_close( config_source_desc_t * osrc );
void mucs_source_delete( config_source_desc_t * osrc );

#endif

