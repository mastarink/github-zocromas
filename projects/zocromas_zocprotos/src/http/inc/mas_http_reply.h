#ifndef MAS_HTTP_REPLY_H
#  define MAS_HTTP_REPLY_H

#  include "mas_http_types.h"

mas_http_t *mas_http_make_etag( mas_rcontrol_t * prcontrol, mas_http_t * http );
mas_http_t *mas_http_make_docroot( mas_rcontrol_t * prcontrol, mas_http_t * http );

mas_http_t *mas_http_make_body_simple( mas_rcontrol_t * prcontrol, mas_http_t * http );
mas_http_t *mas_http_make_body( mas_rcontrol_t * prcontrol, mas_http_t * http, const char *format, ... );

mas_http_t *mas_http_make_out_header_simple( mas_http_t * http, const char *name, const char *value );
mas_http_t *mas_http_make_out_header( mas_http_t * http, const char *name, const char *fmt, ... );

mas_http_t *mas_http_make_out_std_headers( mas_rcontrol_t * prcontrol, mas_http_t * http );

mas_http_t *mas_http_reply( MAS_PASS_OPTS_DECLARE mas_rcontrol_t * prcontrol, mas_http_t * http );
mas_http_t *mas_http_reply_test( mas_rcontrol_t * prcontrol, mas_http_t * http );
const char *mas_http_status_code_message( mas_rcontrol_t * prcontrol, mas_http_t * http );

mas_http_t *mas_http_make_data_auto( MAS_PASS_OPTS_DECLARE mas_rcontrol_t * prcontrol, mas_http_t * http );

#endif
