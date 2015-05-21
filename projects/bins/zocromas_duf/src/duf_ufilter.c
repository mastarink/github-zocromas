#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_error_types.h"
#include "duf_debug_defs.h"
#include "duf_trace_defs.h"
#include "duf_trace.h"
#include "duf_errors.h"

#include "duf_dbg.h"
#include "duf_print_defs.h"


#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"



#include "duf_config_ref.h"
/* ###################################################################### */
#include "duf_ufilter.h"
/* ###################################################################### */


duf_ufilter_t *
duf_ufilter_create( void )
{
  duf_ufilter_t *pu = NULL;

  pu = mas_malloc( sizeof( duf_ufilter_t ) );
  memset( pu, 0, sizeof( duf_ufilter_t ) );
  pu->max_rel_depth = 99;
  return pu;
}

void
duf_ufilter_delete( duf_ufilter_t * pu )
{
  if ( pu )
  {
    mas_free( pu->glob );
    pu->glob = NULL;

    mas_free( pu->same_md5 );
    pu->same_md5 = NULL;

    mas_argv_delete( pu->globx.include_files.argc, pu->globx.include_files.argv );
    pu->globx.include_files.argc = 0;
    pu->globx.include_files.argv = NULL;

    mas_argv_delete( pu->globx.exclude_files.argc, pu->globx.exclude_files.argv );
    pu->globx.exclude_files.argc = 0;
    pu->globx.exclude_files.argv = NULL;
    mas_free( pu );
  }
}

void
duf_ufilter_copy( duf_ufilter_t * pu, const duf_ufilter_t * pusrc )
{
  if ( pu && pusrc )
  {
    memcpy( pu, pusrc, sizeof( duf_ufilter_t ) );
    pu->glob = mas_strdup( pusrc->glob );
    pu->same_md5 = mas_strdup( pusrc->same_md5 );

    pu->globx.include_files.argc = 0;
    pu->globx.include_files.argv = NULL;
    pu->globx.include_files.argc = mas_argv_clone( &pu->globx.include_files.argv, pusrc->globx.exclude_files.argc, pusrc->globx.exclude_files.argv );

    pu->globx.exclude_files.argc = 0;
    pu->globx.exclude_files.argv = NULL;
    pu->globx.include_files.argc = mas_argv_clone( &pu->globx.include_files.argv, pusrc->globx.exclude_files.argc, pusrc->globx.exclude_files.argv );
  }
}

duf_ufilter_t *
duf_ufilter_clone( const duf_ufilter_t * pusrc )
{
  duf_ufilter_t *pu = NULL;

  if ( pusrc )
  {
    pu = duf_ufilter_create(  );
    duf_ufilter_copy( pu, pusrc );
  }
  return pu;
}

duf_ufilter_t *
duf_ufilter_create_from( const duf_ufilter_t * pusrc )
{
  duf_ufilter_t *pu = NULL;

  pu = duf_ufilter_create(  );
  if ( pusrc )
    duf_ufilter_copy( pu, pusrc );
  return pu;
}
