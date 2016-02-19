#ifndef MAS_DUF_MAINTENANCE_H
#  define MAS_DUF_MAINTENANCE_H
#  define DUF_SQL_PDI_STMT

#  include "duf_maintenance_z.h"

#  include "duf_defs.h"

extern int duf_verbose( void ) __attribute__ ( ( weak ) );
extern int duf_dry_run( void ) __attribute__ ( ( weak ) );


#  include "duf_printn_defs_preset.h"
#  include "duf_tracen_maintenance.h"

#  include "duf_compile_defs.h"
#  include "duf_errorn_defs.h"
/* #  include "duf_printn_defs.h" */

#  include <mastar/error/mas_errors_maintenance.h>
/* #  include <mastar/error/mas_error_reporting.h> */



/* #  include "duf_start_end.h" */
#  include "duf_se.h"

#  include "duf_dodefs.h"

#  include "duf_debug_defs.h"
#  include "duf_dbg.h"


#  include "duf_config_util_z.h"

#endif

/*
vi: ft=c
*/
