#ifndef MAS_LISTENER_CONTROL_H
#  define MAS_LISTENER_CONTROL_H

#  include <mastar/types/mas_listener_control_types.h>

#  include <mastar/types/mas_autoset_types.h>
#  include <mastar/types/mas_autoobject_types.h>

unsigned mas_lcontrols_cleaning_transactions__( int removeit, long jwait );
unsigned mas_lcontrol_cleaning_transactions__( mas_lcontrol_t * plcontrol, int removeit, long jwait );

/* unsigned mas_lcontrol_cleaning_transactions_count( mas_lcontrol_t * plcontrol, int removeit, long jwait ); */


int mas_lcontrol_variable_create_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                       const char *txt );
int mas_lcontrol_variable_set_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                    const char *txt );
mas_varvec_element_t *mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name );



mas_autoobject_t *mas_lcontrol_get_autoobject( mas_lcontrol_t * plcontrol, const char *docroot, const char *uri );

#endif
