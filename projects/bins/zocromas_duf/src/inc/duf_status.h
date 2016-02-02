#ifndef MAS_DUF_STATUS_H
#  define MAS_DUF_STATUS_H

/* void                                                                                                                          */
/* global_status_register_xcmd( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage, int no, */
/*                              duf_option_source_t source );                                                                    */
const char *global_status_explanation_tmp( int index, const duf_longval_extended_t * extended, int *pr );
void global_status_reset( void );

#endif
