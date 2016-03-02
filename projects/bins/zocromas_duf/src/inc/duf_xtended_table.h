#ifndef MAS_DUF_XTENDED_TABLE_H
# define MAS_DUF_XTENDED_TABLE_H

# include "duf_option_types.h"                                       /* duf_longval_extended_table_t */

/* extern const duf_longval_extended_table_t **lo_extended_table_multi; */

/* const duf_longval_extended_table_t **duf_extended_table_multi( void ); (* obsolete/not used 20160208.130331 *) */

/* const duf_longval_extended_vtable_t *const *duf_extended_vtable_multi( void ); */
const muc_longval_extended_table_t *const *duf_xtable_list( void );

#endif
