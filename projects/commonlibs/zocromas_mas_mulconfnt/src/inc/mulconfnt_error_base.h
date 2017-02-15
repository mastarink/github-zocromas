#ifndef MUCS_ERROR_BASE_H
# define MUCS_ERROR_BASE_H

# include "mulconfnt_types.h"

void mucs_error_close( mucs_error_t * error );
void mucs_error_delete( mucs_error_t * error );

int mucs_error_vset( mucs_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, va_list args );
int mucs_error_set( mucs_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... );

int mucs_error_set_from_error( mucs_error_t * error, mucs_error_t * srcerror );

#endif
