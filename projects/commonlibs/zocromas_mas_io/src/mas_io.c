#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>

#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>



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
    /* tMSG( "to write" ); */
    msgsz = cbuf ? strlen( cbuf ) + ( z ? 1 : 0 ) : 0;
    w = mas_fwrite( cbuf, 1, msgsz, f );
    if ( w != msgsz )
    {
      /* EMSG( "written %d of %d", w, msgsz ); */
    }
  }
  return w;
}

int
mas_write_string( int fd, const char *cbuf, int z )
{
  int w = -1;
  int msgsz;

  if ( fd > 0 )
  {
    /* tMSG( "to write" ); */
    msgsz = cbuf ? strlen( cbuf ) + ( z ? 1 : 0 ) : 0;
    w = write( fd, cbuf, msgsz );
    if ( w != msgsz )
    {
      /* EMSG( "written %d of %d", w, msgsz ); */
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
    /* EMSG( "written %d", w ); */
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
    /* EMSG( "written %d", w ); */
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
    /* EMSG( "written %d", w ); */
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
    /* EMSG( "written %d", w ); */
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

  /* int rf; */

  w = fwrite( ptr, size, nmemb, stream );
  /* rf = fflush( stream ); */
  ( void ) fflush( stream );
  /* mMSG( "written %ld rf:%d", w, rf ); */
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
          /* mMSG( "buf: '%s'", buf ); */
          r = i;
          break;
        }
        else if ( c == '\r' )
        {
          buf[i] = 0;
          /* mMSG( "buf: '%s'", buf ); */
          r = i;
          break;
        }
        else if ( !c )
        {
          /* mMSG( "buf: '%s'", buf ); */
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
mas_read_all_new_bad( int fd, char **pbuf, size_t * psz, size_t maxsz )
#else
int
mas_fread_all_new_bad( FILE * stream, char **pbuf, size_t * psz, size_t maxsz )
#endif
{
  ssize_t totread = 0;
  ssize_t tsz = -1;
  void *t = NULL;
  char *buf = NULL;
  ssize_t readsz = 0;
  ssize_t rsz;
  size_t bufsz;

  bufsz = 1024 * 4 + 5;

  WMSG( "TO READ io (read all)" );
#ifndef MAS_CHANNEL_STREAM_READ
  if ( fd > 0 )
#else
  if ( stream )
#endif
  {
    buf = mas_malloc( bufsz );

    tsz = 0;
    if ( pbuf )
    {
      t = *pbuf;
      if ( psz )
        tsz = *psz;
      maxsz += tsz;
    }
    rsz = bufsz - 5;

    do
    {
      ssize_t xmaxsz, rsza;

      xmaxsz = tsz + rsz;
      rsza = rsz;
      if ( maxsz && xmaxsz > maxsz )
      {
        rsza = rsz - xmaxsz + maxsz;
      }
      WMSG( "TO READ io : rsz:%lu; rsza:%lu; maxsz:%lu; xmaxsz:%lu", ( unsigned long ) rsz, ( unsigned long ) rsza, ( unsigned long ) maxsz,
            ( unsigned long ) xmaxsz );
      if ( rsza <= 0 )
        break;
      {
        /* mMSG( "to read 'all'" ); */
#ifndef MAS_CHANNEL_STREAM_READ
        readsz = read( fd, buf, rsza );
#else
        readsz = mas_fread( buf, 1, rsza, stream );
#endif
        WMSG( "READ io 'all' %ld", ( unsigned long ) readsz );
        /* tMSG( "r:[%d] [%s]", readsz, buf ); */
        if ( readsz > 0 )
        {
          size_t xsz = 0;
          unsigned char *x = NULL;

          totread += readsz;
          xsz = tsz + readsz;
          x = mas_malloc( xsz + 1 );
          /* tMSG( "new size:%lu (%lu+%lu)", xsz, tsz, readsz ); */
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
      /* EMSG( "Wow" ); */
      mas_free( t );
    }
    mas_free( buf );
  }
  return totread;
}

void
mas_io_append2buffer( char **px_buffer, ssize_t * px_buffer_size, char *read_buf, ssize_t readsz )
{
  size_t xsz = 0;
  char *x = NULL;

  xsz = *px_buffer_size + readsz;
  x = mas_malloc( xsz + 1 );
  /* tMSG( "new size:%lu (%lu+%lu)", xsz, x_buffer_size, readsz ); */
  if ( *px_buffer )
  {
    memcpy( x, *px_buffer, *px_buffer_size );
    x[*px_buffer_size] = 0;
    mas_free( *px_buffer );
    *px_buffer = NULL;
  }
  memcpy( x + *px_buffer_size, read_buf, readsz );
  x[xsz] = 0;
  *px_buffer = x;
  *px_buffer_size = xsz;
  /* tMSG( "%u new str:[%s]", x_buffer_size, x_buffer ); */
}

#ifndef MAS_CHANNEL_STREAM_READ
int
mas_io_read_some( int fd, char **px_buffer, size_t * pxbuffer_size, size_t maxsz )
#else
int
mas_io_fread_some( FILE * stream, char **px_buffer, size_t * pxbuffer_size, size_t maxsz )
#endif
{
  ssize_t totread = 0;
  ssize_t x_buffer_size = -1;
  char *x_buffer = NULL;
  ssize_t readsz = 0;
  ssize_t rsz;
  size_t bufsz;

  bufsz = 2048 * 4 + 4;

  /* MAS_LOG( "to read io (read some)" ); */
#ifndef MAS_CHANNEL_STREAM_READ
  if ( fd > 0 )
#else
  if ( stream )
#endif
  {
    char *read_buf = NULL;

    read_buf = mas_malloc( bufsz );
    x_buffer_size = 0;
    rsz = bufsz - 5;
    /* if ( px_buffer )     */
    /*   *px_buffer = NULL; */
    if ( px_buffer )
    {
      x_buffer = *px_buffer;
      if ( pxbuffer_size )
        x_buffer_size = *pxbuffer_size;
    }
    do
    {
      if ( rsz > maxsz - totread )
        rsz = maxsz - totread;
      readsz = 0;
      if ( rsz <= 0 )
        break;
      {
	EVAL_PREPARE;
        WMSG( "TO READ io 'SOME' %lu maxsz:%lu; totread:%lu", ( unsigned long ) rsz, ( unsigned long ) maxsz, ( unsigned long ) totread );
        if ( rsz > bufsz )
        {
          HMSG( "WOOOOOOOOOOOOOO !TO READ 'SOME' %lu", ( unsigned long ) rsz );
        }
#ifndef MAS_CHANNEL_STREAM_READ
        IEVAL( readsz, read( fd, read_buf, rsz ) );
#else
        IEVAL( readsz, mas_fread( read_buf, 1, rsz, stream ) );
#endif
        WMSG( "READ 'SOME' %ld of %ld (e%d)", ( unsigned long ) readsz, ( unsigned long ) rsz, errno );
      }
      if ( readsz > 0 )
      {
        read_buf[readsz] = 0;
        mas_io_append2buffer( &x_buffer, &x_buffer_size, read_buf, readsz );
        totread += readsz;
      }
      /* WMSG( "READ %lu (tot %lu) max %lu x_buffer_size %lu {%s}", ( unsigned long ) readsz, ( unsigned long ) totread, */
      /*       ( unsigned long ) maxsz, ( unsigned long ) x_buffer_size, x_buffer );                                     */
      if ( readsz == rsz )
      {
        if ( readsz )
        {
          WMSG( "READ 'SOME' BUFFER FULL %ld", ( signed long ) readsz );
        }
        else
        {
          WMSG( "READ 'SOME' BUFFER EMPTY %ld", ( signed long ) readsz );
        }
      }
      else
      {
        WMSG( "READ 'SOME' BUFFER PART %ld", ( signed long ) readsz );
      }
    }
    while ( readsz == rsz );
    /* if ( peof )                                                                        */
    /* {                                                                                  */
    /*   if ( readsz != rsz || readsz == 0 )                                              */
    /*     *peof = 1;                                                                     */
    /*   else                                                                             */
    /*     *peof = 0;                                                                     */
    /*   WMSG( "READ 'SOME' SET BF %s", *peof ? ( readsz ? "PART" : "EMPTY" ) : "FULL" ); */
    /* }                                                                                  */
    /* tMSG( "%u x_buffer:[%s]", x_buffer_size, x_buffer ); */
    if ( px_buffer )
    {
      /* tMSG( "%u ret x_buffer:[%s]", x_buffer_size, x_buffer ); */
      *px_buffer = x_buffer;
      if ( pxbuffer_size )
        *pxbuffer_size = x_buffer_size;
    }
    mas_free( read_buf );
  }
  WMSG( "READ '/SOME' %lu", ( unsigned long ) totread );
  return totread;
}
