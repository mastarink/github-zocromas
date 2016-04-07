#ifndef MAS_DUF_MOD_TYPES_H
# define MAS_DUF_MOD_TYPES_H

struct duf_mod_handler_s
{
  const char *name;
  void *handler;
};

typedef struct duf_mod_handler_s duf_mod_handler_t;

#endif
