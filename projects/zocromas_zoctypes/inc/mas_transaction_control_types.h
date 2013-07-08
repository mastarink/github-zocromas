#ifndef MAS_TRANSACTION_CONTROL_TYPES_H
#  define MAS_TRANSACTION_CONTROL_TYPES_H

#  include <mastar/types/mas_common_types.h>
#  include <mastar/types/mas_list_def.h>

/* #  include <mastar/types/mas_varvec_types.h> */
#  include <mastar/types/mas_varset_types.h>

#  include <mastar/types/mas_channel_types.h>
#  include <mastar/types/mas_opts_types.h>

/*
this:
  mas_transaction_control_types.h
related:
  mas_common_types.h
  mas_listener_control_types.h
*/

typedef struct mas_rcontrol_s mas_rcontrol_t;
typedef struct mas_transaction_protodesc_s mas_transaction_protodesc_t;
typedef int ( *mas_transaction_fun_t ) ( mas_rcontrol_t * prcontrol, const void *pvoid );

struct mas_transaction_protodesc_s
{
  char *name;
  unsigned proto_id;
  mas_transaction_fun_t func;
  mas_varset_t *variables;
};


// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_rcontrol_list_s, mas_rcontrol_s ) mas_rcontrol_list_head_t;
typedef MAS_LIST_ENTRY( mas_rcontrol_s ) mas_rcontrol_list_entry_t;
// *INDENT-ON*

/* typedef enum mas_transaction_protocol_s */
/* {                                       */
/*   MAS_TRANSACTION_PROTOCOL_NONE,        */
/*   MAS_TRANSACTION_PROTOCOL_XCROMAS,     */
/*   MAS_TRANSACTION_PROTOCOL_HTTP,        */
/* } mas_transaction_protocol_t;           */


/* struct mas_r_control_s                          */
/* {                                               */
/*   unsigned id;                                  */
/*   pthread_t thread;                             */
/* };                                              */
/* typedef struct mas_r_control_s mas_r_control_t; */

struct mas_rcontrol_s
{
  char signature[2];
  mas_rcontrol_list_entry_t next;
  struct mas_ocontrol_s h;

  double start_time;
  struct mas_lcontrol_s *plcontrol;

  /* const struct mas_options_s *popts; */

  bin_type_t qbin;
  /* struct sockaddr_in addr; */
  /* mas_serv_addr_t serv; */
  /* int port;             */
  /* mas_status_t status; */
  unsigned nc;
  unsigned xch_cnt;
  char *uuid;
  const mas_transaction_protodesc_t *proto_desc;
  pthread_cond_t waitchan_cond;
  pthread_mutex_t waitchan_mutex;
  unsigned complete:1;
  unsigned connection_keep_alive:1;
#  ifdef MAS_TR_PERSIST
  unsigned persistent_transaction:1;
#  endif
  unsigned waitchan_waiting:1;
  unsigned stop:1;
};


#endif
