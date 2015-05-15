#ifndef MAS_DUF_STATUS_TYPES_H
#  define MAS_DUF_STATUS_TYPES_H
#  include "duf_ufilter_types.h"

#  define  DUF_TMP_PATH_MAX 6
#  define  DUF_TMP_EXPLANATION_MAX 6

typedef struct
{
  char *path[DUF_TMP_PATH_MAX];
  unsigned explanation_index;
  char *option_explanation[DUF_TMP_EXPLANATION_MAX];
} duf_tmp_t;


typedef struct
{
  int actions_done;
  /* int selection_done; */
  duf_ufilter_t *selection_bound_ufilter;
  duf_tmp_t *tmp;
} duf_status_t;

#endif

/*
vi: ft=c
*/
