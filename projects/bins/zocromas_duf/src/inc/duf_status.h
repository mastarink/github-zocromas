#ifndef MAS_DUF_STATUS_H
# define MAS_DUF_STATUS_H

/* # include "duf_option_types.h"                                       (* duf_longval_extended_t; duf_longval_extended_vtable_t ♠ *) */

/* void                                                                                                                          */
/* global_status_register_xcmd( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage, unsigned noo, */
/*                              duf_option_source_t source );                                                                    */
const char *global_status_explanation_tmp( int index, const muc_longval_extended_t * extended, int *pr );
void global_status_reset( void );

#endif
