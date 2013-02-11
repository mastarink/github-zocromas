#ifndef MAS_SERVER_TRANSACTION_HTTP_H
#  define MAS_SERVER_TRANSACTION_HTTP_H

#  include <mastar/types/mas_transaction_control_types.h>

int mas_proto_http( mas_rcontrol_t * prcontrol, const void *string );

#  undef MAS_WEAK
#endif
