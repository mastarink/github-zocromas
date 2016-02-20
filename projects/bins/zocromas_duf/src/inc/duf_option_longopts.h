#ifndef MAS_DUF_OPTION_LONGOPTS_H
# define MAS_DUF_OPTION_LONGOPTS_H

# include "duf_option_types.h"                                       /* duf_option_t; duf_longval_extended_vtable_t */

duf_option_t *duf_options_create_longopts_table( duf_longval_extended_vtable_t ** xvtable );
void duf_options_delete_longopts_table( duf_option_t * longopts );

#endif
