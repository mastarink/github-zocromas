#ifndef MASREGERR_STRUCTS_H
# define MASREGERR_STRUCTS_H

# include "masregerr_types.h"

struct masregerrs_s
{
  size_t size;
  masregerr_t *err_array;
  masregerr_t *current;
  long serial;
};
struct masregerr_s
{
  int mark;
  const char *func;
  const char *func1;
  const char *func2;
  int line;
  const char *file;
  const char *package;
  int err_no;
  int sys;
  long serial;
  void *data;
  char *msg;
};

#endif
