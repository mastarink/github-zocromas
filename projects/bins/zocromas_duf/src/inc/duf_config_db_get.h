#ifndef MAS_DUF_CONFIG_DB_GET_H
# define MAS_DUF_CONFIG_DB_GET_H

# include "duf_config_types.h"                                       /* duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

const char *duf_get_config_db_main_fpath( void );
const char *duf_get_config_db_adm_fpath( void );
const char *duf_get_config_db_tempo_fpath( void );
const char *duf_get_config_db_main_name( void );
const char *duf_get_config_db_adm_name( void );
const char *duf_get_config_db_tempo_name( void );
const char *duf_get_config_db_opened_name( void );
const char *duf_get_config_db_dir( void );
const char *duf_get_config_db_subdir( void );

const char *duf_get_config_db_path( void );

#endif
