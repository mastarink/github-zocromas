#ifndef MAS_DUF_UFILTER_REF_H
# define MAS_DUF_UFILTER_REF_H

# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_item_types.h"                                         /* duf_items_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

int duf_ufilter_max_rel_depth( const duf_ufilter_t * pu );
int duf_ufilter_max_filter( const duf_ufilter_t * pu, unsigned seq, const duf_items_t * pitems );

int duf_ufilter_use_format( const duf_ufilter_t * pu );
int duf_ufilter_std_leaf_set_num( const duf_ufilter_t * pu );
int duf_ufilter_orderid( const duf_ufilter_t * pu );
const char *duf_ufilter_std_leaf_set_name( const duf_ufilter_t * pu );

#endif
