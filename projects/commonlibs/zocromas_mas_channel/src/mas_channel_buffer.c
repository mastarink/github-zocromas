#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <arpa/inet.h>

#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/tools/mas_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/io/mas_io.h>

#include "mas_channel_object.h"
#include "mas_channel_listen.h"
#include "mas_channel.h"
#include "mas_channel_buffer.h"

/*
this:
  mas_channel_buffer.c
related:
  mas_channel.c
  mas_channel_listen.c
  mas_channel_open.c
  mas_channel_types.h
more:
  mas_channel.h
  mas_channel_listen.h
  mas_channel_open.h
  
  mas_io.h
  mas_io.c
  mas_client.c
  
  mas_transaction_control.c
*/

void
mas_channel_buffer_strip_to( mas_channel_t * pchannel, size_t sz, int dontcopy )
{
  if ( !sz )
    sz = pchannel->buffer.length / 2;
  if ( pchannel->buffer.length > sz )
  {
    mas_channel_buffer_strip( pchannel, pchannel->buffer.length - sz, dontcopy );
  }
}

void
mas_channel_buffer_strip( mas_channel_t * pchannel, size_t sz, int dontcopy )
{
  size_t oldsz = 0;
  size_t oldlength = 0;
  char *oldbuf = NULL;
  ssize_t oldbufiptr = 0;
  ssize_t newsz = 0;
  ssize_t newlength = 0;
  char *newbuf = NULL;
  ssize_t newbufiptr = 0;

  if ( pchannel )
  {
    oldbuf = pchannel->buffer.buffer;
    oldsz = pchannel->buffer.size;
    oldlength = pchannel->buffer.length;
    if ( oldbuf )
    {
      oldbufiptr = pchannel->buffer.iptr;
      pchannel->buffer.size = 0;
      pchannel->buffer.length = 0;
      pchannel->buffer.buffer = NULL;
      pchannel->buffer.iptr = 0;

      /* is there something yet ? */
      if ( sz < oldsz )
      {
        newsz = oldsz - sz;
        newlength = oldlength - sz;
        if ( newlength < 0 )
          newlength = 0;
        newbuf = mas_malloc( newsz );
        newbufiptr = oldbufiptr - sz;

        memcpy( newbuf, oldbuf + sz, newsz );
      }
      if ( !dontcopy )
      {
        int fd_copy;

        fd_copy = pchannel->buffer.fd_copy;
        if ( fd_copy )
        {
          /*
             int r;

             r = */ write( fd_copy, oldbuf, sz );
          /* HMSG( "CUT/COPY %d (%lu)", r, sz ); */
        }
      }
      pchannel->buffer.buffer = newbuf;
      pchannel->buffer.iptr = newbufiptr;
      pchannel->buffer.size = newsz;
      pchannel->buffer.length = newlength;

      mas_free( oldbuf );
    }
  }
}

char *
mas_channel_buffer_start( mas_channel_t * pchannel )
{
  return pchannel->buffer.buffer;
}

char *
mas_channel_buffer( mas_channel_t * pchannel, size_t * psz )
{
  char *b = NULL;

  if ( !pchannel->buffer.buffer )
    ( void ) /* r = */ mas_channel_read_some( pchannel );
  if ( psz )
    *psz = pchannel->buffer.size;
  b = pchannel ? pchannel->buffer.buffer : NULL;
  return b;
}

ssize_t
mas_channel_buffer_iptr( mas_channel_t * pchannel )
{
  if ( !pchannel->buffer.buffer )
    ( void ) /* r = */ mas_channel_read_some( pchannel );
  return pchannel->buffer.iptr;
}

const char *
mas_channel_buffer_ptr( mas_channel_t * pchannel )
{
  if ( !pchannel->buffer.buffer )
    ( void ) /* r = */ mas_channel_read_some( pchannel );
  return pchannel->buffer.buffer + pchannel->buffer.iptr;
}

void
mas_channel_set_buffer_ptr( mas_channel_t * pchannel, const char *ptr )
{
  pchannel->buffer.iptr = ptr - pchannel->buffer.buffer;
}

void
mas_channel_set_buffer_copy( mas_channel_t * pchannel, const char *path )
{
  mas_channel_buffer_strip( pchannel, pchannel->buffer.iptr, 0 );
  if ( pchannel->buffer.fd_copy )
  {
    /* int r;

       r = */ close( pchannel->buffer.fd_copy );
    /* HMSG( "(%d)CLOSE COPY", r ); */
    pchannel->buffer.fd_copy = 0;
  }
  if ( path )
  {
    pchannel->buffer.fd_copy = mas_open( path, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
    WMSG( "(%d)OPEN COPY '%s'", pchannel->buffer.fd_copy, path );
    if ( pchannel->buffer.fd_copy < 0 )
    {
      perror( "OPEN COPY : " );
    }
  }
}

static void
_mas_channel_set_buffer( mas_channel_t * pchannel, char *buffer, size_t size )
{
  pchannel->buffer.buffer = buffer;
  pchannel->buffer.iptr = 0;
  pchannel->buffer.length = 0;
  pchannel->buffer.size = size;
}

void
mas_channel_delete_buffer( mas_channel_t * pchannel )
{
  char *buffer;

  /* size_t length; */
  /* size_t size; */
  if ( pchannel->buffer.fd_copy )
  {
    int r;

    r = write( pchannel->buffer.fd_copy, pchannel->buffer.buffer, pchannel->buffer.length );
    WMSG( "DEL/COPY %d", r );
  }

  buffer = pchannel->buffer.buffer;
  /* length = pchannel->buffer.length; */
  /* size = pchannel->buffer.size; */
  _mas_channel_set_buffer( pchannel, NULL, 0 );
  if ( buffer )
  {
    WMSG( "DELETE BUFFER" );
    mas_free( buffer );
  }
}

const char *
mas_channel_buffer_find_eol( mas_channel_t * pchannel )
{
  int r = 0;

  while ( 1 /* !mas_channel_buffer_eof( pchannel ) */  )
  {
    if ( mas_channel_buffer_eob( pchannel ) )
    {
      /* mas_channel_buffer_strip_to( pchannel, 0, 0 ); */
      IEVAL( r, mas_channel_read_some( pchannel ) );
      WMSG( "(%d)SOME/EOL %lu L%lu", r, ( unsigned long ) pchannel->buffer.size, ( unsigned long ) pchannel->buffer.length );
    }
    if ( !pchannel->buffer.buffer[pchannel->buffer.iptr] || pchannel->buffer.buffer[pchannel->buffer.iptr] == '\n'
         || pchannel->buffer.buffer[pchannel->buffer.iptr] == '\r' || mas_channel_buffer_eof( pchannel ) )
      break;
    pchannel->buffer.iptr++;
  }
  return mas_channel_buffer_ptr( pchannel );
}

char *
mas_channel_buffer_nl_dup( mas_channel_t * pchannel )
{
  const char *cs = NULL;
  char *ps = NULL;
  size_t sz = 0;

  cs = mas_channel_buffer_nl( pchannel, &sz );
  if ( cs && sz )
    ps = mas_strndup( cs, sz );
  return ps;
}

/* set ptr to end of line from current, return current + size */
const char *
mas_channel_buffer_nl( mas_channel_t * pchannel, size_t * psz )
{
  const char *begline = NULL;
  size_t iptr0 = 0;
  size_t sz = 0;

  iptr0 = pchannel->buffer.iptr;
  if ( !mas_channel_buffer_eof( pchannel ) )
  {
    /* begline = mas_channel_buffer_ptr( pchannel ); */
    {
      mas_channel_buffer_find_eol( pchannel );
      if ( pchannel->buffer.buffer[pchannel->buffer.iptr] == '\r' || pchannel->buffer.buffer[pchannel->buffer.iptr] == '\n' )
      {
        char c;

        c = pchannel->buffer.buffer[pchannel->buffer.iptr++];
        if ( ( pchannel->buffer.buffer[pchannel->buffer.iptr] == '\r' || pchannel->buffer.buffer[pchannel->buffer.iptr] == '\n' )
             && c != pchannel->buffer.buffer[pchannel->buffer.iptr] && !mas_channel_buffer_eof( pchannel ) )
          pchannel->buffer.iptr++;
      }
      sz = ( pchannel->buffer.iptr - iptr0 );
      /* HMSG( "!!!! SZ: %lu P: %p %p", sz, pchannel->buffer.buffer, pchannel->buffer.buffer + pchannel->buffer.length ); */
      if ( !sz && mas_channel_buffer_eof( pchannel ) )
        begline = NULL;
      else
        begline = &pchannel->buffer.buffer[iptr0];
    }
  }
  if ( psz )
    *psz = sz;
  return begline;
}

const char *
mas_channel_search( mas_channel_t * pchannel, const char *needle, size_t len, mas_voidp_fun_t cb, void *arg )
{
  size_t match = 0;
  size_t niptr = 0;

  {
    /* HMSG( "TO SEARCH  %lu iptr:%lu len:%lu L:%lu [%d:%d]", match, pchannel->buffer.iptr, len, pchannel->buffer.length, */
    /*       mas_channel_buffer_eof( pchannel ), mas_channel_buffer_eob( pchannel ) );                                    */
    while ( match < len && !mas_channel_buffer_eof( pchannel ) /*??? && len < pchannel->buffer.size */  )
    {
      if ( mas_channel_buffer_eob( pchannel ) /* || !pchannel->buffer.buffer */  )
      {
        int r;

        mas_channel_buffer_strip_to( pchannel, 0, 0 );
        IEVAL( r, mas_channel_read_some( pchannel ) );
        if ( cb )
          ( cb ) ( arg );
      }
      if ( !mas_channel_buffer_eob( pchannel ) )
      {
        /* HMSG( "LOOP %lu iptr:%lu len:%lu L:%lu [%d:%d]", match, pchannel->buffer.iptr, len, pchannel->buffer.length, */
        /*       mas_channel_buffer_eof( pchannel ), mas_channel_buffer_eob( pchannel ) );                              */
        if ( needle[niptr] == pchannel->buffer.buffer[pchannel->buffer.iptr] )
        {
          match++;
          /* HMSG( "   MATCH %lu of %lu N[%lu]:'%c' %02x ? S[%lu]:'%c' %02x", match, len, niptr, needle[niptr] >= ' ' ? needle[niptr] : '.', */
          /*       needle[niptr], pchannel->buffer.iptr,                                                                                     */
          /*       pchannel->buffer.buffer[pchannel->buffer.iptr] >= ' ' ? pchannel->buffer.buffer[niptr] : '.',                             */
          /*       pchannel->buffer.buffer[niptr] );                                                                                         */
          niptr++;
          pchannel->buffer.iptr++;
        }
        else
        {
          /* HMSG( "DISMATCH %lu of %lu N[%lu]:'%c' %02x ? S[%lu]:'%c' %02x", match, len, niptr, needle[niptr] >= ' ' ? needle[niptr] : '.', */
          /*       needle[niptr], pchannel->buffer.iptr,                                                                                     */
          /*       pchannel->buffer.buffer[pchannel->buffer.iptr] >= ' ' ? pchannel->buffer.buffer[niptr] : '.',                             */
          /*       pchannel->buffer.buffer[pchannel->buffer.iptr] );                                                                         */
          pchannel->buffer.iptr -= match;
          pchannel->buffer.iptr++;
          match = 0;
          niptr = 0;
        }
      }
    }
  }
  if ( match )
    pchannel->buffer.iptr -= len;
  return match ? mas_channel_buffer_ptr( pchannel ) : NULL;
}

int
mas_channel_buffer_endfile( mas_channel_t * pchannel )
{
  /* if ( !pchannel->buffer.buffer )               */
  /*   ( void ) mas_channel_read_some( pchannel ); */
  return pchannel->buffer.endfile;
}

int
mas_channel_buffer_eob( mas_channel_t * pchannel )
{
  return mas_channel_buffer_iptr( pchannel ) >= pchannel->buffer.length;
}

int
mas_channel_buffer_eof( mas_channel_t * pchannel )
{
  /* return pchannel->buffer.endfile && mas_channel_buffer_ptr( pchannel ) >= pchannel->buffer.buffer + pchannel->buffer.length; */
  return pchannel->buffer.endfile && mas_channel_buffer_eob( pchannel );
}

void
mas_channel_buffer_set_maxread( mas_channel_t * pchannel, size_t maxr )
{
  pchannel->buffer.maxread = maxr;
}
