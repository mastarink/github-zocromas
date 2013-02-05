#ifndef MAS_LOGGER_H
#  define MAS_LOGGER_H

#  include "mas_log_types.h"

int mas_logger_start( void );
int mas_logger_stop( void );

int mas_logger_flush( void );
int mas_logger_close( void );
void mas_logger_cleanup( void *arg );

/* extern pthread_mutex_t logger_queue_mutex ; */
extern pthread_rwlock_t logger_queue_rwlock;

/* extern pthread_mutex_t logger_mutex; */

#endif
