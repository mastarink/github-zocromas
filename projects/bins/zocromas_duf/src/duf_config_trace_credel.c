#include <assert.h>
/* #include <stdarg.h> */
#include <string.h>
#include <sys/time.h>

#include <mastar/trace/mas_trace_credel.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_config4trace_ref.h"
#include "duf_cfg_credel.h"

#include "duf_option_config.h"
#include "duf_option_config_credel.h"
#include "duf_xtended_table.h"

/* #include "duf_expandable.h" */

/* ###################################################################### */
#include "duf_config_trace_credel.h"
/* ###################################################################### */
/* TODO 20160223.183423 move to trace-lib; rename accordingly */

#if 0

mas_config_trace_t *
duf_config_trace_create( void )
{
  duf_config_trace_t *tcfg = NULL;

  tcfg = mas_malloc( sizeof( duf_config_trace_t ) );
  memset( tcfg, 0, sizeof( duf_config_trace_t ) );
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      tcfg->loadtime = /* cfg->loadtime = */ ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  tcfg->num_classes = DUF_TRACE_LEVEL_MAX;
# if 0
  tcfg->class_levels = mas_calloc( DUF_TRACE_LEVEL_MAX, sizeof( tcfg->class_levels[0] ) );
# else
  tcfg->class_levels = mas_malloc( DUF_TRACE_LEVEL_MAX * sizeof( tcfg->class_levels[0] ) );
  memset( tcfg->class_levels, 0, DUF_TRACE_LEVEL_MAX * sizeof( tcfg->class_levels[0] ) );
# endif

# if 0
  tcfg->errors += 2;
/* tcfg->options = 1; */
/* tcfg->fs += 1; */

/* tcfg->options  = 11; */
/* tcfg->options  = 71; */
  tcfg->temp += 2;
# else
  tcfg->class_levels[DUF_TRACE_LEVEL_errors] += 2;
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 1;  */
/* tcfg->class_levels[DUF_TRACE_LEVEL_fs] += 1;      */
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 11; */
/* tcfg->class_levels[DUF_TRACE_LEVEL_options] = 71; */
  tcfg->class_levels[DUF_TRACE_LEVEL_temp] += 2;
# endif
  return tcfg;
}

void
duf_config_trace_delete( duf_config_trace_t * tcfg )
{
  mas_free( tcfg->stream.file );
  tcfg->stream.file = NULL;
  mas_free( tcfg->class_levels );
  tcfg->class_levels = NULL;
  tcfg->num_classes = 0;
  mas_free( tcfg );
}
#endif
