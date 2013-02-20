#  define MSG_VERSION 0x20130210
#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/io/mas_io.h>
#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_open.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_message_io.h"


/*
this:
  mas_messsage_io.c
related:
  mas_client.c

*/


#ifndef MAS_CHANNEL_STREAM_WRITE
int
mas_write_message( int fd, const char *cbuf, mas_header_t * header )
#else
int
mas_fwrite_message( FILE * stream, const char *cbuf, mas_header_t * header )
#endif
{
  int r = -1;

  /* int rf=0; */
  int len = 0;
  int msgsz = 0;
  mas_message_t *msg = NULL;

#ifndef MAS_CHANNEL_STREAM_WRITE
  if ( fd > 0 )
#else
  if ( stream )
#endif
  {
    ioMSG( "header : %p", ( void * ) header );
    /* HMSG( "writing message" ); */
    if ( header )
    {
      ioMSG( "header sign : %lx", ( unsigned long ) header->sign );
      if ( header->sign != MSG_SIGNATURE )
      {
        EMSG( "header wrong sign before write: %lx", ( unsigned long ) header->sign );
      }
      len = header->len;
      ioMSG( "len from header : %u", len );
    }
    if ( ( !header || !header->binary ) && cbuf )
    {
      /* const char *p = cbuf + strlen( cbuf );  */
      /*                                         */
      /* while ( p > cbuf && *( p - 1 ) <= ' ' ) */
      /* {                                       */
      /*   --p;                                  */
      /*   *p = 0;                               */
      /* }                                       */
      len = strlen( cbuf ) + 1;
    }
    msgsz = sizeof( mas_header_t ) + len;
    ioMSG( "w. to malloc %d", msgsz );
    msg = mas_malloc( msgsz );
    ioMSG( "w. malloc %d - %p", msgsz, ( void * ) msg );
    ioMSG( "w. to memset msg" );
    memset( msg, 0, msgsz );
    ioMSG( "w. memset'd msg" );
    {
      if ( header )
        msg->h = *header;
      if ( cbuf )
      {
        ioMSG( "w. to memcpy message to %p; len:%lu", ( void * ) msg->message, strlen( msg->message ) );
        memcpy( msg->message, cbuf, len );
        ioMSG( "w.  memcpy'd message" );
      }
      ioMSG( "w. to fill header" );
      msg->h.sign = MSG_SIGNATURE;
      msg->h.vers = MSG_VERSION;
      msg->h.len = len;
      msg->h.sender_stamp = ctrl.stamp;
      msg->h.pid = getpid(  );
      msg->h.pth = pthread_self(  );
      ioMSG( "to write h/msg %d", msgsz );
      /* HMSG( "to write h/msg %d", msgsz ); */
#ifndef MAS_CHANNEL_STREAM_WRITE
      r = write( fd, msg, msgsz );
      /* rf=0; */
#else
      r = mas_fwrite( msg, 1, msgsz, stream );
      /* rf = fflush( stream ); */
#endif
      /* HMSG( "fwritten h/msg %d", r ); */
      /* EMSG( "WRITE r:%d of %d v.%x", r, msgsz, msg->h.vers ); */
      if ( r != msgsz )
      {
        EMSG( "written %d of %d", r, msgsz );
        P_ERR;
      }
      ioMSG( "written(sent); new opts:%d; size: %d", msg->h.new_opts, msgsz );
    }
    mas_free( msg );
  }
  else
  {
    r = -1;
  }
  if ( r < 0 )
  {
    EMSG( "written %d", r );
  }
  return r;
}

#ifndef MAS_CHANNEL_STREAM_READ
int
mas_read_message( int fd, char **pbuf, mas_header_t * pheader )
#else
int
mas_fread_message( FILE * stream, char **pbuf, mas_header_t * pheader )
#endif
{
  int r = -1, rh = -1, rmsg = -1;
  int msghsz = 0;
  int msgsz = 0;
  mas_header_t *msgh = NULL;


#ifndef MAS_CHANNEL_STREAM_READ
  if ( fd > 0 )
#else
  if ( stream )
#endif
  {
    rmsg = 0;
    if ( pbuf )
      *pbuf = NULL;

    msghsz = sizeof( mas_header_t );
    msgh = mas_malloc( msghsz );
    memset( msgh, 0, msghsz );
    ioMSG( "to read header, a.hsz:%d", msghsz );
    ioMSG( "vvvvvvvvvvvvvvvvvvvvvvvvvvvv %lu", ( unsigned long ) time( NULL ) );
#ifndef MAS_CHANNEL_STREAM_READ
    rh = read( fd, msgh, msghsz );
#else
    rh = mas_fread( msgh, 1, msghsz, stream );
#endif
    /* EMSG( "READ rh:%d of %d : %x : %x", rh, msghsz, msgh->sign, msgh->vers ); */

    if ( rh > 0 && rmsg >= 0 )
    {
      rmsg += rh;
      if ( rh == msghsz && msgh->len > 0 )
      {
        mas_message_t *msg = NULL;

        if ( msgh->vers != MSG_VERSION )
        {
          EMSG( "version not match: %x ? %x : %s", msgh->vers, MSG_VERSION, ( char * ) msgh );
          EMSG( "(sign)%lx : %lx", ( unsigned long ) msgh->sign, ( unsigned long ) MSG_SIGNATURE );
        }
        else
        {
          ioMSG( "version OK" );
        }
        if ( msgh->sign != MSG_SIGNATURE )
        {
          EMSG( "wrong signature: %lx", ( unsigned long ) msgh->sign );
          /* sleep (3); */
        }
        else
        {
          ioMSG( "signature OK" );
          ioMSG( "(vers)%x : %x", msgh->vers, MSG_VERSION );
          ioMSG( "(sign)%lx : %lx", ( unsigned long ) msgh->sign, ( unsigned long ) MSG_SIGNATURE );

          if ( pheader )
          {
            *pheader = *msgh;
            /*
             *pheader->sender_pid = msgh->h.sender_pid;
             *pheader->sender_vdate = msgh->h.sender_vdate;
             *pheader->sender_vtime = msgh->h.sender_vtime;
             */
          }

          msgsz = sizeof( mas_header_t ) + msgh->len;
          msg = mas_malloc( msgsz );
          ioMSG( "to read header, a.sz:%d; len:%d", msgsz, msgh->len );
          memset( msg, 0, msgsz );
#ifndef MAS_CHANNEL_STREAM_READ
          r = read( fd, msg->message, msgh->len );
#else
          r = mas_fread( msg->message, 1, msgh->len, stream );
#endif
          /* EMSG( "READ message rh:%d of %d", r, msgh->len ); */
          if ( r > 0 )
          {
            if ( pbuf )
            {
              msg->h = *msgh;
              ioMSG( "got; new opts:%d; size:%d / %d", msg->h.new_opts, msgsz, msg->h.len );
              *pbuf = mas_malloc( msg->h.len );
              ioMSG( "buf: a.sz:%d", msg->h.len );
              memcpy( *pbuf, msg->message, msg->h.len );
              /* ioMSG( "got msg: %s", *pbuf ); */
            }
            else
            {
              EMSG( "mas_read_message error can't pass msg" );
            }
            if ( r > 0 && rmsg >= 0 )
              rmsg += r;
            else
              rmsg = -1;
          }
          mas_free( msg );
        }
      }
      else
      {
        EMSG( "header size (%d : %d)", rh, msghsz );
        rmsg = -1;
      }
    }
    else
    {
      if ( rh )
      {
        EMSG( "header (%d)", rh );
      }
      rmsg = -1;
    }

    mas_free( msgh );
  }
  else
  {
    rmsg = -1;
  }
  return rmsg;
}

int
mas_channel_read_message( const mas_channel_t * pchannel, char **pbuf, mas_header_t * pheader )
{
  int r;

#ifndef MAS_CHANNEL_STREAM_READ
  r = mas_read_message( mas_channel_fd( pchannel ), pbuf, pheader );
#else
  r = mas_fread_message( mas_channel_stream( pchannel ), pbuf, pheader );
#endif
  return r;
}

int
mas_channel_write_message( const mas_channel_t * pchannel, const char *cbuf, mas_header_t * pheader )
{
  int w;

  /* HMSG( "to write message" ); */
#ifndef MAS_CHANNEL_STREAM_WRITE
  w = mas_write_message( mas_channel_fd( pchannel ), cbuf, pheader );
#else
  w = mas_fwrite_message( mas_channel_stream( pchannel ), cbuf, pheader );
#endif

  return w;
}
