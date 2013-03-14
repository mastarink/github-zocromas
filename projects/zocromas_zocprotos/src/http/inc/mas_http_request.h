#ifndef MAS_HTTP_REQUEST_H
#  define MAS_HTTP_REQUEST_H

#  include "mas_http_types.h"

mas_http_t *mas_proto_http_create_request( mas_rcontrol_t * prcontrol );
mas_http_t *mas_proto_http_parse_request( mas_rcontrol_t * prcontrol, const mas_transaction_protodesc_t * proto_desc, mas_http_t * http );
const char *mas_http_method_name( mas_http_t * http );
int mas_proto_http_delete_request( mas_http_t * http );

mas_http_method_t mas_proto_http_parse_method( mas_http_t * http );

const char *mas_proto_http_method_name( mas_http_t * http );

#endif
