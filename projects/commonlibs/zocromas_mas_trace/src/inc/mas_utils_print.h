#ifndef MAS_MAST_UTILS_PRINT_H
#  define MAS_MAST_UTILS_PRINT_H

#  include <stdio.h>            /* FILE */
#  include "mas_print_defs.h"

const char *mas_color_s( int is_atty, const char *s );
const char *mas_fcoloro_s( FILE * out, int force_color, int nocolor, const char *s );
const char *mas_fcolor_s( FILE * out, const char *s );

int mas_vsncolor_s( int is_atty, char *buf, size_t size, const char *fmt, va_list args );
int mas_sncolor_s( int is_atty, char *buf, size_t size, const char *fmt, ... );
int mas_vprint_coloro_s( FILE * out, int force_color, int nocolor, const char *fmt, va_list args );
int mas_print_coloro_s( FILE * out, int force_color, int nocolor, const char *fmt, ... );
int mas_vprint_color_s( FILE * out, const char *fmt, va_list args );
int mas_print_color_s( FILE * out, const char *fmt, ... );



int mas_vprintfo( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, int force_color, int nocolor,
                 const char *fmt, va_list args );
int mas_printfo( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, int force_color, int nocolor,
                const char *fmt, ... ) __attribute__ ( ( format( __printf__, 10, 11 ) ) );

int mas_vprintf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out,
                 const char *fmt, va_list args );
int mas_printf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out,
                const char *fmt, ... ) __attribute__ ( ( format( __printf__, 8, 9 ) ) );

int mas_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );
int mas_writes( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );



#endif
