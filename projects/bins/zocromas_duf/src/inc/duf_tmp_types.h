#ifndef MAS_DUF_TMP_TYPES_H
#  define MAS_DUF_TMP_TYPES_H


#  define  DUF_TMP_INDEX_MAX 6

typedef struct
{
  unsigned tmp_index;
  char *tmp_string[DUF_TMP_INDEX_MAX];
} duf_tmp_t;

#endif
