#ifndef MAS_RUNONCE_H

typedef struct
{
  pid_t ppid;
  char *progname;
  int argc;
  char **argv;
} prec_t;

typedef const char *( fun_t ) ( pid_t pid, const char *line, const char *eline );
#endif
