#ifndef MAS_DUF_ITEM_TYPES_H
# define MAS_DUF_ITEM_TYPES_H

typedef struct duf_items_s duf_items_t;

struct duf_items_s
{
  unsigned long long files;
  unsigned long long dirs;
  unsigned long long total;
};

#endif

/*
vi: ft=c
*/
