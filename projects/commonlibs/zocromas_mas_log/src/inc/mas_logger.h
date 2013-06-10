#ifndef MAS_LOGGER_H
#  define MAS_LOGGER_H
#  define MAS_LIST_QH

#  include "mas_log_types.h"

mas_loginfo_list_head_t *mas_logger_queue( int create );
void mas_logger_delete( int stopever );

#  ifndef MAS_NO_THREADS
int mas_logger_start( MAS_PASS_OPTS_DECLARE1 );
int mas_logger_stop( void );
#  endif

int mas_logger_flush( void );
int mas_logger_close( void );
void mas_logger_cleanup( void *arg );

#  ifndef MAS_NO_THREADS
/* extern pthread_mutex_t logger_queue_mutex ; */
extern pthread_rwlock_t logger_queue_rwlock;

/* extern pthread_mutex_t logger_mutex; */
#  endif

#endif
