#ifndef MAS_ARGVC_TOOLS_H
#  define MAS_ARGVC_TOOLS_H

#  include <sys/types.h>

typedef struct
{
  int argc;
  char **argv;
} mas_argvc_t;
typedef struct
{
  int argc;
  char *const *argv;
} mas_cargvc_t;

int mas_add_argvc_args( mas_argvc_t * targ, const char *args, int ia_offset );
int mas_add_argvc_argvc( mas_argvc_t * targ, mas_argvc_t * arg, int ia_offset );
int mas_add_argvc_arg_nodup( mas_argvc_t * targ, char *arg );
int mas_add_argvc_arg( mas_argvc_t * targ, const char *arg );
int mas_add_argvc_cmdline( mas_argvc_t * targ, const char *cmdline, size_t len );
int mas_del_argvc( mas_argvc_t * targ, int from_a );
int mas_argvc_delete( mas_argvc_t * targ );
int mas_argvc_clone( mas_argvc_t * darg, mas_argvc_t * sarg );
char *mas_argvc_join( mas_argvc_t * targ, int from_a, char delim );
char *mas_argvc_string( mas_argvc_t * targ, int from_a );

#endif