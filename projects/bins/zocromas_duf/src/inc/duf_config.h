#ifndef MAS_DUF_CONFIG_H
#  define MAS_DUF_CONFIG_H

duf_config_t *duf_cfg_create( void );
void duf_cfg_delete( duf_config_t * cfg );

void duf_config_create( void );
void duf_config_delete( void );

int duf_config_show( void );
int duf_config_optionally_show( void );


#endif
