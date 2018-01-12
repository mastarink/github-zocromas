#ifndef MUCS_SOURCE_FLAGS_H
# define MUCS_SOURCE_FLAGS_H

# include "mulconfnt_types.h"

mucs_flags_t mucs_source_flags( mucs_source_t * osrc );
int mucs_source_flag( mucs_source_t * osrc, mucs_flags_t mask );

#endif
