#include <assert.h>
#include <string.h>
#include <dlfcn.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_mod_handle.h"

/* ###################################################################### */
#include "duf_mod_handle.h"
/* ###################################################################### */

void *
duf_load_symbol( const char *libname, const char *symbol )
{
  void *psym = NULL;
  void *han = NULL;
  char *path = NULL;

  path = mas_normalize_path_plus( MAS_LIBDIR, "dufmod", NULL );
  path = mas_strcat_x( path, libname );
  path = mas_strcat_x( path, ".so" );

  han = dlopen( path, RTLD_NOLOAD | RTLD_LAZY );
  if ( !han )
    han = dlopen( path, RTLD_LOCAL | RTLD_LAZY );
  if ( han )
    psym = ( void * ) dlsym( han, symbol );
  MAST_TRACE( sccb, 0, "[han:%p] %s : %p", han, symbol, psym );
  mas_free( path );
  return psym;
}

const duf_mod_handler_t *
duf_load_mod_handler_symbol( const char *libname )
{
  const duf_mod_handler_t *mhan = NULL;

  MAST_TRACE( mod, 0, "@@@@@@to load lib: %s", libname );
  mhan = ( const duf_mod_handler_t * ) duf_load_symbol( libname, "duf_mod_handler_uni" );
  MAST_TRACE( mod, 0, "%s", mhan ? mhan->name : NULL );
  return mhan;
}

void *
duf_load_mod_handler_symbol_find( const char *libname, const char *masmodsymbol )
{
  void *ptr = NULL;

  for ( const duf_mod_handler_t * mhan = duf_load_mod_handler_symbol( libname ); !ptr && mhan && mhan->name; mhan++ )
  {
    if ( 0 == strcmp( mhan->name, masmodsymbol ) )
      ptr = mhan->handler;
  }
  /* QT( "@%s::%s ~ %p", libname, masmodsymbol, ptr ); */
  return ptr;
}
