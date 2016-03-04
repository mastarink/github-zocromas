#ifndef MAS_STD_MOD_SETS_H
# define MAS_STD_MOD_SETS_H

# include "duf_sql_set_types.h"                                      /* duf_sql_set_t */

# if 0
extern duf_sql_set_t std_leaf_set_num;
extern duf_sql_set_t std_node_set_num;

extern duf_sql_set_t std_ns_leaf_set;
extern duf_sql_set_t std_ns_node_set;
# else

extern duf_sql_set_t std_leaf_sets[];
extern int std_leaf_nsets;

extern duf_sql_set_t std_node_sets[];
extern int std_node_nsets;
# endif

#endif
