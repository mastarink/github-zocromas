#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_status_types.h"
#include "duf_ufilter.h"

/* ###################################################################### */
/* #include "duf_status.h" */
/* ###################################################################### */

duf_status_t global_status;

static duf_tmp_t *
duf_tmp_create( void )
{
  duf_tmp_t *tmp = NULL;

  tmp = mas_malloc( sizeof( duf_tmp_t ) );
  memset( tmp, 0, sizeof( duf_tmp_t ) );
  return tmp;
}

static void
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
global_status_register_xcmd( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage, int no,
                             duf_option_source_t source )
{
#define XCMDS_STEP 64
  if ( global_status.n_xcmds == global_status.alloc_xcmds )
  {
    size_t z, n;

    z = global_status.alloc_xcmds;
    global_status.alloc_xcmds += XCMDS_STEP;
    n = global_status.alloc_xcmds * sizeof( duf_xcmd_t );
#if 0
    global_status.xcmds = mas_realloc( global_status.xcmds, n );
#else
    {
      duf_xcmd_t *xc;

      xc = mas_malloc( n );
      if ( global_status.xcmds )
      {
        memcpy( xc, global_status.xcmds, n );
        mas_free( global_status.xcmds );
      }
      global_status.xcmds = xc;
    }
#endif
    memset( global_status.xcmds + z, 0, XCMDS_STEP * sizeof( duf_xcmd_t ) );
  }
  {
    int pos;

    pos = global_status.n_xcmds++;
    global_status.xcmds[pos].extended = extended;
    global_status.xcmds[pos].optargg = mas_strdup( optargg );
    global_status.xcmds[pos].istage = istage;
    global_status.xcmds[pos].no = no;
    global_status.xcmds[pos].source = source;
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
#if 0
  duf_ufilter_delete( global_status.selection_bound_ufilter );
  global_status.selection_bound_ufilter = NULL;
#endif
  if ( global_status.xcmds )
  {
    for ( unsigned pos = 0; pos < global_status.n_xcmds; pos++ )
    {
      mas_free( global_status.xcmds[pos].optargg );
    }
    mas_free( global_status.xcmds );
    global_status.xcmds = NULL;
  }

  mas_free( global_status.db_attached_selected );
  global_status.db_attached_selected = NULL;
  mas_free( global_status.db_opened_name );
  global_status.db_opened_name = NULL;
}

__attribute__ ( ( constructor( 101 ) ) )
     static void constructor_global_status( void )
{
  global_status_init(  );
}

__attribute__ ( ( destructor( 101 ) ) )
     static void destructor_global_status( void )
{
/* #define DUF_MEM_ROW "◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐◑◐" */
#define DUF_MEM_ROW " ▤▤▤▤▤▤▤▤▤▤▤▤▤ "
  global_status_reset(  );
#ifdef MAS_TRACEMEM
  print_memlist_msg( FL, stdout, /* */
                     "\n\x1b[0;1;7;44;35m" DUF_MEM_ROW "<  1.", /* */
                     "\n\x1b[0;1;7;46;37m" DUF_MEM_ROW "<  2.", /* */
                     "\x1b[0;1;7;32m    MEMORY FREE    ", /* */
                     "\x1b[0;1;7;44;35m  >" DUF_MEM_ROW "\x1b[0m", "\n\n\x1b[0;1;33;41m                              MEMORY TABLE                              \x1b[0m" /* */
         );
#endif
}
