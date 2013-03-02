#ifndef MAS_MESSAGE_TYPES_H
#  define MAS_MESSAGE_TYPES_H

#  include <stdint.h>
#  include <mastar/types/mas_common_types.h>

struct mas_header_old_s
{
  uint32_t sign;
  char z;
  uint32_t vers;
  /* pid_t pid; */
  uint64_t pid;
  /* pthread_t pth; */
  uint64_t pth;
  mas_stamp_t sender_stamp;
  unsigned new_opts:1;
  unsigned bad:1;
  unsigned done_cmd:1;
  unsigned no_len:1;
  unsigned direct_output:1;
  unsigned x1:11;
  bin_type_t binary;
  uint16_t len;
};
struct mas_header_s
{
  uint32_t vers:32;
  uint32_t sign:32;
  char z;

  uint32_t x1;
  uint32_t x2:19;
  unsigned new_opts:1;
  unsigned bad:1;
  unsigned done_cmd:1;
  unsigned no_len:1;
  unsigned direct_output:1;

  /* pid_t pid; */
  uint32_t pid;
  /* pthread_t pth; */
  uint64_t pth;
  mas_stamp_t sender_stamp;

  bin_type_t binary;
  uint16_t len;
  uint16_t restart_cnt;
};

typedef struct mas_header_s mas_header_t;


struct mas_message_s
{
  mas_header_t h;
  char message[1];
};
typedef struct mas_message_s mas_message_t;

#endif
