#ifndef MAS_DUF_STATUS_TYPES_H
#  define MAS_DUF_STATUS_TYPES_H

typedef struct
{
  int actions_done;
  int selection_done;
  unsigned long long changes;
  unsigned long long total_files;
} duf_status_t;

#endif
