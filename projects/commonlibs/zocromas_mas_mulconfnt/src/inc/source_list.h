#ifndef MULCONF_SOURCE_LIST_H
# define MULCONF_SOURCE_LIST_H

# include "mulconfnt_types.h"

void mulconfnt_source_list_integrate( config_source_list_t * list );
config_source_desc_t *mulconfnt_source_list_add_source( config_source_list_t * list, config_source_t source_type, int count, void *data_ptr,
                                                        const char *delims, const char *eq, const config_prefix_encoder_t * pref_ids );

#endif
