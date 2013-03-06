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
  mas_basic_def.h
*/

void
mas_channel_buffer_strip( mas_channel_t * pchannel, size_t sz )
{
  size_t oldsz;
  size_t oldlength;
  char *oldbuf;
  ssize_t oldbufiptr;
  ssize_t newsz;
  ssize_t newlength;
  char *newbuf;
  ssize_t newbufiptr;

  /* HMSG( "STRIP %ld", ( unsigned long ) sz ); */
  if ( pchannel )
  {
    int fd_copy;

    fd_copy = pchannel->buffer.fd_copy;
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

      if ( sz < oldsz )
      {
        newsz = oldsz - sz;
        newlength = oldlength - sz;
        if ( newlength < 0 )
          newlength = 0;
        newbuf = mas_malloc( newsz );
        newbufiptr = oldbufiptr - sz;

        memcpy( newbuf, oldbuf + sz, newsz );
        if ( fd_copy )
        {
          int r;

          r = write( fd_copy, oldbuf, sz );
          HMSG( "CUT/COPY %d (%lu)", r, sz );
        }

        pchannel->buffer.buffer = newbuf;
        pchannel->buffer.iptr = newbufiptr;
        pchannel->buffer.size = newsz;
        pchannel->buffer.length = newlength;
      }
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
  mas_channel_buffer_strip( pchannel, pchannel->buffer.iptr );
  if ( pchannel->buffer.fd_copy )
  {
    close( pchannel->buffer.fd_copy );
    pchannel->buffer.fd_copy = 0;
  }
  if ( path )
  {
    pchannel->buffer.fd_copy = open( path, O_CREAT | O_WRONLY, 0644 );
    HMSG( "(%d)OPEN COPY '%s'", pchannel->buffer.fd_copy, path );
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
  /* pchannel->buffer.enddata = 0; */
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
    HMSG( "DEL/COPY %d", r );
  }

  buffer = pchannel->buffer.buffer;
  /* length = pchannel->buffer.length; */
  /* size = pchannel->buffer.size; */
  _mas_channel_set_buffer( pchannel, NULL, 0 );
  if ( buffer )
  {
    HMSG( "DELETE BUFFER" );
    mas_free( buffer );
  }
}

const char *
mas_channel_buffer_find_eol( mas_channel_t * pchannel )
{
  int r = 0;
  const char *ptr;

  ptr = mas_channel_buffer_ptr( pchannel );
  while ( ptr )
  {
    if ( !*ptr )
    {
      ssize_t curlen;

      curlen = ptr - mas_channel_buffer_ptr( pchannel );
      IEVAL( r, mas_channel_read_some( pchannel ) );
      HMSG( "(%d)SOME/EOL %lu L%lu", r, ( unsigned long ) pchannel->buffer.size, ( unsigned long ) pchannel->buffer.length );
      ptr = mas_channel_buffer_ptr( pchannel ) + curlen;
    }
    if ( !*ptr || *ptr == '\n' || *ptr == '\r' )
      break;
    ptr++;
  }
  return ptr;
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
  const char *ptr = NULL;
  const char *begline = NULL;
  size_t sz = 0;

  if ( !mas_channel_buffer_eof( pchannel ) )
  {
    /* begline = mas_channel_buffer_ptr( pchannel ); */
    if ( mas_channel_buffer_ptr( pchannel ) )
    {
      ptr = mas_channel_buffer_find_eol( pchannel );
      if ( *ptr == '\r' || *ptr == '\n' )
      {
        char c;

        c = *ptr++;
        if ( ( *ptr == '\r' || *ptr == '\n' ) && c != *ptr )
          ptr++;
      }
      sz = ( ptr - mas_channel_buffer_ptr( pchannel ) );
      if ( !sz && mas_channel_buffer_eof( pchannel ) )
        begline = NULL;
      else
        begline = mas_channel_buffer_ptr( pchannel );
      mas_channel_set_buffer_ptr( pchannel, ptr );
    }
  }
  if ( psz )
    *psz = sz;
  return begline;
}

int
mas_channel_buffer_enddata( mas_channel_t * pchannel )
{
  /* if ( !pchannel->buffer.buffer )               */
  /*   ( void ) mas_channel_read_some( pchannel ); */
  return pchannel->buffer.enddata;
}
int
mas_channel_buffer_endfile( mas_channel_t * pchannel )
{
  /* if ( !pchannel->buffer.buffer )               */
  /*   ( void ) mas_channel_read_some( pchannel ); */
  return pchannel->buffer.endfile;
}

int
mas_channel_buffer_eof( mas_channel_t * pchannel )
{
  return pchannel->buffer.endfile && mas_channel_buffer_ptr( pchannel ) == pchannel->buffer.buffer + pchannel->buffer.length;
}

void
mas_channel_buffer_set_maxread( mas_channel_t * pchannel, size_t maxr )
{
  pchannel->buffer.maxread = maxr;
}
