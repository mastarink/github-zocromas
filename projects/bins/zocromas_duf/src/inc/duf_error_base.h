#ifndef MAS_DUF_ERROR_BASE_H
#  define MAS_DUF_ERROR_BASE_H

#  include <stdarg.h>

#  include "duf_error_types.h"

duf_error_code_t duf_error_code_p( size_t rp );
duf_error_code_t duf_error_code_i( duf_error_index_t e );
duf_error_code_t duf_error_code_rev( const duf_error_event_t * rev );

int duf_errnumber_c( duf_error_code_t rtest );
int duf_errnumber_i( duf_error_code_t rtest );

const char *duf_error_name_p( size_t rp );
const char *duf_error_name_i( duf_error_index_t ri );
const char *duf_error_name_c( duf_error_code_t rc );
const char *duf_error_name_rev( const duf_error_event_t * rev );


int duf_error_line_p( size_t rp );
int duf_error_line_i( duf_error_index_t ri );
int duf_error_line_rev( const duf_error_event_t * rev );

const char *duf_error_func_p( size_t rp );
const char *duf_error_func_i( duf_error_index_t ri );
const char *duf_error_func_rev( const duf_error_event_t * rev );

const char *duf_error_message_p( size_t rp );
const char *duf_error_message_i( duf_error_index_t ri );
const char *duf_error_message_rev( const duf_error_event_t * rev );

int duf_icount_reported_i( duf_error_index_t ri );
int duf_icount_reported_rev( const duf_error_event_t * rev );

#endif
