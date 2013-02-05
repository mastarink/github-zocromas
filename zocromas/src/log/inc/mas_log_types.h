#ifndef MAS_LOG_TYPES_H
#  define MAS_LOG_TYPES_H

#  include "mas_list_def.h"
#  include "mas_th_types.h"

#  include <mastar/channel/mas_channel_types.h>

#  include "listener/inc/mas_listener_control_types.h"
#  include "transaction/inc/mas_transaction_control_types.h"

// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_loginfo_list_s, mas_loginfo_s ) mas_loginfo_list_head_t;
typedef MAS_LIST_ENTRY( mas_loginfo_s ) mas_loginfo_list_entry_t;
// *INDENT-ON*

struct mas_loginfo_s
{
  th_type_t thtype;
  pid_t pid;
  pthread_t pth;
  char *func;
  int line;
  char *message;
  mas_loginfo_list_entry_t next;
  const mas_channel_t *pchannel;
  const mas_lcontrol_t *plcontrol;
  const mas_rcontrol_t *prcontrol;
  unsigned long lserial;
  unsigned lstatus;
  unsigned long rserial;
  unsigned long serial;
  unsigned rstatus;
  double logtime;
  int lerrno;
};
typedef struct mas_loginfo_s mas_loginfo_t;



#endif
