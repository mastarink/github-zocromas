#include <unistd.h>

typedef struct
{
  unsigned nonew:1;
  unsigned duftrace;
} tmp_trace_t;
typedef struct
{
  int aaa;
  tmp_trace_t trace;
} tmp_cfg_t;
tmp_cfg_t tmpcfg = {.trace.duftrace = 5 };

tmp_cfg_t *ptmpcfg = &tmpcfg;

#define DUF_FL __func__,__LINE__


#define DUF_TRACE_AT_CFG  trace
#define DUF_DBG_AT_CFG  dbg
#define DUF_TRACE_LOADTIME 0
/* #define DUF_TRACE_FUNWIDTH(_cfg) _cfg->opt.output.fun_width */
#define DUF_TRACE_FUNWIDTH( _cfg ) 18
#define DUF_TRACE_LOADTIME_C( _cfg ) 0
#define DUF_TRACE_FILE_C( _cfg ) stderr
#define DUF_TRACE_CONFIG ptmpcfg

#include "duf_trace_defs.h"
#include "duf_trace.h"

int
duf_output_level( void )
{
  return 0;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  DUF_TRACE( duftrace, 0, "hello %d", 45 );
  sleep( 1 );
  DUF_TRACE( duftrace, 0, "bye %d %d", 8675, 23 );
  return 0;
}
