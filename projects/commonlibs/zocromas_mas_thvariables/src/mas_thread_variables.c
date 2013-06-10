#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
#include <mastar/channel/mas_channel.h>


#include <mastar/thtools/mas_thread_tools.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
#  include <mastar/varset/mas_varset_object.h>
#  include <mastar/varset/mas_varset_search.h>
#  include <mastar/varset/mas_varset.h>
#endif


#include "mas_thread_variables.h"

extern mas_control_t ctrl;


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
#ifdef MAS_OLD_VARIABLES_HTTP
    mas_variables_delete( thd->variables );
#else
    mas_varset_delete( thd->variables );
#endif
    thd->variables = NULL;
  }
  return 0;
}

/* int                                                                             */
/* mas_thread_variables_msg( const char *vclass_name )                                  */
/* {                                                                               */
/*   mas_thdata_t *thd;                                                            */
/*   mas_variable_t *v;                                                            */
/*                                                                                 */
/*   ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );      */
/*   if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables ) */
/*   {                                                                             */
/*     MAS_LIST_FOREACH( v, thd->variables, next )                                 */
/*     {                                                                           */
/*       if ( !vclass_name || 0 == strcmp( vclass_name, v->vclass_name ) )                        */
/*         rMSG( ">>>> %s: %s", v->name, v->value );                               */
/*     }                                                                           */
/*   }                                                                             */
/*   return 0;                                                                     */
/* }                                                                               */

/* int                                                                                                                             */
/* _mas_channel_write_variables( const char *vclass_name, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel )            */
/* {                                                                                                                               */
/*   int r;                                                                                                                        */
/*   int wfd;                                                                                                                      */
/*                                                                                                                                 */
/*   wfd = mas_channel_fd_back( pchannel );                                                                                        */
/*   r = _mas_write_variables( vclass_name, fname, fvalue, fmt, wfd );                                                                  */
/*                                                                                                                                 */
/*   return r;                                                                                                                     */
/* }                                                                                                                               */

/* int                                                                                                                    */
/* _mas_channel_write_variables_1( const char *vclass_name, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel ) */
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
/*       if ( !vclass_name || 0 == strcmp( vclass_name, v->vclass_name ) )                                                               */
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

/* int                                                                                                                              */
/* _mas_thread_variables_channel_write( const char *vclass_name, int fname, int fvalue, const char *fmt, mas_channel_t * pchannel ) */
/* {                                                                                                                                */
/*   int w = 0;                                                                                                                     */
/*                                                                                                                                  */
/*   mas_thdata_t *thd;                                                                                                             */
/*                                                                                                                                  */
/*   ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );                                                       */
/*   if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) && thd->variables )                                                  */
/*   {                                                                                                                              */
/*     w = _mas_variables_channel_writef( thd->variables, vclass_name, fname, fvalue, fmt, pchannel );                              */
/*     (* MAS_LIST_FOREACH( v, thd->variables, next )                        *)                                                     */
/*     (* {                                                                  *)                                                     */
/*     (*   if ( !vclass_name || 0 == strcmp( vclass_name, v->vclass_name ) )               *)                                      */
/*     (*   {                                                                *)                                                     */
/*     (*     int ww = 0;                                                    *)                                                     */
/*     (*                                                                    *)                                                     */
/*     (*     if ( fname && fvalue )                                         *)                                                     */
/*     (*       ww = mas_channel_writef( pchannel, fmt, v->name, v->value ); *)                                                     */
/*     (*     else if ( fvalue )                                             *)                                                     */
/*     (*       ww = mas_channel_writef( pchannel, fmt, v->value );          *)                                                     */
/*     (*     else if ( fname )                                              *)                                                     */
/*     (*       ww = mas_channel_writef( pchannel, fmt, v->name );           *)                                                     */
/*     (*     if ( ww > 0 )                                                  *)                                                     */
/*     (*       w += ww;                                                     *)                                                     */
/*     (*     else                                                           *)                                                     */
/*     (*     {                                                              *)                                                     */
/*     (*       EMSG( "BAD (cnt:%d) w:%d", cnt, w );                         *)                                                     */
/*     (*                                                                    *)                                                     */
/*     (*       w = ww;                                                      *)                                                     */
/*     (*       break;                                                       *)                                                     */
/*     (*     }                                                              *)                                                     */
/*     (*   }                                                                *)                                                     */
/*     (*   cnt++;                                                           *)                                                     */
/*     (* }                                                                  *)                                                     */
/*   }                                                                                                                              */
/*   return w;                                                                                                                      */
/* }                                                                                                                                */


/* int                                                                                                      */
/* mas_thread_variables_channel_write( const char *vclass_name, const char *fmt, mas_channel_t * pchannel ) */
/* {                                                                                                        */
/*   return _mas_thread_variables_channel_write( vclass_name, 1, 1, fmt, pchannel );                        */
/* }                                                                                                        */

/* int                                                                                                                                 */
/* mas_thread_vvariable_create_x( th_type_t thtype, const char *vclass_name, const char *name, mas_xvsnprintf_t func, const char *fmt, */
/*                                va_list args )                                                                                       */
/* {                                                                                                                                   */
/*   int r = 0;                                                                                                                        */
/*   mas_thdata_t *thd;                                                                                                                */
/*                                                                                                                                     */
/*   ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );                                                          */
/*   if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )                                                               */
/*   {                                                                                                                                 */
/*     thd->variables = mas_variable_vcreate_x( thd->variables, (* thtype, *) vclass_name, name, func, fmt, args, 0 );                 */
/*   }                                                                                                                                 */
/*   return r;                                                                                                                         */
/* }                                                                                                                                   */

/* int                                                                                                                                      */
/* mas_thread_variable_create_x( th_type_t thtype, const char *vclass_name, const char *name, mas_xvsnprintf_t func, const char *fmt, ... ) */
/* {                                                                                                                                        */
/*   int r;                                                                                                                                 */
/*   va_list args;                                                                                                                          */
/*                                                                                                                                          */
/*   va_start( args, fmt );                                                                                                                 */
/*   r = mas_thread_vvariable_create_x( thtype, vclass_name, name, func, fmt, args );                                                       */
/*   va_end( args );                                                                                                                        */
/*   return r;                                                                                                                              */
/* }                                                                                                                                        */

/* int                                                                                                             */
/* mas_thread_variable_create_text( th_type_t thtype, const char *vclass_name, const char *name, const char *txt ) */
/* {                                                                                                               */
/*   int r = 0;                                                                                                    */
/*   mas_thdata_t *thd;                                                                                            */
/*                                                                                                                 */
/*   ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );                                      */
/*   if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )                                           */
/*   {                                                                                                             */
/*     thd->variables = mas_variable_create_text( thd->variables, (* thtype, *) vclass_name, name, txt, 0 );       */
/*   }                                                                                                             */
/*   return r;                                                                                                     */
/* }                                                                                                               */

#ifdef MAS_OLD_VARIABLES_HTTP
mas_variable_t *
mas_thread_variables_find( const char *vclass_name, const char *name )
#else
mas_vclass_element_t *
mas_thread_variables_find( const char *vclass_name, const char *name )
#endif
{
#ifdef MAS_OLD_VARIABLES_HTTP
  mas_variable_t *found = NULL;
#else
  mas_vclass_element_t *found = NULL;
#endif
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    /* lock */
    if ( thd->variables )
    {
#ifdef MAS_OLD_VARIABLES_HTTP
      found = mas_variables_find( thd->variables, vclass_name, name );
#else
      found = mas_varset_find_variable( thd->variables, vclass_name, name );
#endif
    }
  }
  return found;
}

int
mas_thread_variable_set_text( th_type_t thtype, const char *vclass_name, const char *name, const char *txt )
{
  int r = 0;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( name && ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) )
  {
    /* lock */
#ifdef MAS_OLD_VARIABLES_HTTP
    thd->variables = mas_variable_set_text( thd->variables, /* thtype, */ vclass_name, name, txt );
#else
#endif
  }
  return r;
}
