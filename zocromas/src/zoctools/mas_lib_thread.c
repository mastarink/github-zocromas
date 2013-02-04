#include "mas_basic_def.h"

#include <pthread.h>

#include "mas_common.h"

#include "mas_lib_thread.h"

/*
this:
  mas_lib_thread.c
related:
  mas_tools.c

*/


int
mas_pthread_create( pthread_t * thread, const pthread_attr_t * attr, void *( *start_routine ) ( void * ), void *arg )
{
  return pthread_create( thread, attr, start_routine, arg );
}

int
mas_xpthread_create( pthread_t * thread, void *( *start_routine ) ( void * ), th_type_t thtype, void *arg )
{
  return pthread_create( thread, &ctrl.thglob.custom_attr, start_routine, arg );
}

int
mas_pthread_cancel( pthread_t th )
{
  return pthread_cancel( th );
}

int
mas_pthread_mutex_lock( pthread_mutex_t * mutex )
{
  return pthread_mutex_lock( mutex );
}

int
mas_pthread_mutex_trylock( pthread_mutex_t * mutex )
{
  return pthread_mutex_trylock( mutex );
}

int
mas_pthread_mutex_unlock( pthread_mutex_t * mutex )
{
  return pthread_mutex_unlock( mutex );
}

int
mas_pthread_join( pthread_t thread, void **retval )
{
  return pthread_join( thread, retval );
}

int
mas_xpthread_join( pthread_t thread )
{
  return pthread_join( thread, NULL );
}

pthread_t
mas_pthread_self( void )
{
  return pthread_self(  );
}

int
mas_pthread_tryjoin_np( pthread_t thread, void **retval )
{
  return pthread_tryjoin_np( thread, retval );
}

int
mas_xpthread_tryjoin_np( pthread_t thread )
{
  return pthread_tryjoin_np( thread, NULL );
}

int
mas_pthread_timedjoin_np( pthread_t thread, void **retval, const struct timespec *abstime )
{
  return pthread_timedjoin_np( thread, retval, abstime );
}

int
mas_xpthread_timedjoin_np( pthread_t thread,  const struct timespec *abstime )
{
  return pthread_timedjoin_np( thread, NULL, abstime );
}

int
mas_pthread_attr_init( pthread_attr_t * attr )
{
  return pthread_attr_init( attr );
}

int
mas_pthread_attr_destroy( pthread_attr_t * attr )
{
  return pthread_attr_destroy( attr );
}

int
mas_pthread_detach( pthread_t thread )
{
  return pthread_detach( thread );
}

void
mas_pthread_exit( void *retval )
{
  pthread_exit( retval );
}

/* int                                                                                                   */
/* mas_pthread_mutex_destroy( pthread_mutex_t * mutex )                                                  */
/* {                                                                                                     */
/*   return mas_pthread_mutex_destroy( mutex );                                                          */
/* }                                                                                                     */
/*                                                                                                       */
/* int                                                                                                   */
/* mas_pthread_mutex_init( pthread_mutex_t * restrict mutex, const pthread_mutexattr_t * restrict attr ) */
/* {                                                                                                     */
/*   return pthread_mutex_init( mutex, attr );                                                           */
/* }                                                                                                     */
/*                                                                                                       */
/* int                                                                                                   */
/* mas_xpthread_mutex_destroy( pthread_mutex_t * mutex )                                                 */
/* {                                                                                                     */
/*   return mas_pthread_mutex_destroy( mutex );                                                          */
/* }                                                                                                     */
/*                                                                                                       */
/* int                                                                                                   */
/* mas_xpthread_mutex_init( pthread_mutex_t * restrict mutex )                                           */
/* {                                                                                                     */
/*   return pthread_mutex_init( mutex, NULL );                                                           */
/* }                                                                                                     */
