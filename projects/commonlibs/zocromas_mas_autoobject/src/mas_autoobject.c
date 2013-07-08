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
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>



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
  int rdr = 0;

  aao = ( mas_autoobject_t * ) a;
  bao = ( mas_autoobject_t * ) b;
  if ( a == b )
    return 0;
  if ( !aao->name )
    return bao->name ? -1 : 0;
  if ( !bao->name )
    return aao->name ? 1 : 0;
  if ( aao->docroot )
    rdr = strcmp( aao->docroot, bao->docroot );
  return rdr ? rdr : strcmp( aao->name, bao->name );
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
    case MAS_IACCESS_SENDFILEL:
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
    case MAS_IACCESS_SENDFILEL:
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
    if ( obj->etag )
      mas_free( obj->etag );
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FILE:
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_CHAR:
      mas_free( obj->data );
      break;
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_SENDFILEL:
    case MAS_IACCESS_BAD:
      break;
    }
    obj->data = NULL;
  }
  return 0;
}

int
mas_autoobject_qopen( mas_autoobject_t * obj, const char *docroot, const char *name )
{
  _mas_autoobject_set_name( obj, docroot, name );
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

    /* HMSG( "TO OPEN (%d) %s :: %s", obj->iaccess_type, obj->docroot, obj->name ); */
    memset( &st, 0, sizeof( st ) );
    /* hbefore = obj->handler.i; */
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SPLICE:
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_SENDFILEL:
      obj->reopen_cnt++;
      {
        char *fname = NULL;

        if ( obj->docroot )
          fname = mas_strdup( obj->docroot );
        fname = mas_strcat_x( fname, obj->name );
        obj->handler.i = mas_open( fname, O_RDONLY );
        /* HMSG( "OPEN (%d) %s", obj->handler.i, fname ); */
        mas_free( fname );
      }
      obj->stat_cnt++;
      rst = fstat( mas_autoobject_fd( obj ), &st );
      obj->inode = st.st_ino;
      obj->time = st.st_mtime;

      break;
    case MAS_IACCESS_FILE:
      obj->reopen_cnt++;
      {
        char *fname = NULL;

        if ( obj->docroot )
          fname = mas_strdup( obj->docroot );
        fname = mas_strcat_x( fname, obj->name );
        obj->handler.f = fopen( fname, "r" );
        /* HMSG( "OPEN %s", fname ); */
        mas_free( fname );
      }
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
mas_autoobject_set_name( mas_autoobject_t * obj, const char *docroot, const char *name )
{
  if ( obj && name && ( !obj->name || 0 != strcmp( name, obj->name ) ) )
  {
    int o;

    o = mas_autoobject_opened( obj );
    mas_autoobject_close( obj );
    _mas_autoobject_set_name( obj, docroot, name );
    if ( o )
      mas_autoobject_reopen( obj );
  }
}

int
mas_autoobject_open( mas_autoobject_t * obj, const char *docroot, const char *name )
{
  if ( obj )
    mas_autoobject_close( obj );
  return mas_autoobject_qopen( obj, docroot, name );
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
    case MAS_IACCESS_SENDFILEL:
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
    case MAS_IACCESS_SENDFILEL:
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
mas_autoobject_set_data( mas_autoobject_t * obj, const void *ptr )
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
      case MAS_IACCESS_SENDFILEL:
        break;
      case MAS_IACCESS_BAD:
        break;
      }
  }
  return r;
}

size_t
mas_autoobject_size( mas_autoobject_t * obj )
{
  if ( obj )
    mas_autoobject_reopen( obj );
  return obj ? obj->size : 0;
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
    case MAS_IACCESS_SENDFILEL:
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
    case MAS_IACCESS_SENDFILEL:
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
    case MAS_IACCESS_SENDFILE:
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
          off_t start = 0;

          fd = mas_autoobject_fd( obj );
          obj->sendfile_cnt++;
          obj->pass = 1;
          r = sendfile( han, fd, &start, obj->size );
        }
        break;
      case MAS_IACCESS_SENDFILEL:
        if ( han && obj->handler.v && obj->size )
        {
          int fd;

          fd = mas_autoobject_fd( obj );
          obj->sendfile_cnt++;
          obj->pass = 1;
          r = sendfile( han, fd, NULL, obj->size );
        }
        break;
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

content_type_ext_t content_exts[] = {
  {MAS_CONTENT_HTML, ":html:htm:"}
  ,
  {MAS_CONTENT_GIF, ":gif:GIF:"}
  ,
  {MAS_CONTENT_JPEG, ":jpg:JPG:jpeg:JPEG"}
  ,
  {MAS_CONTENT_TEXT, ":txt:"}
};

content_type_details_t content_types[] = {
  {MAS_CONTENT_HTML, "text", "html"}
  ,
  {MAS_CONTENT_TEXT, "text", "plain"}
  ,
  {MAS_CONTENT_GIF, "image", "gif"}
  ,
  {MAS_CONTENT_JPEG, "image", "jpeg"}
  ,
  {MAS_CONTENT_FORM_DATA, "multipart", "form-data"}
};

static mas_content_type_t
mas_content_type_by_ext( mas_autoobject_t * obj )
{
  mas_content_type_t t = MAS_CONTENT_BAD;
  const char *epath;
  const char *ext = NULL;
  char extbuf[256];

  char *fname = NULL;

  if ( obj->docroot )
    fname = mas_strdup( obj->docroot );
  fname = mas_strcat_x( fname, obj->name );
  obj->handler.i = mas_open( fname, O_RDONLY );


  epath = fname + strlen( fname );
  while ( epath > fname )
  {
    epath--;
    if ( *epath == '.' )
      ext = epath + 1;
    else if ( *epath == '/' )
      break;
  }
  if ( ext )
  {
    int len;

    strcpy( extbuf + 1, ext );
    extbuf[0] = ':';
    len = strlen( ext );
    extbuf[0] = ':';
    extbuf[len + 1] = ':';
    extbuf[len + 2] = '\0';
  }

  /* rMSG( "CHECK EXT %s", extbuf ); */
  for ( int it = 0; it < sizeof( content_exts ) / sizeof( content_exts[0] ); it++ )
  {
    /* MAS_LOG( "CHECK EXT '%s' ? '%s'", content_exts[it].extset, extbuf ); */
    if ( strstr( content_exts[it].extset, extbuf ) )
    {
      t = content_exts[it].ctype;
      /* MAS_LOG( "CHECK EXT = %d", t ); */
      break;
    }
  }
  if ( fname )
    mas_free( fname );
  return t;
}

mas_content_type_t
mas_autoobject_content_type_by_ext( mas_autoobject_t * obj )
{
  mas_content_type_t t = MAS_CONTENT_BAD;

  if ( obj )
    t = mas_content_type_by_ext( obj );
  return t;
}

mas_content_type_t
mas_autoobject_icontent_type( mas_autoobject_t * obj )
{
  return obj ? obj->icontent_type : MAS_CONTENT_BAD;
}

void
mas_autoobject_set_icontent_type( mas_autoobject_t * obj, mas_content_type_t ict )
{
  if ( obj )
    obj->icontent_type = ict;
}

char *
mas_autoobject_content_type_string( mas_autoobject_t * obj )
{
  char *s = NULL;

  if ( obj )
  {
    for ( int it = 0; it < sizeof( content_types ) / sizeof( content_types[0] ); it++ )
    {
      /* MAS_LOG( "CT: %d", mas_udata_icontent_type( ud ) ); */
      if ( mas_autoobject_icontent_type( obj ) == content_types[it].ctype )
      {
        s = mas_strdup( content_types[it].mtype );
        s = mas_strcat_x( s, "/" );
        s = mas_strcat_x( s, content_types[it].dtype );
      }
    }
  }
  return s;
}

time_t
mas_autoobject_time( mas_autoobject_t * obj )
{
  if ( obj )
    mas_autoobject_reopen( obj );
  return obj ? obj->time : 0;
}

const char *
mas_autoobject_gtime( mas_autoobject_t * obj )
{
  if ( obj )
  {
    char stime[64];

    /* Mon, 01 Oct 2012 20:49:57 GMT */
    if ( !obj->gtime )
    {
      time_t t;

      t = mas_autoobject_time( obj );
      strftime( stime, sizeof( stime ), "%a, %d %b %Y %T GMT", gmtime( &t ) );
      obj->gtime = mas_strdup( stime );
    }
  }
  return obj->gtime;
}

const char *
mas_autoobject_etag( mas_autoobject_t * obj )
{
  const char *etag = NULL;

  if ( obj )
  {
    if ( !obj->etag )
    {
      mas_autoobject_reopen( obj );
      if ( obj->inode && obj->time && obj->size )
      {
        char etag_buf[256];

        snprintf( etag_buf, sizeof( etag_buf ), "%lx-%lx-%lx", obj->inode, obj->size, obj->time * 1000000 );

        obj->etag = mas_strdup( "\"" );
        obj->etag = mas_strcat_x( obj->etag, etag_buf );
        obj->etag = mas_strcat_x( obj->etag, "\"" );
      }
    }
    etag = obj->etag;
  }
  return etag;
}
