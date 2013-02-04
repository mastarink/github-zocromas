#ifndef MAS_LISTENER_CONTROL_H
#  define MAS_LISTENER_CONTROL_H

#  include "mas_listener_control_types.h"


unsigned mas_lcontrols_cleaning_transactions( int removeit, long jwait );
unsigned mas_lcontrol_cleaning_transactions( mas_lcontrol_t * plcontrol, int removeit, long jwait );
/* unsigned mas_lcontrol_cleaning_transactions_count( mas_lcontrol_t * plcontrol, int removeit, long jwait ); */


int mas_lcontrol_variable_create_text( mas_lcontrol_t * plcontrol, th_type_t thtype, const char *vclass, const char *name,
                                       const char *txt );
int mas_lcontrol_variable_set_text( mas_lcontrol_t * plcontrol, th_type_t thtype, const char *vclass, const char *name,
                                    const char *txt );
mas_variable_t *mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass, const char *name );


#endif
