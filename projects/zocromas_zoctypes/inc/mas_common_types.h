#ifndef MAS_COMMON_TYPES_H
#  define MAS_COMMON_TYPES_H

#  include <sys/time.h>
#  include <stdint.h>
#  include <pthread.h>
#  include <mastar/types/mas_list_def.h>

typedef int ( *mas_error_handler_fun_t ) ( const char *func, int line, int issys, int rcode, int ierrno, int *perrno, const char *fmt,
                                           const char *xmsg );
typedef void ( *mas_voidp_fun_t ) ( void *arg );

typedef struct mas_string_csetv_s
{
  int c;
  char *const *v;
} mas_string_csetv_t;

typedef struct mas_string_setv_s
{
  int c;
  char **v;
} mas_string_setv_t;


typedef enum
{
  MAS_STATUS_NONE,
  MAS_STATUS_START,
  MAS_STATUS_INIT,
  MAS_STATUS_OPEN,
  MAS_STATUS_WAIT,
  MAS_STATUS_WORK,
  MAS_STATUS_CLOSE,
  MAS_STATUS_STOP,
  MAS_STATUS_END,
} mas_status_t;

struct mas_stamp_s
{
  char vtsc[16];
  /* uint64_t vdate; */
  /* uint64_t vtime; */
  /* uint64_t vts;   */
  uint64_t lts;
  uint64_t first_lts;
  uint64_t prev_lts;
  uint64_t start_time;
};
typedef struct mas_stamp_s mas_stamp_t;

typedef enum
{
  MSG_BIN_NONE,
  MSG_BIN_EMPTY_COMMAND,
  MSG_BIN_UNKNOWN_COMMAND,
  MSG_BIN_ERROR_IN_COMMAND,
  MSG_BIN_OPTS,
  MSG_BIN_QUIT,
  MSG_BIN_EXIT,
  MSG_BIN_RESTART,
  MSG_BIN_DISCONNECT,
} bin_type_t;


// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_ocontrol_list_s, mas_ocontrol_s ) mas_ocontrol_list_head_t;
typedef MAS_LIST_ENTRY( mas_ocontrol_s ) mas_ocontrol_list_entry_t;
// *INDENT-ON*

typedef struct mas_ocontrol_s
{
  /* mas_ocontrol_list_entry_t next; */
  /* unsigned id; */
  unsigned long serial;

  pthread_t thread;
  pid_t tid;

  struct mas_channel_s *pchannel;
  mas_status_t status;
  struct timeval activity_time;
} mas_ocontrol_t;

// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_ucontrol_list_s, mas_ucontrol_s ) mas_ucontrol_list_head_t;
typedef MAS_LIST_ENTRY( mas_ucontrol_s ) mas_ucontrol_list_entry_t;
// *INDENT-ON*

typedef struct mas_ucontrol_s
{
  mas_ucontrol_list_entry_t next;
  struct mas_ocontrol_s h;
} mas_ucontrol_t;

 /* \(serial\|thread\|tid\|pchannel\|status\|activity_time\) */
typedef size_t( *mas_xvsnprintf_t ) ( char *str, size_t size, const char *format, va_list args );

#endif
