#ifndef MAS_TRANSACTION_CONTROL_TYPES_H
#  define MAS_TRANSACTION_CONTROL_TYPES_H

#  include "mas_common_types.h"
#  include "channel/inc/mas_channel_types.h"

// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_rcontrol_list_s, mas_rcontrol_s ) mas_rcontrol_list_head_t;
typedef MAS_LIST_ENTRY( mas_rcontrol_s ) mas_rcontrol_list_entry_t;
// *INDENT-ON*

typedef enum mas_transaction_protocol_s
{
  MAS_TRANSACTION_PROTOCOL_NONE,
  MAS_TRANSACTION_PROTOCOL_XCROMAS,
  MAS_TRANSACTION_PROTOCOL_HTTP,
} mas_transaction_protocol_t;

/* struct mas_r_control_s                          */
/* {                                               */
/*   unsigned id;                                  */
/*   pthread_t thread;                             */
/* };                                              */
/* typedef struct mas_r_control_s mas_r_control_t; */

struct mas_rcontrol_s
{
  /* unsigned id; */
  pthread_t thread;
  pid_t tid;
  double start_time;
  struct mas_lcontrol_s *plcontrol;
  struct mas_channel_s *pchannel;
  /* struct sockaddr_in addr; */
  /* mas_serv_addr_t serv; */
  /* int port;             */
  /* mas_status_t status; */
  mas_rcontrol_list_entry_t next;
  unsigned long serial;
  unsigned nc;
  mas_status_t status;
  unsigned xch_cnt;
  char *uuid;
  mas_transaction_protocol_t proto;
  pthread_cond_t waitchan_cond;
  pthread_mutex_t waitchan_mutex;
  struct timeval activity_time;
  unsigned complete:1;
  unsigned keep_alive:1;
#  ifdef MAS_TR_PERSIST
  unsigned persistent_transaction:1;
#  endif
  unsigned waitchan_waiting:1;
};
typedef struct mas_rcontrol_s mas_rcontrol_t;


#endif
