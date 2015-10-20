#ifndef MAS_ERROR_BASE_H
#  define MAS_ERROR_BASE_H

#  include <stdarg.h>
#  include <unistd.h>

#  include "mas_error_types.h"

mas_error_code_t mas_error_code_p( size_t rp );
mas_error_code_t mas_error_code_i( mas_error_index_t e );
mas_error_code_t mas_error_code_rev( const mas_error_event_t * rev );

int mas_errnumber_c( mas_error_code_t rtest );
int mas_errnumber_i( mas_error_code_t rtest );

const char *mas_error_name_p( size_t rp );
const char *mas_error_name_i( mas_error_index_t ri );
const char *mas_error_name_c( mas_error_code_t rc );
const char *mas_error_name_rev( const mas_error_event_t * rev );


int mas_error_line_p( size_t rp );
int mas_error_line_i( mas_error_index_t ri );
int mas_error_line_rev( const mas_error_event_t * rev );

const char *mas_error_func_p( size_t rp );
const char *mas_error_func_i( mas_error_index_t ri );
const char *mas_error_func_rev( const mas_error_event_t * rev );

const char *mas_error_message_p( size_t rp );
const char *mas_error_message_i( mas_error_index_t ri );
const char *mas_error_message_rev( const mas_error_event_t * rev );

void mas_vset_error_message_p( size_t rp, const char *fmt, va_list args );
void mas_vset_error_message_i( mas_error_index_t ri, const char *fmt, va_list args );
void mas_set_error_message_rev( mas_error_event_t * rev, const char *fmt, ... );
void mas_set_error_message_p( size_t rp, const char *fmt, ... );
void mas_set_error_message_i( mas_error_index_t ri, const char *fmt, ... );

int mas_icount_reported_i( mas_error_index_t ri );
int mas_icount_reported_rev( const mas_error_event_t * rev );

#endif
