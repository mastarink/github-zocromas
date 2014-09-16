#ifndef MAS_DUF_ERRORS_H
#  define MAS_DUF_ERRORS_H

#  include "duf_error_types.h"

duf_error_code_t duf_vclear_error( duf_error_code_t r, va_list args );
duf_error_code_t duf_clear_error( duf_error_code_t r, ... );

int duf_errindex( duf_error_code_t rtest );
const char *duf_error_name( duf_error_code_t c );

void duf_set_mereport( int once, int doreport, ... );
void duf_vset_ereport( int once, int doreport, va_list args );
void duf_set_ereport( int once, int enable, duf_error_code_t rtest );
int duf_get_ereport( duf_error_code_t rtest );

int duf_ecount( duf_error_code_t rtest );


void duf_set_emax_count( int maxcount, duf_error_code_t rtest );
void duf_vset_emax_count( int maxcount, va_list args );
void duf_set_memax_count( int maxcount, ... );

#endif
