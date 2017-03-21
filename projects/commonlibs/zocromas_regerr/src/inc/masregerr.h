#ifndef MAS_ZOCROMAS_REGERR_H
# define MAS_ZOCROMAS_REGERR_H

# include "masregerr_types.h"

void masregerrs_set_max_print( int num );

int masregerr_reg( masregerrs_t * regerrs, const char *func, int line, const char *file, const char *func1, const char *func2, const char *package,
                   int *perrno, int sys, const char *msg );
int masregerr_print_simple( const masregerr_t * rge, const char *msg );
int masregerr_print_simple_all_back( masregerrs_t * regerrs, const char *msg );
int masregerr_print_simple_all( masregerrs_t * regerrs, const char *msg, int max_print );
int masregerr_print_simple_last( masregerrs_t * regerrs, const char *msg );

size_t masregerrs_count_all( masregerrs_t * regerrs );

#endif
