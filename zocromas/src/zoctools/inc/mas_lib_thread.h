#ifndef MAS_LIB_THREAD_H
#  define MAS_LIB_THREAD_H

int mas_pthread_cancel( pthread_t th );

int mas_pthread_create( pthread_t * thread, const pthread_attr_t * attr, void *( *start_routine ) ( void * ), void *arg );
int mas_xpthread_create( pthread_t * thread, void *( *start_routine ) ( void * ), th_type_t thtype, void *arg );

int mas_pthread_mutex_lock( pthread_mutex_t * mutex );
int mas_pthread_mutex_trylock( pthread_mutex_t * mutex );
int mas_pthread_mutex_unlock( pthread_mutex_t * mutex );

pthread_t mas_pthread_self( void );

int mas_pthread_join( pthread_t thread, void **retval );
int mas_xpthread_join( pthread_t thread );

int mas_pthread_tryjoin_np( pthread_t thread, void **retval );
int mas_xpthread_tryjoin_np( pthread_t thread );
int mas_pthread_timedjoin_np( pthread_t thread, void **retval, const struct timespec *abstime );
int mas_xpthread_timedjoin_np( pthread_t thread, const struct timespec *abstime );

int mas_pthread_attr_init( pthread_attr_t * attr );
int mas_pthread_attr_destroy( pthread_attr_t * attr );

int mas_pthread_detach( pthread_t thread );
void mas_pthread_exit( void *retval );

/* int mas_pthread_mutex_destroy( pthread_mutex_t * mutex );                                                  */
/* int mas_pthread_mutex_init( pthread_mutex_t * restrict mutex, const pthread_mutexattr_t * restrict attr ); */
/* int mas_xpthread_mutex_destroy( pthread_mutex_t * mutex );                                                 */
/* int mas_xpthread_mutex_init( pthread_mutex_t * restrict mutex );                                           */

#  undef MAS_WEAK
#endif
