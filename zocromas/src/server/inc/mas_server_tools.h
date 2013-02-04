#ifndef MAS_SERVER_TOOLS_H
#  define MAS_SERVER_TOOLS_H

#  include "xcromas/inc/mas_message_types.h"
#  include "transaction/inc/mas_transaction_control_types.h"


unsigned mas_clients_count( mas_rcontrol_t * prcontrol );
void do_quit_server( mas_rcontrol_t * prcontrol, bin_type_t * pbinary );
const char *mas_sstatus( mas_status_t status );

#  undef MAS_WEAK
#endif
