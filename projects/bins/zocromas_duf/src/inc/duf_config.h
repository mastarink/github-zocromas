#ifndef MAS_DUF_CONFIG_H
#  define MAS_DUF_CONFIG_H

#  include "duf_types.h"
extern duf_config_t *duf_config;

int duf_config_create( void );
int duf_config_delete( void );

int duf_config_show( void );

#endif
