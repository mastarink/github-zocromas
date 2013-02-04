#ifndef MAS_CHANNEL_TYPES_H
#  define MAS_CHANNEL_TYPES_H

#  include <sys/socket.h>
#  include <sys/un.h>
#  include <arpa/inet.h>

typedef enum
{ CHN_SOCKET } chn_type_t;

typedef union
{
  struct sockaddr_un path;
  struct sockaddr_in addr;
} mas_serv_addr_t;

typedef enum
{
  MSG_PROTO_UNKNOWN,
  MSG_PROTO_MASSPC,
  MSG_PROTO_HTTP
} mas_proto_t;

struct mas_channel_s
{
  /* pid_t pid; */
  chn_type_t type;
  int fd_socket;
  int fd_io;
  FILE *stream_io;
  mas_serv_addr_t serv;
  mas_serv_addr_t cli;
  int port;
  char *host;
  char *error_func;
  int error_line;
  int error_errno;
  mas_proto_t proto;
  unsigned listening:1;
  unsigned opened:1;
  unsigned cloned:1;
  unsigned error:1;
};
typedef struct mas_channel_s mas_channel_t;

#endif
