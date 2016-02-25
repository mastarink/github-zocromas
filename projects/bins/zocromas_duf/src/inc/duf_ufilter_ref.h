#ifndef MAS_DUF_UFILTER_REF_H
# define MAS_DUF_UFILTER_REF_H

# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ♠ */

int duf_ufilter_max_rel_depth( const duf_ufilter_t * pu );
int duf_ufilter_max_filter( const duf_ufilter_t * pu, unsigned seq, const duf_items_t * pitems );

#endif
