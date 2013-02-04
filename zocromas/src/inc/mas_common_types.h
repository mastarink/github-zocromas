#ifndef MAS_COMMON_TYPES_H
#define MAS_COMMON_TYPES_H

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
  char *vtsc;
  unsigned long vdate;
  unsigned long vtime;
  unsigned long vts;
  unsigned long lts;
  unsigned long first_lts;
  unsigned long prev_lts;
  unsigned long start_time;
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


#endif
