#ifndef MAS_ERROR_REPORTING_H
# define MAS_ERROR_REPORTING_H

# include <stdarg.h>
# include <stdio.h>

# include <mastar/trace/mas_trace_types.h>
# include "mas_error_types.h"

/* void mas_force_count_ereport( int count ); */
/* void mas_force_fixed_ereport( int count ); */
void mas_force_offset_ereport( int count );

void mas_mset_ereport_level_c( int once, int enable, int abs, ... );
void mas_vmset_ereport_level_c( int once, int enable, int abs, va_list args );

/* void mas_set_report_level_c( int once, int enable, int abs, mas_error_code_t rtest ); */

/* int mas_ereport_level_n_c( mas_error_code_t rtest ); */
/* int mas_ereport_level_n_i( mas_error_index_t ri ); */
int mas_ereport_level_c( mas_error_code_t rc );

int mas_enabled_ereport_level_i( mas_error_index_t rtest );

int mas_ecount_reported_plus_c( mas_error_code_t rtest, int test );
int mas_ecount_reported_plus_i( mas_error_index_t ri, int test );
int mas_ecount_reported_plus_rev( mas_error_event_t * rev, int test );

int mas_ecount_reported_i( mas_error_index_t ri );
int mas_ecount_reported_rev( const mas_error_event_t * rev );

void mas_set_emax_count_c( int maxcount, mas_error_code_t rtest );
void mas_vset_emax_count_c( int maxcount, va_list args );
void mas_set_memax_count_c( int maxcount, ... );
int mas_emax_count_c( mas_error_code_t rc );

int mas_error_line_i( mas_error_index_t e );
const char *mas_error_func_i( mas_error_index_t e );
const char *mas_error_message_i( mas_error_index_t e );

void mas_error_report_rev( const mas_config_trace_t * tcfg, int trace_errindex, mas_error_event_t * rev, int test, FILE * out, int verb );
void mas_error_report_i( const mas_config_trace_t * tcfg, int trace_errindex, mas_error_index_t ri, int test, FILE * out, int verb );
void mas_error_report_p( const mas_config_trace_t * tcfg, int trace_errindex, size_t rp, int test, FILE * out, int verb );
void mas_error_report_all( const mas_config_trace_t * tcfg, int trace_errindex, int test, FILE * out, int verb );

int _mas_enabled_ereport_c( mas_error_code_t rc, int maxerr );

#endif
