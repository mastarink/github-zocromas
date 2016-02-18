#ifndef MAS_DUF_CONFIG_OPT_DEBUG_TYPES_H
#define MAS_DUF_CONFIG_OPT_DEBUG_TYPES_H

typedef struct
{
  int debug;
  unsigned nosqlite;
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_opt_debug_t;


#endif
