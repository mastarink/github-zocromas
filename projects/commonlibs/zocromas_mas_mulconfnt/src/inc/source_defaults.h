#ifndef MULCONF_SOURCE_DEFAULTS_H
# define MULCONF_SOURCE_DEFAULTS_H

# include "mulconfnt_types.h"

size_t mucs_source_defaults_count( void );
const config_source_desc_t *mucs_source_defaults( void );
const config_source_desc_t *mucs_source_default( size_t n );

#endif
