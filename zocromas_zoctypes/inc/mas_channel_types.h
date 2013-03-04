#ifndef MAS_CHANNEL_TYPES_H
#  define MAS_CHANNEL_TYPES_H

#  include <stdio.h>
#  include <sys/socket.h>
#  include <sys/un.h>
#  include <arpa/inet.h>

typedef enum
{
  CHN_NONE,
  CHN_RFILE,
  CHN_WFILE,
  CHN_SOCKET,
} chn_type_t;

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

struct mas_channel_buffer_s
{
  size_t maxread;
  size_t size;
  ssize_t length;
  char *buffer;
  ssize_t iptr;
  unsigned feof:1;
};
typedef struct mas_channel_buffer_s mas_channel_buffer_t;

struct mas_channel_s
{
  /* pid_t pid; */
  chn_type_t type;
  int fd_socket;
  int fd_io;
  FILE *stream_io;
  mas_serv_addr_t serv;
  mas_serv_addr_t serv_instance;
  mas_serv_addr_t cli;
  int port;
  char *host;
  char *error_func;
  int error_line;
  int error_errno;
  mas_proto_t proto;
  mas_channel_buffer_t buffer;
  unsigned is_server:1;
  unsigned listening:1;
  unsigned opened:1;
  unsigned cloned:1;
  unsigned feof:1;
  unsigned error:1;
};
typedef struct mas_channel_s mas_channel_t;

#endif
