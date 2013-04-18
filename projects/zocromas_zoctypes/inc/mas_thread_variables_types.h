#ifndef MAS_THREAD_VARIABLES_TYPES_H
#  define MAS_THREAD_VARIABLES_TYPES_H

#  include <mastar/types/mas_th_types.h>
#  include <mastar/types/mas_listener_control_types.h>
#  include <mastar/types/mas_variables_types.h>
#  include <mastar/types/mas_transaction_control_types.h>

struct mas_thdata_s
{
  th_type_t type;
  mas_lcontrol_t *plcontrol;
  mas_rcontrol_t *prcontrol;
  /* mas_channel_t *pchannel; */
  mas_variables_list_head_t *variables;
};
typedef struct mas_thdata_s mas_thdata_t;

struct mas_thread_id_s
{
  /* th_type_t type; */
  /* listener (server) thread */
  int l;
  /* transaction thread */
  int r;
  mas_status_t status;
};
typedef struct mas_thread_id_s mas_thread_id_t;


#endif
