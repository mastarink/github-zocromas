#ifndef MAS_MUC_OPTION_LONGOPTS_H
# define MAS_MUC_OPTION_LONGOPTS_H

# include "muc_option_types.h"                                       /* muc_option_t; muc_longval_extended_vtable_t */

muc_option_t *muc_options_create_longopts_table( muc_longval_extended_vtable_t ** xvtable );
void muc_options_delete_longopts_table( muc_option_t * longopts );

#endif
