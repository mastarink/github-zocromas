#ifndef MAS_ZOCROMAS_REGERR_H
# define MAS_ZOCROMAS_REGERR_H

# include "masregerr_types.h"

void masregerrs_set_max_print( int num );

void masregerr_reg( masregerrs_t * regerrs, const char *func, int line, const char *file, const char *func1, const char *func2, const char *package,
                    int *perrno, int sys, const char *msg, ... );
void masregerr_print_simple( const masregerr_t * rge, const char *msg );
void masregerr_print_simple_all_back_default( masregerrs_t * regerrs, const char *msg );
void masregerr_print_simple_all_default( masregerrs_t * regerrs, const char *msg, int max_print );
void masregerr_print_simple_last_default( masregerrs_t * regerrs, const char *msg );
const char *masregerr_last_msg_default( masregerrs_t * regerrs );

unsigned masregerrs_count_all_default( masregerrs_t * regerrs, int count_hidden );
void masregerrs_delete_default( masregerrs_t * regerrs );

#endif
