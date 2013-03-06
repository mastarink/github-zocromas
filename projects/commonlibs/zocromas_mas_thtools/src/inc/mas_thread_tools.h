#ifndef MAS_THREAD_TOOLS_H
#  define MAS_THREAD_TOOLS_H

#  include <mastar/types/mas_listener_control_types.h>
#  include <mastar/types/mas_thread_variables_types.h>

void mas_thread_make_key( void );
extern pthread_once_t mas_thread_key_once;
extern pthread_key_t mas_thread_key;



/* mas_lcontrol_t *mas_find_listener_control( pthread_t pth, int *ppos );                                */
/* mas_rcontrol_t *mas_find_transaction_control( mas_lcontrol_t * plcontrol, pthread_t pth, int *ppos ); */

mas_thdata_t *mas_in_thread( th_type_t thtype, mas_lcontrol_t * plcontrol, mas_rcontrol_t * prcontrol /*, mas_channel_t * pchannel */  );
void mas_in_thread_end( void );


th_type_t mas_thself_type( void );
const char *mas_thread_self_type_name( void );


mas_lcontrol_t *mas_thself_plcontrol( void );
mas_rcontrol_t *mas_thself_prcontrol( void );
mas_channel_t *mas_thself_pchannel( void );

int mas_xxpthread_join( pthread_t thread, long nanos );
int mas_xxspthread_join( pthread_t thread, long sec, long nanos );
int mas_xxtspthread_join( pthread_t thread, struct timespec *jwait );



const char *mas_thread_type_name( th_type_t typ );




#endif
