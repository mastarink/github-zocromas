#define R_GOOD(_r) ((_r)>=0)
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>
#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include "structs.h"
#include "mysql.h"

void
dufnx_config_mysql( mucs_option_interface_t * interface, mas_dufnx_data_t * pdufnx_data )
{
  mucs_option_t _uUu_ options_mysql[] = {
    {.name = "mysql-server",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.server,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-user",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.user,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-password",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.password,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-db",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &pdufnx_data->mysql.db,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "mysql-port",.shortn = '\0',.restype = MUCS_RTYP_UINT,.cust_ptr = &pdufnx_data->mysql.port,.flags = MUCS_FLAG_AUTOFREE},
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_config_option_interface_tabnode_add( interface, "mysql-table", options_mysql );
}
