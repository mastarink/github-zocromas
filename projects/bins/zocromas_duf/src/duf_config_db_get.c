#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

/* ###################################################################### */
#include "duf_config_db_get.h"
/* ###################################################################### */

const char *
duf_get_config_db_main_fpath( void )
{
  return DUF_CONFIGGS( db.main.fpath );
}

const char *
duf_get_config_db_adm_fpath( void )
{
  return DUF_CONFIGGS( db.adm.fpath );
}

const char *
duf_get_config_db_tempo_fpath( void )
{
  return DUF_CONFIGGS( db.tempo.fpath );
}

const char *
duf_get_config_db_main_name( void )
{
  return DUF_CONFIGGSP( db.main.name_x );
}

const char *
duf_get_config_db_adm_name( void )
{
  return DUF_CONFIGGSP( db.adm.name_x );
}

const char *
duf_get_config_db_tempo_name( void )
{
  return DUF_CONFIGGSP( db.tempo.name_x );
}

const char *
duf_get_config_db_dir( void )
{
  return DUF_CONFIGGSP( db.dir_x );
}

const char *
duf_get_config_db_subdir( void )
{
  return DUF_CONFIGGSP( db.subdir_x );
}

const char *
duf_get_config_db_path( void )
{
  return DUF_CONFIGGS( db.path );
}
