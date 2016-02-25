#ifndef MAS_DUF_CFG_CREDEL_H
# define MAS_DUF_CFG_CREDEL_H

# include "duf_config_types.h"                                       /* duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t ♠ */

duf_config_t *duf_cfg_create( void );
void duf_cfg_delete( duf_config_t * cfg );

#endif
