#include <unistd.h>
#include "mas_trace_credel.h"

typedef struct
{
/* unsigned nonew:1; */
  int duftrace;
} tmp_trace_t;

/* typedef struct                             */
/* {                                          */
/*   int aaa;                                 */
/*   tmp_trace_t trace;                       */
/* } tmp_cfg_t;                               */
/* tmp_cfg_t tmpcfg = {.trace.duftrace = 5 }; */
/*                                            */
/* tmp_cfg_t *ptmpcfg = &tmpcfg;              */

#define MAST_FL __func__,__LINE__

#define MAST_TRACE_AT_CFG_CR(_cfg)  &(_cfg.trace)
#define MAST_TRACE_AT_CFG_  trace.
/* #define MAST_DBG_AT_CFG  dbg */
#define MAST_TRACE_LOADTIME 0
/* #define MAST_TRACE_FUNWIDTH(_cfg) _cfg->opt.output.fun_width */
#define MAST_TRACE_FUNWIDTH_C( _cfg ) 18
#define MAST_TRACE_FORCE_COLOR_C( _cfg ) 0
#define MAST_TRACE_NOCOLOR_C( _cfg ) 0
#define MAST_TRACE_LOADTIME_C( _cfg ) 0
#define MAST_TRACE_FILE_C( _cfg ) stderr
#define MAST_TRACE_CONFIG ptcfg
#define MAST_TRACE_LEVEL_C( _cfg, _what, _name ) 0

#include "mas_trace_defs.h"
#include "mas_trace.h"

int
mas_output_level( void )
{
  return 0;
}

mas_config_trace_t *ptcfg = NULL;

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  ptcfg = mas_config_trace_create( sizeof( tmp_trace_t ) / sizeof( int ) );
  MAST_TRACE( duftrace, 0, "hello %d", 45 );
  sleep( 1 );
  MAST_TRACE( duftrace, 0, "bye %d %d", 8675, 23 );
  mas_config_trace_delete(ptcfg);
  return 0;
}
