#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/log/mas_log.h>

#include <mastar/types/mas_opts_types.h>

#include <mastar/fileinfo/mas_fileinfo.h>
#include "mas_http_utils.h"
#include "mas_http_reply.h"

#include "mas_http_get.h"

/*
this:
  mas_http_get.c
more:
  mas_http_get.h
  mas_http_reply.c
  mas_http_reply.h
  mas_http_request.c
  mas_http_request.h
  mas_http_types.h
  mas_http_utils.c
  mas_http_utils.h
  mas_transaction.c
  mas_transaction_http.c
  mas_transaction_http.h

  mas_transaction_xcromas.h
*/


mas_http_t *
mas_http_make_out_head_get( mas_rcontrol_t * prcontrol, mas_http_t * http )
{
  if ( http && http->reply_content )
  {
    MAS_LOG( "to make out" );
    MAS_LOG( "SIZE %lu", ( unsigned long ) mas_fileinfo_data_size( http->reply_content ) );

    http = mas_http_make_out_header_simple( http, "Accept-Ranges", "bytes" );
    /* {                                                        */
    /*   if ( http->imethod != MAS_HTTP_METHOD_HEAD )                  */
    /*     http = mas_http_make_body_simple( prcontrol, http ); */
    /* }                                                        */
    MAS_LOG( "to write re status:%d", http->status_code );
  }
  return http;
}
