#ifndef MAS_DUF_SEQ_STRUCTS_H
# define MAS_DUF_SEQ_STRUCTS_H

# include "duf_seq_types.h"

struct seq_s
{
  unsigned long long gen;
  unsigned long long leaf;
  unsigned long long node;
  unsigned long long row;
};

#endif

/*
vi: ft=c
*/
