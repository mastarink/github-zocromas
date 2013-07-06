#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/sendfile.h>



#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>


#include "mas_autoobject_object.h"
#include "mas_autoobject.h"

void
mas_autoobject_destroy( mas_autoobject_t * obj )
{
  if ( obj )
  {
    mas_autoobject_close( obj );
    mas_autoobject_delete_data( obj );
    mas_autoobject_delete( obj );
  }
}

int
_mas_autoobject_compare( const void *a, const void *b )
{
  mas_autoobject_t *aao, *bao;

  aao = ( mas_autoobject_t * ) a;
  bao = ( mas_autoobject_t * ) b;
  if ( a == b )
    return 0;
  if ( !aao->name )
    return bao->name ? -1 : 0;
  if ( !bao->name )
    return aao->name ? 1 : 0;
  return strcmp( aao->name, bao->name );
}

int
mas_autoobject_fd( mas_autoobject_t * obj )
{
  int fd = 0;

  if ( obj )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      fd = obj->handler.i;
      break;
    case MAS_IACCESS_FILE:
      fd = fileno( obj->handler.f );
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return fd;
}

FILE *
mas_autoobject_file( mas_autoobject_t * obj )
{
  FILE *f = NULL;

  if ( obj )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      break;
    case MAS_IACCESS_FILE:
      f = obj->handler.f;
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return f;
}


void
mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t )
{
  if ( obj && obj->iaccess_type != t )
  {
    int o;

    o = mas_autoobject_opened( obj );
    if ( o )
    {
      mas_autoobject_close( obj );
      mas_autoobject_delete_data( obj );
    }
    _mas_autoobject_set_iaccess_type( obj, t );
    if ( o )
    {
      mas_autoobject_reopen( obj );
    }
  }
}

void
_mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t )
{
  obj->iaccess_type = t;
}

int
mas_autoobject_delete_data( mas_autoobject_t * obj )
{
  if ( obj && obj->data )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FILE:
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_CHAR:
      mas_free( obj->data );
      break;
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_BAD:
      break;
    }
    obj->data = NULL;
  }
  return 0;
}

int
mas_autoobject_qopen( mas_autoobject_t * obj, const char *name )
{
  _mas_autoobject_set_name( obj, name );
  return mas_autoobject_reopen( obj );
}

int
mas_autoobject_reopen( mas_autoobject_t * obj )
{
  if ( obj && !obj->handler.v )
  {
    int rst = -1;

    /* int hbefore; */
    struct stat st;

    fprintf( stderr, "TO OPEN %s\n", obj->name );
    memset( &st, 0, sizeof( st ) );
    /* hbefore = obj->handler.i; */
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      obj->reopen_cnt++;
      obj->handler.i = mas_open( ( char * ) obj->name, O_RDONLY );
      obj->stat_cnt++;
      rst = fstat( mas_autoobject_fd( obj ), &st );
      break;
    case MAS_IACCESS_FILE:
      obj->reopen_cnt++;
      obj->handler.f = fopen( ( char * ) obj->name, "r" );
      obj->stat_cnt++;
      rst = fstat( mas_autoobject_fd( obj ), &st );
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
    if ( rst >= 0 )
      obj->size = st.st_size;
  }
  else
  {
  }
  return obj->handler.i ? 0 : -1;
}

void
mas_autoobject_set_name( mas_autoobject_t * obj, const char *name )
{
  if ( obj && name && ( !obj->name || 0 != strcmp( name, obj->name ) ) )
  {
    int o;

    o = mas_autoobject_opened( obj );
    mas_autoobject_close( obj );
    _mas_autoobject_set_name( obj, name );
    if ( o )
      mas_autoobject_reopen( obj );
  }
}

int
mas_autoobject_open( mas_autoobject_t * obj, const char *name )
{
  if ( obj )
    mas_autoobject_close( obj );
  return mas_autoobject_qopen( obj, name );
}

/* int                                                                          */
/* mas_autoobject_qopen_data( mas_autoobject_t * obj )                          */
/* {                                                                            */
/*   return obj->data ? mas_autoobject_qopen( obj, ( char * ) obj->data ) : -1; */
/* }                                                                            */

int
mas_autoobject_opened( mas_autoobject_t * obj )
{
  int opened = 0;

  if ( obj && obj->handler.v )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      opened = ( mas_autoobject_fd( obj ) ) ? 1 : 0;
      break;
    case MAS_IACCESS_FILE:
      opened = ( ( FILE * ) obj->handler.f ) ? 1 : 0;
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return opened;
}

int
_mas_autoobject_close( mas_autoobject_t * obj )
{
  if ( obj && obj->handler.v )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      obj->close_cnt++;
      close( mas_autoobject_fd( obj ) );
      break;
    case MAS_IACCESS_FILE:
      obj->close_cnt++;
      fclose( mas_autoobject_file( obj ) );
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
    obj->handler.v = NULL;
  }
  return 0;
}

int
mas_autoobject_close( mas_autoobject_t * obj )
{
  int r = 0;

  mas_autoobject_set_data( obj, NULL );
  return r;
}

int
mas_autoobject_set_data( mas_autoobject_t * obj, void *ptr )
{
  int r = 0;

  if ( obj )
  {
    _mas_autoobject_close( obj );
    mas_autoobject_delete_data( obj );
    if ( ptr )
      switch ( obj->iaccess_type )
      {
      case MAS_IACCESS_CHAR:
        obj->data = mas_strdup( ( char * ) ptr );
        obj->size = strlen( obj->data );
        break;
      case MAS_IACCESS_FILE:
      case MAS_IACCESS_FCHAR:
        r = mas_autoobject_load_data( obj, 1 );
        break;
      case MAS_IACCESS_SPLICE:
      case MAS_IACCESS_SENDFILE:
        break;
      case MAS_IACCESS_BAD:
        break;
      }
  }
  return r;
}

int
mas_autoobject_load_data( mas_autoobject_t * obj, int use_new )
{
  int r = 0;

  if ( use_new > 1 )
    mas_autoobject_close( obj );
  else if ( use_new > 0 )
    mas_autoobject_delete_data( obj );
  if ( obj && !obj->data )
  {
    mas_autoobject_reopen( obj );
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_CHAR:
      break;
    case MAS_IACCESS_FILE:
      {
        if ( obj->handler.v )
        {
          if ( obj->size > 0 )
          {
            char *buf;

            buf = mas_malloc( obj->size + 2 );
            obj->load_cnt++;
            obj->pass = 1;
            r = fread( buf, 1, obj->size, mas_autoobject_file( obj ) );
            if ( r > 0 )
            {
              obj->data = buf;
              buf[obj->size] = 0;
            }
            else
              mas_free( buf );
          }
        }
      }
      break;
    case MAS_IACCESS_FCHAR:
      {
        if ( obj->handler.v )
        {
          if ( obj->size > 0 )
          {
            char *buf;

            buf = mas_malloc( obj->size + 2 );
            if ( buf )
            {
              mas_autoobject_rewind( obj );
              {
                int fd;

                fd = mas_autoobject_fd( obj );
                obj->load_cnt++;
                obj->pass = 1;
                r = read( fd, buf, obj->size );
              }
              if ( r > 0 )
              {
                obj->data = buf;
                buf[obj->size] = 0;
              }
              else
                mas_free( buf );
            }
            if ( r < 0 )
              perror( "LOAD_DATA" );
            /* fprintf( stderr, "(%d) Nopened:%lu/%lu LOAD_DATA use_new:%d; data:%d (%lu); opened:%d\n", r, obj->reopen_cnt, obj->close_cnt, */
            /*          use_new, obj->data ? 1 : 0, obj->size, mas_autoobject_opened( obj ) );                                               */
          }
        }
      }
      break;
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return 0;
}

int
mas_autoobject_rewind( mas_autoobject_t * obj )
{
  int r = -1;

  if ( obj )
  {
    r = 0;
    mas_autoobject_reopen( obj );
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_CHAR:
      break;
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
      if ( obj->handler.v && obj->size )
      {
        int fd;

        fd = mas_autoobject_fd( obj );
        if ( obj->pass )
        {
          obj->lseek_cnt++;
          lseek( fd, SEEK_SET, 0 );
          obj->pass = 0;
        }
      }
      break;
    case MAS_IACCESS_FILE:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return r;
}

int
mas_autoobject_cat( int han, mas_autoobject_t * obj, int use_new )
{
  int r = -1;

  if ( obj )
  {
    r = mas_autoobject_load_data( obj, use_new );
    if ( r >= 0 )
      r = mas_autoobject_rewind( obj );
    if ( r >= 0 )
    {
      switch ( obj->iaccess_type )
      {
      case MAS_IACCESS_CHAR:
      case MAS_IACCESS_FILE:
      case MAS_IACCESS_FCHAR:
        r = write( han, obj->data, obj->size );
        break;
      case MAS_IACCESS_SPLICE:
        if ( han && obj->handler.v && obj->size )
        {
          int fd;

          fd = mas_autoobject_fd( obj );
          obj->splice_cnt++;
          obj->pass = 1;
          r = splice( fd, NULL, han, NULL, obj->size, SPLICE_F_MOVE | SPLICE_F_MORE | SPLICE_F_NONBLOCK );
          /* r = splice( fd, NULL, han, NULL, obj->size, SPLICE_F_MOVE |  SPLICE_F_NONBLOCK ); */
        }
      case MAS_IACCESS_SENDFILE:
        if ( han && obj->handler.v && obj->size )
        {
          int fd;

          fd = mas_autoobject_fd( obj );
          obj->sendfile_cnt++;
          obj->pass = 1;
          r = sendfile( han, fd, 0, obj->size );
        }
      case MAS_IACCESS_BAD:
        break;
      }
    }
    else
    {
      fprintf( stderr, "ERROR load_data\n" );
    }
  }
  return r;
}
