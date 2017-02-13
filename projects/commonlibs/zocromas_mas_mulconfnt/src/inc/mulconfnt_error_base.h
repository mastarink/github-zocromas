#ifndef MULCONF_ERROR_BASE_H
# define MULCONF_ERROR_BASE_H

# include "mulconfnt_types.h"

void mulconfnt_error_close( config_error_t * error );
void mulconfnt_error_delete( config_error_t * error );

int mulconfnt_error_vset( config_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, va_list args );
int mulconfnt_error_set( config_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... );

int mulconfnt_error_set_from_error( config_error_t * error, config_error_t * srcerror );

#endif
