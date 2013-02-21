#ifndef MAS_MESSAGE_TYPES_H
#  define MAS_MESSAGE_TYPES_H

#  include <mastar/types/mas_common_types.h>

struct mas_header_s
{
  unsigned sign;
  char z;
  unsigned vers;
  pid_t pid;
  pthread_t pth;
  mas_stamp_t sender_stamp;
  unsigned new_opts:1;
  unsigned bad:1;
  unsigned done_cmd:1;
  unsigned no_len:1;
  unsigned direct_output:1;
  bin_type_t binary;
  int len;
};
typedef struct mas_header_s mas_header_t;


struct mas_message_s
{
  mas_header_t h;
  char message[1];
};
typedef struct mas_message_s mas_message_t;

#endif
