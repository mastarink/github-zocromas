#ifndef MAS_DUF_CONFIG_H
# define MAS_DUF_CONFIG_H

# include "duf_config_types.h"                                       /* duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t ✗ */
# include "duf_config_opt_types.h"
# include "duf_config_output_types.h"                                /* duf_config_output_t ✗ */

duf_config_t *duf_get_config( void );

const duf_config_opt_t *duf_get_cfg_opt( const duf_config_t * cfg );
const duf_config_output_t *duf_get_cfg_output( const duf_config_t * cfg );


#endif
