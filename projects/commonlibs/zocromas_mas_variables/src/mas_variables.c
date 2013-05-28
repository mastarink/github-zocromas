#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>
#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/io/mas_io.h>

#include <mastar/channel/mas_channel.h>

#include "mas_variables.h"

/*
this:
  mas_variables.c
related:
  mas_variables.h
  mas_thread_variables.c
  mas_thread_tools.c

  mas_channel.c

*/



static mas_variables_list_head_t *
mas_variables_setup( mas_variables_list_head_t * variables )
{
  if ( !variables )
  {
    variables = mas_malloc( sizeof( mas_variables_list_head_t /* variables */  ) );
    if ( variables )
    {
      memset( variables, 0, sizeof( mas_variables_list_head_t /* variables */  ) );
      MAS_LIST_INIT( variables );
    }
  }
  return variables;
}

static mas_variables_list_head_t *
mas_variable_attach( mas_variables_list_head_t * variables, mas_variable_t * var )
{
  if ( var )
  {
    MAS_LIST_ADD( variables, var, next );
  }
  return variables;
}

static mas_variable_t *
mas_variable_create( /* th_type_t thtype, */const char *vclass, const char *name, mas_vartype_t vtype,
                     const char *value, size_t valsize, int nomem )
{
  mas_variable_t *var = NULL;

  if ( name )
  {
    var = mas_malloc( sizeof( mas_variable_t ) ); /* Insert at the head. */
    if ( var )
    {
      memset( var, 0, sizeof( mas_variable_t ) );
      if ( nomem )
        var->name = ( char * ) name;
      else
        var->name = mas_strdup( name );
      if ( value )
      {
        if ( nomem )
        {
          var->value = ( char * ) value;
        }
        else
        {
          var->value = mas_malloc( valsize );
          memcpy( var->value, value, valsize );
        }
        var->valsize = valsize;
      }
      if ( vclass )
      {
        if ( nomem )
          var->vclass = ( char * ) vclass;
        else
          var->vclass = mas_strdup( vclass );
      }
      /* var->thtype = thtype; */
      /* switch ( thtype )            */
      /* {                            */
      /* case MAS_THREAD_NONE:        */
      /*   break;                     */
      /* case MAS_THREAD_MAIN:        */
      /*   break;                     */
      /* case MAS_THREAD_MASTER:      */
      /*   break;                     */
      /* case MAS_THREAD_LISTENER:    */
      /*   break;                     */
      /* case MAS_THREAD_TRANSACTION: */
      /*   break;                     */
      /* case MAS_THREAD_TICKER:      */
      /*   break;                     */
      /* case MAS_THREAD_WATCHER:     */
      /*   break;                     */
      /* case MAS_THREAD_LOGGER:      */
      /*   break;                     */
      /* }                            */
    }
  }
  return var;
}

mas_variables_list_head_t *
mas_variable_create_typed( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name,
                           mas_vartype_t vtype, const void *data, size_t datasize, int nomem )
{
  if ( ( variables = mas_variables_setup( variables ) ) )
  {
    mas_variable_t *var = NULL;
/* TODO : find/create class, attach variable as 2 elements of struct iovec */
    var = mas_variable_create( /* thtype, */vclass, name, vtype, data, datasize, nomem );
    variables = mas_variable_attach( variables, var );
  }
  return variables;
}

mas_variables_list_head_t *
mas_variable_create_binary( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name,
                            const void *data, size_t datasize )
{
  variables = mas_variable_create_typed( variables, /* thtype, */ vclass, name, MAS_VARTYPE_BINARY, data, datasize, 0 );
  return variables;
}

mas_variables_list_head_t *
mas_variable_create_text( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name,
                          const void *txt, int nomem )
{
  variables = mas_variable_create_typed( variables, /* thtype, */  vclass, name, MAS_VARTYPE_TEXT, txt, strlen( txt ) + 1, nomem );
  return variables;
}

mas_variables_list_head_t *
mas_variable_vcreate_x( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name,
                        mas_xvsnprintf_t func, const char *fmt, va_list args, int nomem )
{
  if ( ( variables = mas_variables_setup( variables ) ) )
  {
    char *text = NULL;
    size_t txsize = 1024 * 10;

    text = mas_malloc( txsize );
    if ( !func )
      func = mas_xvsnprintf;
    ( *func ) ( text, txsize, fmt, args );
    variables = mas_variable_create_text( variables, /* thtype, */  vclass, name, text, nomem );
#ifdef MAS_LOG
    MAS_LOG( "V %s.%s=[%s]", vclass, name, text );
#endif
    mas_free( text );
  }

  return variables;
}

mas_variables_list_head_t *
mas_variable_create_x( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name,
                       mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  variables = mas_variable_vcreate_x( variables, /* thtype, */  vclass, name, func, fmt, args, 0 );
  va_end( args );
  return variables;
}

int
mas_variables_delete( mas_variables_list_head_t * variables )
{
  if ( variables )
  {
    mas_variable_t *var;

    while ( ( var = MAS_LIST_FIRST( variables ) ) )
    {
      MAS_LIST_REMOVE( variables, var, mas_variable_s, next );
      mas_free( var->value );
      mas_free( var->name );
      mas_free( var->vclass );
      mas_free( var );
    }
    mas_free( variables );
  }
  return 0;
}

mas_variable_t *
mas_variables_find( mas_variables_list_head_t * variables, const char *vclass, const char *name )
{
  mas_variable_t *var_found = NULL;

  if ( variables )
  {
    mas_variable_t *var;

    MAS_LIST_FOREACH( var, variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, var->vclass ) )
      {
        if ( name && 0 == strcmp( name, var->name ) )
        {
          var_found = var;
          break;
        }
      }
    }
  }
  return var_found;
}

mas_variable_t *
mas_variable_matching( mas_variables_list_head_t * variables, mas_variable_t * var, const char *vclass, const char *name )
{
  if ( var )
    var = mas_variable_next( var, vclass, name );
  else
    var = mas_variable_first( variables, vclass, name );
  return var;
}

mas_variable_t *
mas_variable_next( mas_variable_t * var, const char *vclass, const char *name )
{
  while ( var )
  {
    var = MAS_LIST_NEXT( var, next );
    if ( var && ( !vclass || 0 == strcmp( vclass, var->vclass ) ) && ( !name || 0 == strcmp( name, var->name ) ) )
      break;
  }
  return var;
}

mas_variable_t *
mas_variable_first( mas_variables_list_head_t * variables, const char *vclass, const char *name )
{
  mas_variable_t *var = NULL;

  if ( variables )
  {
    MAS_LIST_FOREACH( var, variables, next )
    {
      if ( var && ( !vclass || 0 == strcmp( vclass, var->vclass ) ) && ( !name || 0 == strcmp( name, var->name ) ) )
        break;
    }
  }
  return var;
}


mas_variables_list_head_t *
mas_variable_set_text( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass, const char *name, const void *txt )
{
  if ( ( variables = mas_variables_setup( variables ) ) )
  {
    mas_variable_t *found = NULL;
    mas_variable_t *var = NULL;

    found = mas_variables_find( variables, vclass, name );
    if ( found )
    {
      if ( found->value )
        mas_free( found->value );
      found->value = mas_strdup( txt );
    }
    else
    {
      var = mas_variable_create( /* thtype, */ vclass, name, MAS_VARTYPE_TEXT, txt, strlen( txt ) + 1, 0 );
      variables = mas_variable_attach( variables, var );
    }
  }
  return variables;
}

int
_mas_variables_writef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt, int wfd )
{
  int w = 0;
  mas_variable_t *var;

  if ( variables )
  {
    MAS_LIST_FOREACH( var, variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, var->vclass ) )
      {
        int ww = 0;

        if ( fname && fvalue )
          ww = mas_writef( wfd, fmt, var->name, var->value );
        else if ( fvalue )
          ww = mas_writef( wfd, fmt, var->value );
        else if ( fname )
          ww = mas_writef( wfd, fmt, var->name );
        if ( ww > 0 )
          w += ww;
      }
    }
  }
  return w;
}

int
mas_variables_writef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt, int wfd )
{
  return _mas_variables_writef( variables, vclass, 1, 1, fmt, wfd );
}

int
_mas_variables_fwritef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt, FILE * f )
{
  int w = 0;
  mas_variable_t *var;

  if ( variables )
  {
    MAS_LIST_FOREACH( var, variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, var->vclass ) )
      {
        int ww = 0;

        if ( fname && fvalue )
          ww = mas_fprintf( f, fmt, var->name, var->value );
        else if ( fvalue )
          ww = mas_fprintf( f, fmt, var->value );
        else if ( fname )
          ww = mas_fprintf( f, fmt, var->name );
        if ( ww > 0 )
          w += ww;
      }
    }
  }
  return w;
}

int
mas_variables_fwritef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt, FILE * f )
{
  return _mas_variables_fwritef( variables, vclass, 1, 1, fmt, f );
}

int
_mas_variables_channel_writef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt,
                               const mas_channel_t * pchannel )
{
  int w = 0;

  mas_variable_t *v;
  int cnt = 0;

  /* rMSG( ">>>>>>> %p : %s <<<<<<<<<", ( void * ) variables, vclass ); */
  if ( variables )
  {
    MAS_LIST_FOREACH( v, variables, next )
    {
      /* rMSG( ">>>>>>> %s ? %s <<<<<<<<<", vclass, v->vclass ); */
      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
      {
        int ww = 0;

        /* rMSG( ">>>>>>>(%s) '%s' : '%s'", fmt, v->name, v->value ); */
        if ( fname && fvalue )
          ww = mas_channel_writef( pchannel, fmt, v->name, v->value );
        else if ( fvalue )
          ww = mas_channel_writef( pchannel, fmt, v->value );
        else if ( fname )
          ww = mas_channel_writef( pchannel, fmt, v->name );
        if ( ww > 0 )
          w += ww;
        else
        {
#ifdef EMSG
          EMSG( "BAD (cnt:%d) w:%d", cnt, w );
#endif
          w = ww;
          break;
        }
      }
      cnt++;
    }
  }
  return w;
}


int
mas_variables_chwritef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt, const mas_channel_t * pchannel )
{
  return _mas_variables_channel_writef( variables, vclass, 1, 1, fmt, pchannel );
}

int
mas_variables_memory_write_pairs( mas_variables_list_head_t * variables, const char *vclass, char *buffer, size_t max )
{
  char *pbuffer = NULL;
  mas_variable_t *v;

  if ( variables && buffer )
  {
    ssize_t smax;

    smax = max - 10;
    pbuffer = buffer;
    MAS_LIST_FOREACH( v, variables, next )
    {
      size_t len;

      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
      {
        strncpy( pbuffer, v->name, smax );
        pbuffer += ( len = strlen( pbuffer ) );
        smax -= len;
        if ( smax < 50 )
          break;

        strncpy( pbuffer, ": ", smax );
        pbuffer += ( len = 2 );
        smax -= len;
        if ( smax < 50 )
          break;

        if ( v->value )
        {
          strncpy( pbuffer, v->value, smax );
          pbuffer += ( len = strlen( pbuffer ) );
          smax -= len;
          if ( smax < 50 )
            break;
        }

        strncpy( pbuffer, "\r\n", smax );
        pbuffer += ( len = 2 );
        smax -= len;
        if ( smax < 50 )
          break;
      }
    }
  }
  return pbuffer - buffer;
}

int
mas_variables_chwrite_pairs( mas_variables_list_head_t * variables, const char *vclass, const mas_channel_t * pchannel )
{
  int w = 0;

  mas_variable_t *v;
  int cnt = 0;

  if ( variables )
  {
    MAS_LIST_FOREACH( v, variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
      {
        int ww = 0;

        ww = mas_channel_write_string( pchannel, v->name, 0 );
        if ( ww > 0 )
        {
          w += ww;
          ww = mas_channel_write_string( pchannel, ": ", 0 );
        }
        if ( ww > 0 )
        {
          w += ww;
          ww = mas_channel_write_string( pchannel, v->value, 0 );
        }
        if ( ww > 0 )
        {
          w += ww;
          ww = mas_channel_write_string( pchannel, "\r\n", 0 );
        }
        if ( ww < 0 )
        {
#ifdef EMSG
          EMSG( "BAD (cnt:%d) w:%d", cnt, w );
#endif
          w = ww;
          break;
        }
      }
      cnt++;
    }
  }
  return w;
}

int
mas_variables_log_pairs( mas_variables_list_head_t * variables, const char *vclass )
{
  int w = 0;

#ifdef MAS_LOG
  mas_variable_t *v;
  int cnt = 0;

  if ( variables )
  {
    MAS_LIST_FOREACH( v, variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
      {
        MAS_LOG( "HDR %s: %s", v->name, v->value );
      }
      cnt++;
    }
  }
#endif
  return w;
}

int
mas_variables_chwrite_bin( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue,
                           const mas_channel_t * pchannel )
{
  int w = 0;

  mas_variable_t *v;
  int cnt = 0;

  /* rMSG( ">>>>>>> %p : %s <<<<<<<<<", ( void * ) variables, vclass ); */
  if ( variables )
  {
    MAS_LIST_FOREACH( v, variables, next )
    {
      /* rMSG( ">>>>>>> %s ? %s <<<<<<<<<", vclass, v->vclass ); */
#ifdef MAS_LOG
      MAS_LOG( "COMPARE %s : %s", vclass, v->vclass );
#endif
      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
      {
        int ww = 0;

        /* rMSG( ">>>>>>>(%s) '%s' : '%s'", fmt, v->name, v->value ); */
        if ( fname )
        {
          ww = mas_channel_write( pchannel, v->name, strlen( v->name ) );
          if ( ww > 0 )
            w += ww;
        }
        if ( ww >= 0 && fvalue )
        {
#ifdef MAS_LOG
          MAS_LOG( "to write valsize:%lu", v->valsize );
#endif
          ww = mas_channel_write( pchannel, v->value, v->valsize );
#ifdef MAS_LOG
          MAS_LOG( "writtten valsize:%lu", v->valsize );
#endif
          if ( ww > 0 )
            w += ww;
        }
        if ( ww <= 0 )
        {
#ifdef EMSG
          EMSG( "BAD (cnt:%d) w:%d", cnt, w );
#endif
          w = ww;
          break;
        }
      }
      cnt++;
    }
  }
  return w;
}
