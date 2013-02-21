#ifndef MAS_HTTP_UTILS_H
#  define MAS_HTTP_UTILS_H

#  include <mastar/types/mas_http_types.h>


char *mas_proto_http_nonblank( const char *smessage, const char **ppsn );
char *mas_proto_http_nonc( const char *smessage, const char **ppsn, char *delim );

mas_http_t *mas_proto_http_write_pairs( mas_http_t * http, const char *set );
/* mas_http_t *mas_proto_http_write_values( mas_http_t * http, const char *set ); */

mas_http_t *mas_proto_http_write( mas_http_t * http, char *cbuf, size_t sz );
/* mas_http_t *mas_proto_http_vwritef( mas_http_t * http, const char *fmt, va_list args ); */
mas_http_t *mas_proto_http_writef( mas_http_t * http, const char *fmt, ... );

mas_http_t *mas_proto_http_set_path( mas_rcontrol_t * prcontrol, mas_http_t * http );


#endif
