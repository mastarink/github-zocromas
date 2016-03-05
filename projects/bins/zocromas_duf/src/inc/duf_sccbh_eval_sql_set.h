#ifndef MAS_DUF_SCCBH_EVAL_SQL_SET_H
# define MAS_DUF_SCCBH_EVAL_SQL_SET_H

# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, eval_sccbh_sql_set_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage );

#endif
