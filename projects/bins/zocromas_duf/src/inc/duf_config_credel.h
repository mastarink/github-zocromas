#ifndef MAS_DUF_CONFIG_CREDEL_H
# define MAS_DUF_CONFIG_CREDEL_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

void duf_config_create( int argc, char **argv, unsigned mandatory_config );
void duf_config_delete( void );

#endif
