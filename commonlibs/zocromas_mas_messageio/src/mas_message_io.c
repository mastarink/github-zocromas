#define MSG_VERSION 0x20130228
#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/io/mas_io.h>

#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel_open.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */
extern mas_control_t ctrl;

/* extern mas_options_t opts; */

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
  int r = 0;

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
        IEVAL( r, -1 );
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
        /* ioMSG( "w. to memcpy message to %p; len:%u", ( void * ) msg->message, ( unsigned ) strlen( msg->message ) ); */
        WMSG( "MESSAGE sLEN:%u", ( unsigned ) strlen( cbuf ) );
        memcpy( msg->message, cbuf, len );
        ioMSG( "w.  memcpy'd message" );
      }
      ioMSG( "w. to fill header" );
      msg->h.sign = MSG_SIGNATURE;
      msg->h.vers = MSG_VERSION;
      msg->h.restart_cnt = ctrl.restart_cnt;
      msg->h.len = len;
      msg->h.sender_stamp = ctrl.stamp;
      msg->h.pid = getpid(  );
      msg->h.pth = pthread_self(  );
      ioMSG( "to write h/msg %d", msgsz );
      /* HMSG( "to write h/msg %d", msgsz ); */
#ifndef MAS_CHANNEL_STREAM_WRITE
      IEVAL( r, write( fd, msg, msgsz ) );
      /* rf=0; */
#else
      IEVAL( r, mas_fwrite( msg, 1, msgsz, stream ) );
      /* rf = fflush( stream ); */
#endif
      /* HMSG( "fwritten h/msg %d", r ); */
      WMSG( "(%d)WRITE r:%d of %d v.%x [h:%u] {u:%u ul:%u ull:%u vp:%u c:%u pid:%u pth:%u stamp:%u bin:%u i:%u}",
            ctrl.error_handler ? 1 : 0, r, ( int ) msgsz, msg->h.vers, ( unsigned ) sizeof( mas_header_t ), ( unsigned ) sizeof( unsigned ),
            ( unsigned ) sizeof( unsigned long ), ( unsigned ) sizeof( unsigned long long ), ( unsigned ) sizeof( void * ),
            ( unsigned ) sizeof( char ), ( unsigned ) sizeof( pid_t ), ( unsigned ) sizeof( pthread_t ), ( unsigned ) sizeof( mas_stamp_t ),
            ( unsigned ) sizeof( bin_type_t ), ( unsigned ) sizeof( int ) );
      if ( r >= 0 && r != msgsz )
      {
        EMSG( "written %d of %d", r, msgsz );
        P_ERR;
        IEVAL( r, -r );
      }
      ioMSG( "written(sent); new opts:%d; size: %d", msg->h.new_opts, msgsz );
    }
    mas_free( msg );
  }
  else
  {
    IEVAL( r, -1 );
  }
  /* if ( r < 0 )               */
  /* {                          */
  /*   EMSG( "written %d", r ); */
  /* }                          */
  return r;
}

int
mas_channel_write_message( mas_channel_t * pchannel, const char *cbuf, mas_header_t * pheader )
{
  int w = 0;

  /* HMSG( "to write message" ); */
#ifndef MAS_CHANNEL_STREAM_WRITE
  IEVAL( w, mas_write_message( mas_channel_fd( pchannel ), cbuf, pheader ) );
#else
  IEVAL( w, mas_fwrite_message( mas_channel_stream( pchannel ), cbuf, pheader ) );
#endif

  return w;
}

int
mas_channel_read_message( mas_channel_t * pchannel, char **pbuf, mas_header_t * pheader )
{
  int r = 0, rmsg = 0;
  int msghsz = 0;
  int msgsz = 0;
  mas_message_t *msg = NULL;

  rmsg = 0;
  if ( pbuf )
    *pbuf = NULL;

  mas_channel_read_remainder( pchannel );
  r = pchannel->buffer.length;
  if ( r > 0 )
    rmsg += r;
  msg = ( mas_message_t * ) pchannel->buffer.buffer;
  if ( r > 0 && rmsg >= 0 )
  {
    if ( r > msghsz && msg->h.len > 0 )
    {
      if ( msg->h.vers != MSG_VERSION )
      {
        EMSG( "version not match: %x ? %x : %s", msg->h.vers, MSG_VERSION, ( char * ) &msg->h );
        EMSG( "(sign)%lx : %lx", ( unsigned long ) msg->h.sign, ( unsigned long ) MSG_SIGNATURE );
        IEVAL( r, -1 );
      }
      else
      {
        ioMSG( "version OK" );
      }
      if ( msg->h.sign != MSG_SIGNATURE )
      {
        EMSG( "wrong signature: %lx", ( unsigned long ) msg->h.sign );
        /* sleep (3); */
        IEVAL( r, -1 );
      }
      else
      {
        HMSG( "signature OK" );
        HMSG( "(vers)%x : %x", msg->h.vers, MSG_VERSION );
        HMSG( "(sign)%lx : %lx", ( unsigned long ) msg->h.sign, ( unsigned long ) MSG_SIGNATURE );

        if ( pheader )
        {
          mas_header_t *msgh = ( mas_header_t * ) msg;

          *pheader = *msgh;
          /*
           *pheader->sender_pid = msgh->h.sender_pid;
           *pheader->sender_vdate = msgh->h.sender_vdate;
           *pheader->sender_vtime = msgh->h.sender_vtime;
           */
        }
        if ( r > 0 )
        {
          if ( pbuf )
          {
            HMSG( "got; new opts:%d; size:%d / %d", msg->h.new_opts, msgsz, msg->h.len );
            *pbuf = mas_malloc( msg->h.len );
            HMSG( "buf: a.sz:%d", msg->h.len );
            memcpy( *pbuf, msg->message, msg->h.len );
          }
          else
          {
            EMSG( "mas_read_message error can't pass msg" );
          }
        }
      }
    }
    else
    {
      EMSG( "header size (got:%d : expected:%d)", r, msghsz );
      IEVAL( rmsg, -1 );
    }
  }
  mas_channel_delete_buffer( pchannel );


  return rmsg;
}
