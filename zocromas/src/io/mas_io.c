#include "mas_basic_def.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "mas_io.h"

/*
this:
  mas_io.c
related:
  mas_messsage_io.c

 
 */


int
mas_fwrite_string( FILE * f, char *cbuf, int z )
{
  int w = -1;
  int msgsz;

  if ( f )
  {
    tMSG( "to write" );
    msgsz = cbuf ? strlen( cbuf ) + ( z ? 1 : 0 ) : 0;
    w = mas_fwrite( cbuf, 1, msgsz, f );
    if ( w != msgsz )
    {
      EMSG( "written %d of %d", w, msgsz );
    }
  }
  return w;
}

int
mas_write_string( int fd, char *cbuf, int z )
{
  int w = -1;
  int msgsz;

  if ( fd > 0 )
  {
    tMSG( "to write" );
    msgsz = cbuf ? strlen( cbuf ) + ( z ? 1 : 0 ) : 0;
    w = write( fd, cbuf, msgsz );
    if ( w != msgsz )
    {
      EMSG( "written %d of %d", w, msgsz );
    }
  }
  return w;
}

int
mas_vwritef( int fd, const char *fmt, va_list args )
{
  int w = 0;
  char buf[4096];

  vsnprintf( buf, sizeof( buf ), fmt, args );
  w = mas_write_string( fd, buf, 0 );
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  return w;
}

int
mas_writef( int fd, const char *fmt, ... )
{
  int w = 0;

  /* char buf[4096]; */
  va_list args;

  va_start( args, fmt );
  /* vsnprintf( buf, sizeof( buf ), fmt, args ); */
  /* w = mas_write_string( fd, buf, 0 ); */
  w = mas_vwritef( fd, fmt, args );
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  va_end( args );
  return w;
}

int
mas_vfprintf( FILE * f, const char *fmt, va_list args )
{
  int w = 0;

  /* char buf[4096]; */

  w = vfprintf( f, fmt, args );
  /* vsnprintf( buf, sizeof( buf ), fmt, args ); */
  /* w = mas_fwrite_string( f, buf, 0 );         */
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  return w;
}

int
mas_fprintf( FILE * f, const char *fmt, ... )
{
  int w = 0;

  /* char buf[4096]; */
  va_list args;

  va_start( args, fmt );
  /* vsnprintf( buf, sizeof( buf ), fmt, args ); */
  /* w = mas_write_string( fd, buf, 0 ); */
  w = mas_vfprintf( f, fmt, args );
  if ( w < 0 )
  {
    EMSG( "written %d", w );
  }
  va_end( args );
  return w;
}

/* mas_cmdmod_do.c */
size_t
mas_fread( void *ptr, size_t size, size_t nmemb, FILE * stream )
{
  size_t r;

  r = fread( ptr, size, nmemb, stream );
  return r;
}

size_t
mas_fwrite( const void *ptr, size_t size, size_t nmemb, FILE * stream )
{
  size_t w;
  int rf;

  w = fwrite( ptr, size, nmemb, stream );
  rf = fflush( stream );
  mMSG( "written %ld rf:%d", w, rf );
  return w;
}

int
mas_read_buf( int fd, char **pbuf )
{
  int rtot = -1;
  int r = 0;
  char *tbuf = NULL;
  char sbuf[1030];

  if ( fd > 0 )
  {
    rtot = 0;
    do
    {
      r = read( fd, sbuf, sizeof( sbuf ) - 5 );
      if ( r > 0 )
      {
        tbuf = mas_strncat_x( tbuf, sbuf, sizeof( sbuf ) - 5 );
        rtot += r;
      }
    }
    while ( r > 0 );
    if ( r < 0 )
    {
      rtot = r;
    }
    else
    {
      if ( pbuf )
      {
        *pbuf = tbuf;
      }
      else
      {
        mas_free( tbuf );
      }
    }
  }
  return rtot;
}

int
mas_read_string( int fd, char **pbuf )
{
  char c;
  char buf[1030];
  int r = -1;

  if ( fd > 0 )
  {
    r = 0;
    if ( pbuf )
      *pbuf = NULL;
    for ( int i = 0; i < sizeof( buf ) - 5; i++ )
    {
      c = 0;
      buf[i + 1] = 0;
      r = read( fd, &c, 1 );
      if ( r >= 0 )
      {
        buf[i] = c;
        if ( c == '\n' )
        {
          buf[i] = 0;
          mMSG( "buf: '%s'", buf );
          r = i;
          break;
        }
        else if ( c == '\r' )
        {
          buf[i] = 0;
          mMSG( "buf: '%s'", buf );
          r = i;
          break;
        }
        else if ( !c )
        {
          mMSG( "buf: '%s'", buf );
          r = i;
          break;
        }
      }
      else if ( r == 0 )
      {
        buf[i] = 0;
      }
      else
      {
        buf[0] = 0;
      }
    }
    if ( *buf && pbuf )
    {
      *pbuf = mas_strdup( buf );
    }
  }
  return r;
}

#ifndef MAS_CHANNEL_STREAM_READ
int
mas_read_all( int fd, char **pbuf, size_t * psz )
#else
int
mas_fread_all( FILE * stream, char **pbuf, size_t * psz )
#endif
{
  ssize_t tsz = -1;
  void *t = NULL;
  char *buf = NULL;
  size_t bufsz = 1024 * 4;
  ssize_t readsz = 0;
  ssize_t rsz;

  MAS_LOG( "to read io (read all)" );
#ifndef MAS_CHANNEL_STREAM_READ
  if ( fd > 0 )
#else
  if ( stream )
#endif
  {
    buf = mas_malloc( bufsz );
    tsz = 0;
    rsz = bufsz - 5;

    if ( pbuf )
      *pbuf = NULL;
    do
    {
      mMSG( "to read 'all'" );
#ifndef MAS_CHANNEL_STREAM_READ
      readsz = read( fd, buf, rsz );
#else
      readsz = mas_fread( buf, 1, rsz, stream );
#endif
      mMSG( "read 'all' %ld", readsz );
      buf[readsz] = 0;
      /* tMSG( "r:[%d] [%s]", readsz, buf ); */
      if ( readsz > 0 )
      {
        size_t xsz = 0;
        unsigned char *x = NULL;

        xsz = tsz + readsz;
        x = mas_malloc( xsz + 1 );
        tMSG( "new size:%lu (%lu+%lu)", xsz, tsz, readsz );
        if ( t )
        {
          memcpy( x, t, tsz );
          x[tsz] = 0;
          mas_free( t );
        }
        memcpy( x + tsz, buf, readsz );
        x[tsz + readsz] = 0;
        t = x;
        tsz = xsz;
        /* tMSG( "%u new str:[%s]", tsz, t ); */
      }
    }
    while ( readsz == rsz );
    /* tMSG( "%u t:[%s]", tsz, t ); */
    if ( pbuf )
    {
      /* tMSG( "%u ret t:[%s]", tsz, t ); */
      *pbuf = t;
      if ( psz )
        *psz = tsz;
    }
    else
    {
      EMSG( "Wow" );
      mas_free( t );
    }
    mas_free( buf );
  }
  return tsz > 0 ? tsz : readsz;
}
