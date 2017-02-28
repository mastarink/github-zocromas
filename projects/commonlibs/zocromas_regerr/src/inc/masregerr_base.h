#ifndef MASREGERR_BASE_H
# define MASREGERR_BASE_H

# include "masregerr_types.h"

masregerrs_t *masregerrs_create( void);
void masregerrs_init( masregerrs_t * regerrs, size_t size );
masregerrs_t *masregerrs_create_setup( size_t size );
void masregerrs_reset( masregerrs_t * regerrs );
void masregerrs_delete( masregerrs_t * regerrs );

void masregerr_reset( masregerr_t * rge );

#endif
