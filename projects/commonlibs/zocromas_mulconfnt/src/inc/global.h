#ifndef MUCS_GLOBAL_H
# define MUCS_GLOBAL_H

# include "mulconfnt_types.h"

mucs_flags_t mucs_global_flags( void );

mucs_flags_t mucs_global_flag( mucs_flags_t flag );
void mucs_set_global_flag( mucs_flags_t flag );
void mucs_set_global_flags( mucs_flags_t flags );

#endif
