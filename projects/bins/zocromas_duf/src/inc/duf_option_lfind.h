#ifndef MAS_DUF_OPTION_LFIND_H
#  define MAS_DUF_OPTION_LFIND_H

# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t */



const duf_longval_extended_t *duf_loption_xfind_at_stdx( int longindex, const duf_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo );
const duf_option_t *duf_loption_find_at_std( int longindex );


#endif
