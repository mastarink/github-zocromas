#ifndef MAS_ERROR_REPORTING_H
#  define MAS_ERROR_REPORTING_H

#  include <stdarg.h>
#  include <stdio.h>

#  include "mas_error_types.h"

void mas_force_count_ereport( int count );
void mas_force_fixed_ereport( int count );
void mas_force_offset_ereport( int count );


void mas_enable_mereport_c( int once, int enable, int abs, ... );
void mas_venable_mereport_c( int once, int enable, int abs, va_list args );

/* void mas_enable_ereport_c( int once, int enable, int abs, mas_error_code_t rtest ); */

int mas_enabled_ereport_n_c( mas_error_code_t rtest );
int mas_enabled_ereport_n_i( mas_error_index_t ri );

int mas_enabled_ereport_i( mas_error_index_t rtest );

int mas_ecount_reported_c( mas_error_code_t rtest );
int mas_ecount_reported_i( mas_error_index_t ri );
int mas_ecount_reported_rev( mas_error_event_t * rev );


void mas_set_emax_count_c( int maxcount, mas_error_code_t rtest );
void mas_vset_emax_count_c( int maxcount, va_list args );
void mas_set_memax_count_c( int maxcount, ... );



int mas_error_line_i( mas_error_index_t e );
const char *mas_error_func_i( mas_error_index_t e );
const char *mas_error_message_i( mas_error_index_t e );

void mas_error_report_i( mas_error_index_t ri, int test, FILE* out, int verb );
void mas_error_report_p( size_t rp, int test, FILE* out, int verb );
void mas_error_report_all( int test, FILE* out, int verb );

#endif
