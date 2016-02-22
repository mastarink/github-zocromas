/* #include <stdarg.h> */
#include <string.h>
#include <sys/time.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_config4trace_ref.h"
#include "duf_cfg_credel.h"

#include "duf_option_config.h"
#include "duf_option_config_credel.h"
#include "duf_xtended_table.h"

#include "duf_expandable.h"

/* ###################################################################### */
#include "duf_config_trace_credel.h"
/* ###################################################################### */

duf_config_trace_t *
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
  tcfg->errors += 2;
/* tcfg->options = 1; */
/* tcfg->fs += 1; */

/* tcfg->options  = 11; */
/* tcfg->options  = 71; */
  tcfg->temp += 2;
  return tcfg;
}

void
duf_config_trace_delete( duf_config_trace_t * tcfg )
{
  mas_free( tcfg->output.file );
  tcfg->output.file = NULL;
  mas_free( tcfg );
}
