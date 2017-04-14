#ifndef MAS_DUFNX_MYSQL_H
# define MAS_DUFNX_MYSQL_H

# include "types.h"

void dufnx_config_mysql( mucs_option_interface_t * interface, mas_dufnx_data_t * pdufnx_data );
mas_qstd_t *dufnx_qstd( mas_dufnx_mysql_data_t * mysql );

#endif
