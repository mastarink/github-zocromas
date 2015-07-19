#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_status_types.h"
#include "duf_ufilter.h"

/* ###################################################################### */
/* #include "duf_status.h" */
/* ###################################################################### */

duf_status_t global_status;

duf_tmp_t *
duf_tmp_create( void )
{
  duf_tmp_t *tmp = NULL;

  tmp = mas_malloc( sizeof( duf_tmp_t ) );
  memset( tmp, 0, sizeof( duf_tmp_t ) );
  return tmp;
}

void
duf_tmp_delete( duf_tmp_t * tmp )
{
  if ( tmp )
  {
    for ( int i = 0; i < DUF_TMP_PATH_MAX; i++ )
    {
      mas_free( tmp->path[i] );
      tmp->path[i] = NULL;
    }
    for ( int i = 0; i < DUF_TMP_EXPLANATION_MAX; i++ )
    {
      mas_free( tmp->option_explanation[i] );
      tmp->option_explanation[i] = NULL;
    }
    mas_free( tmp );
  }
}

void
global_status_init( void )
{
  global_status.tmp = duf_tmp_create(  );
}

void
global_status_reset( void )
{
  duf_tmp_delete( global_status.tmp );
  global_status.tmp = NULL;
  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = NULL;
}

__attribute__ ( ( constructor( 102 ) ) )
     static void constructor_global_status( void )
{
  global_status_init(  );
}

__attribute__ ( ( destructor ) )
     static void destructor_global_status( void )
{
  global_status_reset(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stdout );
#endif
}
