#ifndef MAS_DUF_ERROR_REPORTING_H
#  define MAS_DUF_ERROR_REPORTING_H

#  include <stdarg.h>

#  include "duf_error_types.h"

void duf_force_count_ereport( int count );
void duf_force_fixed_ereport( int count );
void duf_force_offset_ereport( int count );


void duf_enable_mereport_c( int once, int enable, int abs, ... );
void duf_venable_mereport_c( int once, int enable, int abs, va_list args );

/* void duf_enable_ereport_c( int once, int enable, int abs, duf_error_code_t rtest ); */

int duf_enabled_ereport_n_c( duf_error_code_t rtest );
int duf_enabled_ereport_n_i( duf_error_index_t ri );

int duf_enabled_ereport_i( duf_error_index_t rtest );

int duf_ecount_reported_c( duf_error_code_t rtest );
int duf_ecount_reported_i( duf_error_code_t ri );
int duf_ecount_reported_rev( duf_error_event_t * rev );


void duf_set_emax_count_c( int maxcount, duf_error_code_t rtest );
void duf_vset_emax_count_c( int maxcount, va_list args );
void duf_set_memax_count_c( int maxcount, ... );



int duf_error_line_i( duf_error_index_t e );
const char *duf_error_func_i( duf_error_index_t e );
const char *duf_error_message_i( duf_error_index_t e );

void duf_error_report_i( duf_error_code_t ri, int test, FILE* out, int verb );
void duf_error_report_p( size_t rp, int test, FILE* out, int verb );
void duf_error_report_all( int test, FILE* out, int verb );

#endif
