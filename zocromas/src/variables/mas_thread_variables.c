#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <mastar/channel/mas_channel.h>

#include "mas_common.h"

#include "zoctools/inc/mas_thread_tools.h"

#include "mas_variables.h"
#include "mas_thread_variables.h"


/*
this:
  mas_thread_variables.c
related:
  mas_thread_variables.h
  mas_variables.c
  mas_thread_tools.c
more:
  mas_init_threads.c
  mas_init_threads.h
  mas_thread_variables_types.h
  mas_lib_thread.h
  mas_thread_tools.h
  mas_lib_thread.c


*/


int
mas_thread_variables_delete( void )
{
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables )
  {
    mas_variables_delete( thd->variables );
    thd->variables = NULL;
  }
  return 0;
}

int
mas_thread_variables_msg( const char *vclass )
{
  mas_thdata_t *thd;
  mas_variable_t *v;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables )
  {
    MAS_LIST_FOREACH( v, thd->variables, next )
    {
      if ( !vclass || 0 == strcmp( vclass, v->vclass ) )
        rMSG( ">>>> %s: %s", v->name, v->value );
    }
  }
  return 0;
}

/* int                                                                                                                             */
/* _mas_channel_write_variables( const char *vclass, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel )            */
/* {                                                                                                                               */
/*   int r;                                                                                                                        */
/*   int wfd;                                                                                                                      */
/*                                                                                                                                 */
/*   wfd = mas_channel_fd_back( pchannel );                                                                                        */
/*   r = _mas_write_variables( vclass, fname, fvalue, fmt, wfd );                                                                  */
/*                                                                                                                                 */
/*   return r;                                                                                                                     */
/* }                                                                                                                               */

/* int                                                                                                                    */
/* _mas_channel_write_variables_1( const char *vclass, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel ) */
/* {                                                                                                                      */
/*   int w = 0;                                                                                                           */
/*                                                                                                                        */
/*   mas_thdata_t *thd;                                                                                                   */
/*   mas_variable_t *v;                                                                                                   */
/*                                                                                                                        */
/*   ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );                                             */
/*   if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables )                                        */
/*   {                                                                                                                    */
/*     MAS_LIST_FOREACH( v, thd->variables, next )                                                                        */
/*     {                                                                                                                  */
/*       if ( !vclass || 0 == strcmp( vclass, v->vclass ) )                                                               */
/*       {                                                                                                                */
/*         int ww = 0;                                                                                                    */
/*                                                                                                                        */
/*         if ( fname && fvalue )                                                                                         */
/*           ww = mas_channel_writef( pchannel, fmt, v->name, v->value );                                                 */
/*         else if ( fvalue )                                                                                             */
/*           ww = mas_channel_writef( pchannel, fmt, v->value );                                                          */
/*         else if ( fname )                                                                                              */
/*           ww = mas_channel_writef( pchannel, fmt, v->name );                                                           */
/*         if ( ww > 0 )                                                                                                  */
/*           w += ww;                                                                                                     */
/*       }                                                                                                                */
/*     }                                                                                                                  */
/*   }                                                                                                                    */
/*   return w;                                                                                                            */
/* }                                                                                                                      */

int
_mas_thread_variables_channel_write( const char *vclass, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel )
{
  int w = 0;

  mas_thdata_t *thd;

  /* mas_variable_t *v; */
  /* int cnt = 0; */

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables )
  {
    w = _mas_variables_channel_writef( thd->variables, vclass, fname, fvalue, fmt, pchannel );
    /* MAS_LIST_FOREACH( v, thd->variables, next )                        */
    /* {                                                                  */
    /*   if ( !vclass || 0 == strcmp( vclass, v->vclass ) )               */
    /*   {                                                                */
    /*     int ww = 0;                                                    */
    /*                                                                    */
    /*     if ( fname && fvalue )                                         */
    /*       ww = mas_channel_writef( pchannel, fmt, v->name, v->value ); */
    /*     else if ( fvalue )                                             */
    /*       ww = mas_channel_writef( pchannel, fmt, v->value );          */
    /*     else if ( fname )                                              */
    /*       ww = mas_channel_writef( pchannel, fmt, v->name );           */
    /*     if ( ww > 0 )                                                  */
    /*       w += ww;                                                     */
    /*     else                                                           */
    /*     {                                                              */
    /*       EMSG( "BAD (cnt:%d) w:%d", cnt, w );                         */
    /*                                                                    */
    /*       w = ww;                                                      */
    /*       break;                                                       */
    /*     }                                                              */
    /*   }                                                                */
    /*   cnt++;                                                           */
    /* }                                                                  */
  }
  return w;
}


int
mas_thread_variables_channel_write( const char *vclass, const char *fmt, mas_channel_t * pchannel )
{
  return _mas_thread_variables_channel_write( vclass, 1, 1, fmt, pchannel );
}

int
mas_thread_vvariable_create_x( th_type_t thtype, const char *vclass, const char *name, mas_xvsnprintf_t func, const char *fmt,
                               va_list args )
{
  int r = 0;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    thd->variables = mas_variable_vcreate_x( thd->variables, thtype, vclass, name, func, fmt, args, 0 );
  }
  return r;
}

int
mas_thread_variable_create_x( th_type_t thtype, const char *vclass, const char *name, mas_xvsnprintf_t func, const char *fmt,
                              ... )
{
  int r;
  va_list args;

  va_start( args, fmt );
  r = mas_thread_vvariable_create_x( thtype, vclass, name, func, fmt, args );
  va_end( args );
  return r;
}

int
mas_thread_variable_create_text( th_type_t thtype, const char *vclass, const char *name, const char *txt )
{
  int r = 0;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    thd->variables = mas_variable_create_text( thd->variables, thtype, vclass, name, txt, 0 );
  }
  return r;
}

mas_variable_t *
mas_thread_variables_find( const char *vclass, const char *name )
{
  mas_variable_t *found = NULL;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    /* lock */
    if ( thd->variables )
      found = mas_variables_find( thd->variables, vclass, name );
  }
  return found;
}

int
mas_thread_variable_set_text( th_type_t thtype, const char *vclass, const char *name, const char *txt )
{
  int r = 0;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    /* lock */
    thd->variables = mas_variable_set_text( thd->variables, thtype, vclass, name, txt );
  }
  return r;
}
