#ifndef MUCS_SOURCE_LIST_H
# define MUCS_SOURCE_LIST_H

# include "mulconfnt_types.h"

void mucs_source_list_integrate( mucs_source_list_t * list );
mucs_source_t *mucs_source_list_add_source_x( mucs_source_list_t * list, mucs_source_type_t source_type, int count, const void *data_ptr,
                                              const char *delims, const char *eq, const mucs_prefix_encoder_t * pref_ids );
mucs_source_t *mucs_source_list_add_source( mucs_source_list_t * list, mucs_source_type_t source_type, int count, const void *data_ptr );

int mucs_source_list_lookup_all( mucs_source_list_t * source_list, const mucs_option_table_list_t * tablist, void *userdata );

#endif
