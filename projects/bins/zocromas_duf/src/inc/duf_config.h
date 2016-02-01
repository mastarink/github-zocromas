#ifndef MAS_DUF_CONFIG_H
#  define MAS_DUF_CONFIG_H

#  include "duf_config_types.h"

duf_config_t *duf_cfg_create( void );
void duf_cfg_delete( duf_config_t * cfg );

void duf_config_create( int argc, char **argv );
void duf_config_delete( void );

int duf_config_show( void ) __attribute__ ( ( warn_unused_result ) );
int duf_config_optionally_show( void ) __attribute__ ( ( warn_unused_result ) );

int duf_config_make_db_main_path( void );
int duf_config_make_db_adm_path( void );
int duf_config_make_db_temp_path( void );
int duf_config_make_db_paths( void );

#endif
