#ifndef MUCS_OPTION_INTERFACE_H
# define MUCS_OPTION_INTERFACE_H

# include "mulconfnt_types.h"

void mucs_option_interface_add_source( mucs_option_interface_t * interface, mucs_source_type_t source_type, int count, const void *data_ptr,
                                       int min_pass );
int mucs_option_interface_lookup_source_list( mucs_option_interface_t * interface, mucs_source_list_t * source_list, void *userdata );
int mucs_option_interface_lookup_all( mucs_option_interface_t * interface, void *userdata );
int mucs_option_interface_lookup_all_multipass( mucs_option_interface_t * interface, void *userdata, int passes );

#endif
