#ifndef MAS_STD_MOD_SETS_H
#  define MAS_STD_MOD_SETS_H

#  if 0
extern duf_sql_set_t std_leaf_set_to_obsolete;
extern duf_sql_set_t std_node_set_to_obsolete;

extern duf_sql_set_t std_ns_leaf_set;
extern duf_sql_set_t std_ns_node_set;
#  else

extern duf_sql_set_t std_leaf_sets[];
extern size_t std_leaf_nsets;

extern duf_sql_set_t std_node_sets[];
extern size_t std_node_nsets;
#  endif

#endif
