#ifndef MAS_RCONTROL_OBJECT_H
#  define MAS_RCONTROL_OBJECT_H

#  include "transaction/inc/mas_transaction_control_types.h"

mas_rcontrol_t *mas_rcontrol_create( void );
int mas_rcontrol_register( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol );

#  ifdef MAS_TR_PERSIST
int mas_rcontrol_init( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol, unsigned persistent_transaction );
mas_rcontrol_t *mas_rcontrol_make( mas_lcontrol_t * plcontrol, unsigned persistent_transaction );
#  else
int mas_rcontrol_init( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol );
mas_rcontrol_t *mas_rcontrol_make( mas_lcontrol_t * plcontrol );
#  endif
int mas_rcontrol_delete( mas_rcontrol_t * prcontrol, int toclose );


#endif

