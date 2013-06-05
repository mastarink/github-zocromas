#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
/* #include <time.h> */
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/channel/mas_channel.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
#  include <mastar/varset/mas_varset.h>
#endif

#include "mas_http_utils.h"

/*
this:
  mas_http_utils.c
more:
  mas_transaction.c
  mas_http_get.c
  mas_http_get.h
  mas_http_reply.c
  mas_http_reply.h
  mas_http_request.c
  mas_http_request.h
  mas_http_types.h
  mas_http_utils.h
  mas_transaction_http.c
  mas_transaction_http.h

*/

char *
mas_proto_http_nonblank( const char *smessage, const char **ppsn )
{
  const unsigned char *ps = NULL, *pse = NULL;

  if ( smessage )
  {
    ps = ( unsigned char * ) smessage;
    while ( *ps && *ps <= ' ' )
      ps++;
    pse = ps;
    while ( *pse && *pse > ' ' )
      pse++;
    if ( ppsn )
    {
      const unsigned char *psn = NULL;

      psn = pse;
      while ( *psn && *psn <= ' ' )
        psn++;
      *ppsn = ( char * ) psn;
    }
  }
  return ps ? mas_strndup( ( char * ) ps, pse - ps ) : NULL;
}

char *
mas_proto_http_nonc( const char *smessage, const char **ppsn, char *delim )
{
  const char *ps, *pse;

  ps = smessage;
  while ( *ps && strchr( delim, *ps ) )
    ps++;
  pse = ps;
  while ( *pse && !strchr( delim, *pse ) )
    pse++;
/* rMSG("WOW '%s'", ps); */
  if ( ppsn )
  {
    const char *psn = NULL;

    psn = pse;
    while ( *psn && strchr( delim, *psn ) )
      psn++;
    *ppsn = psn;
  }
  return mas_strndup( ps, pse - ps );
}


mas_http_t *
mas_proto_http_write( mas_http_t * http, const char *cbuf, size_t sz )
{
  int w = -1;

  if ( http )
  {
    if ( http->prcontrol )
      w = mas_channel_write( http->prcontrol->h.pchannel, cbuf, sz );
    MAS_LOG( "written http %u of %lu", w, ( unsigned long ) sz );
    if ( w > 0 )
      http->written += w;
  }
  return http;
}

mas_http_t *
mas_proto_http_writef( mas_http_t * http, const char *fmt, ... )
{
  va_list args;
  int w = -1;

  va_start( args, fmt );
  if ( http )
  {
    MAS_LOG( "a" );
    if ( http->prcontrol )
      w = mas_channel_vwritef( http->prcontrol->h.pchannel, fmt, args );
    MAS_LOG( "b" );
    if ( w > 0 )
      http->written += w;
  }
  va_end( args );
  return http;
}

mas_http_t *
mas_proto_http_write_pairs( mas_http_t * http, const char *set )
{
  if ( http )
  {
/* TODO !! writev : mas_variables_iovec_pairs */
#ifdef MAS_OLD_VARIABLES_HTTP
    char *buf;
    int bufsize = 1024 * 6;

    buf = mas_malloc( bufsize );


    if ( buf )
    {
      int wm = -1;

      wm = mas_variables_memory_write_pairs( http->outdata, set, buf, bufsize );
      if ( wm > 0 )
      {
        strcpy( buf + wm, "\r\n" );
        wm += 2;
        MAS_LOG( "w.pairs:[%s]", buf );
        HMSG( "HTTP write OUT HEADERS" );
        http = mas_proto_http_write( http, buf, wm );
      }
      mas_free( buf );
    }
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    /* mas_channel_write( http->prcontrol->h.pchannel ... */
    mas_varset_write( mas_channel_fd( http->prcontrol->h.pchannel ), http->outdata, set );
    /* TODO join write in one - modify varset */
    /* TODO http->indata and http->outdata etc should be vclass, not varset */
    write( mas_channel_fd( http->prcontrol->h.pchannel ), "\r\n", 2 );
    /* mas_varset_write( STDERR_FILENO, http->outdata, set ); */
#else
    mas_varset_vclass_write( mas_channel_fd( http->prcontrol->h.pchannel ), http->outdata );
    /* mas_varset_vclass_write( STDERR_FILENO, http->outdata ); */
#endif
  }
  return http;
}
