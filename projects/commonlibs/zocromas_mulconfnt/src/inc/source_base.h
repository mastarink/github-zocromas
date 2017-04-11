#ifndef MUCS_SOURCE_BASE_H
# define MUCS_SOURCE_BASE_H

# include "mulconfnt_types.h"


void mucs_source_init( mucs_source_t * osrc );
mucs_source_t *mucs_source_create( void );
mucs_source_t *mucs_source_create_setup( mucs_source_type_t source_type, int count, const void *data_ptr, int min_pass, const char *delims,
                                         const char *eq, const mucs_prefix_encoder_t * pref_ids );

void mucs_source_reset( mucs_source_t * osrc );
void mucs_source_delete( mucs_source_t * osrc );

#endif
