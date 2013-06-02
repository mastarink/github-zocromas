#ifndef MAS_LISTENER_CONTROL_H
#  define MAS_LISTENER_CONTROL_H

#  include <mastar/types/mas_listener_control_types.h>


unsigned mas_lcontrols_cleaning_transactions__( int removeit, long jwait );
unsigned mas_lcontrol_cleaning_transactions__( mas_lcontrol_t * plcontrol, int removeit, long jwait );

/* unsigned mas_lcontrol_cleaning_transactions_count( mas_lcontrol_t * plcontrol, int removeit, long jwait ); */


int mas_lcontrol_variable_create_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                       const char *txt );
int mas_lcontrol_variable_set_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                    const char *txt );
#ifdef MAS_OLD_VARIABLES_HTTP
mas_variable_t *mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name );
#else
mas_vclass_element_t *mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name );
#endif

#endif
