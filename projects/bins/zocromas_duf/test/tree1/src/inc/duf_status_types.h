#ifndef MAS_DUF_STATUS_TYPES_H
#  define MAS_DUF_STATUS_TYPES_H
#  include "duf_ufilter_types.h"
#  include "duf_option_types.h"

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
   const duf_longval_extended_t * extended;
   char *optargg;
   duf_option_stage_t istage;
   int no;
   duf_option_source_t source;
} duf_xcmd_t;

typedef struct
{
  int actions_done;
  char *db_attached_selected;
  char *db_opened_name;
  /* int selection_done; */
#if 0
  duf_ufilter_t *selection_bound_ufilter;
#endif
  size_t alloc_xcmds;
  size_t n_xcmds;
  duf_xcmd_t *xcmds;
  duf_tmp_t *tmp;
} duf_status_t;

#endif

/*
vi: ft=c
*/
