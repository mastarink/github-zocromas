#ifndef MAS_DUF_ERRORS_H
#  define MAS_DUF_ERRORS_H

#  include <stdarg.h>

#  include "duf_error_types.h"

void duf_force_count_ereport( int count );
void duf_force_fixed_ereport( int count );
void duf_force_offset_ereport( int count );

duf_error_index_t duf_make_error_c( duf_error_code_t err, const char *funcid, int linid, const char *message );
duf_error_code_t duf_error_code_i( duf_error_index_t e );


long duf_made_errors( void );

duf_error_code_t duf_vclear_error_c( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error_c( duf_error_code_t r, ... );

duf_error_index_t duf_clear_error_i( duf_error_index_t e, ... );

int duf_errnumber_c( duf_error_code_t rtest );
int duf_errnumber_i( duf_error_code_t rtest );
const char *duf_error_name_c( duf_error_code_t c );
const char *duf_error_name_i( duf_error_index_t e );

void duf_enable_mereport_c( int once, int enable, int abs, ... );
void duf_venable_mereport_c( int once, int enable, int abs, va_list args );

/* void duf_enable_ereport_c( int once, int enable, int abs, duf_error_code_t rtest ); */

int duf_enabled_ereport_n_c( duf_error_code_t rtest );
int duf_enabled_ereport_n_i( duf_error_index_t ri );

int duf_enabled_ereport_i( duf_error_index_t rtest );

int duf_ecount_c( duf_error_code_t rtest );
int duf_ecount_i( duf_error_code_t ri );


void duf_set_emax_count_c( int maxcount, duf_error_code_t rtest );
void duf_vset_emax_count_c( int maxcount, va_list args );
void duf_set_memax_count_c( int maxcount, ... );



int duf_error_line_i( duf_error_index_t e );
const char *duf_error_func_i( duf_error_index_t e );
const char *duf_error_message_i( duf_error_index_t e );


#endif
