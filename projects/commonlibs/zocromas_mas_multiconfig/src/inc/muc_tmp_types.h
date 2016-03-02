#ifndef MAS_MUC_TMP_TYPES_H
#  define MAS_MUC_TMP_TYPES_H


#  define  MUC_TMP_INDEX_MAX 6

typedef struct
{
  unsigned tmp_index;
  char *tmp_string[MUC_TMP_INDEX_MAX];
} muc_tmp_t;

#endif
