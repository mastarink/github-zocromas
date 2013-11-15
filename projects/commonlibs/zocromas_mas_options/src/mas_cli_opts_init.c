#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/control/mas_control.h>
#include <mastar/types/mas_control_types.h>


#include "mas_cli_opts_data.h"

#include "mas_cli_opts_def.h"

#include "mas_cli_opts_init.h"


char *
mas_cli_enabled_options( mas_options_t * popts )
{
  /* struct option       */
  /* {                   */
  /*   const char *name; */
  /*   int has_arg;      */
  /*   int *flag;        */
  /*   int val;          */
  /* };                  */
  char *s = NULL;

  /* for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ ) */
  if ( popts && popts->cli_longopts )
    for ( int io = 0; io < popts->cli_longopts_num; io++ )
    {
      char opt[10];

      if ( popts->cli_longopts[io].val < MAS_CLI_OPT_PLUS )
      {
        opt[0] = ( char ) popts->cli_longopts[io].val;
        opt[1] = 0;
        opt[2] = 0;
        if ( popts->cli_longopts[io].has_arg == required_argument /* || popts->cli_longopts[io].has_arg == optional_argument */  )
          opt[1] = ':';
        s = mas_strcat_x( s, opt );
      }
    }
  return s;
}

static long
mas_cli_optval( const char *arg, long def, int *pr )
{
  long val = 0;

  if ( arg && *arg )
  {
    val = strtol( arg, NULL, 10 );
    if ( ( errno == ERANGE && ( val == LONG_MAX || val == LONG_MIN ) ) || ( errno != 0 && val == 0 ) )
    {
      val = 0;
      *pr = -1;
    }
    else
      *pr = 0;
  }
  else if ( pr )
  {
    val = def;
    *pr = 1;
  }

  return val;
}

static int
mas_cli_make_option( mas_options_t * popts, int opt, const char *arg, mas_optionx_t * optx, int indx, const char *m_optarg )
{
  EVAL_PREPARE;
  int r = 0;
  int v = 0;
  unsigned long def = 0;
  mas_optionx_type_t optyp = OPTX_TYPE_NONE;
  unsigned shift = 0;

  optyp = indx >= 0 ? popts->cli_optxs[indx].optx_type : -1;
  shift = popts->cli_optxs[indx].shift;
  def = popts->cli_optxs[indx].def;
  /* mas_optionx_t *optx = NULL; */
  /* optx = mas_cli_find_optx( opt ); */
  /* HMSG( "CLI M/O %d [%c] optx:%d", opt, opt > ' ' && opt <= 'z' ? opt : '-', optx ? 1 : 0 ); */
  if ( shift )
  {
    char *p = ( ( char * ) popts ) + shift;
    long ml_optarg = mas_cli_optval( m_optarg, def, &v );

    /* ( m_optarg && *m_optarg ? strtol( m_optarg, NULL, 10 ) : def ); */

    switch ( optyp )
    {
    case OPTX_TYPE_FLAG:
      popts->flag.bits |= 1UL << ( shift - 1 );
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_NOFLAG:
      popts->flag.bits &= ~( 1UL << ( shift - 1 ) );
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_UNSIGNED:
      *( ( unsigned * ) p ) = ( unsigned ) ml_optarg;
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_INT:
      *( ( int * ) p ) = ( int ) ml_optarg;
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_STR:
      if ( *( ( char ** ) p ) )
        mas_free( *( ( char ** ) p ) );
      *( ( char ** ) p ) = NULL;
      if ( m_optarg )
        *( ( char ** ) p ) = mas_strdup( m_optarg );
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_ARGV:
      ( ( mas_string_setv_t * ) p )->c = mas_add_argv_arg( ( ( mas_string_setv_t * ) p )->c, &( ( mas_string_setv_t * ) p )->v, m_optarg );
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_ARGV_CLEAR:
      ( ( mas_string_setv_t * ) p )->c = mas_del_argv( ( ( mas_string_setv_t * ) p )->c, ( ( mas_string_setv_t * ) p )->v, 0 );
      ( ( mas_string_setv_t * ) p )->v = NULL;
      popts->cli_optxs[indx].set = 1;
      break;
    case OPTX_TYPE_NONE:
      break;
    }
  }
  else if ( 1 )
  {
    switch ( opt )
    {
    case MAS_CLI_OPT_MSG:
      if ( 0 == strcmp( "mem", m_optarg ) )
      {
        /* popts->msg_flag.bit.msg_mem = 1; */
        OPT_SFLAG( popts, msg.name.msg_mem, 1 );
      }
      break;
    default:                   /* '?' ; ':' */
      {
        HMSG( ">>>>>>>>>>>>>>>>WOW %d : %d <<<<<<<<<<<<<<<<<<<", optyp, shift );
        CTRL_PREPARE;
        /* fprintf( ctrl.stderrfile, "Usage: %s [-t nsecs] [-n] name\n", oargv[0] ); */

        /* ctrl.in_client = 0; */
        ctrl.fatal = 1;
        ctrl.keep_listening = 0;
        HMSG( "O:CLI unknown opt:%d [%c] '%s' optind:%d optx:%d indx:%d [%d]", opt, opt > ' '
              && opt <= 'z' ? opt : '-', arg, optind, optx ? 1 : 0, indx, popts->cli_longopts_num );
        IEVAL( r, -88 );
      }
      break;
    }
  }
  if ( v < 0 )
  {
    HMSG( "O:CLI wrong value '%s'", m_optarg );
    IEVAL( r, -1 );
  }
  /* HMSG( "O:getopt_long:%d Usage: %s [--help -h]", oargv[0] ); */
  return r;
}

/* returns number of firdt non-opt arg */
static int
_mas_cli_short_optx( mas_options_t * popts, int opt )
{
  int indx = -1;

  if ( popts )
    for ( int i = 0; i < popts->cli_longopts_num; i++ )
    {
      if ( !popts->cli_optxs[i].longopt.name )
        break;
      if ( popts->cli_optxs[i].longopt.val < MAS_CLI_OPT_PLUS && opt != '?' && opt == popts->cli_optxs[i].longopt.val )
      {
        indx = i;
        break;
      }
    }
  return indx;
}

static int
_mas_cli_options( mas_options_t * popts, int oargc, char *const *oargv )
{
  CTRL_PREPARE;
  int r = 0;
  int opt = -2;
  int indx = -1;
  int afterlast = 0;
  char *enabled_opts /* __attribute__ ( ( unused ) ) */  = NULL;

  optind = 0;
  optarg = NULL;
  opterr = 0;
  optopt = 0;
  indx = -1;
  enabled_opts = mas_cli_enabled_options( popts );
  HMSG( "CLI %d", ( popts && popts->cli_longopts ) ? 1 : 0 );
  /* extern struct option mas_cli_longopts_ctable[]; */

  if ( popts && popts->cli_longopts )
    while ( r >= 0 && !ctrl.fatal && ( indx = -1 )
            && ( opt = getopt_long( oargc, oargv, enabled_opts, /* mas_cli_longopts_ctable */ popts->cli_longopts, &indx ) ) >= 0 )
    {
      HMSG( "CLI opt:%d:'%s' optind:%d err:%d / %d indx:%d; --%s --%s T(%d)", opt, oargv[optind - 1], optind, opt == '?', opt == ':', indx,
            indx >= 0 ? popts->cli_longopts[indx].name : "?", indx >= 0 ? popts->cli_optxs[indx].longopt.name : "?",
            indx >= 0 ? popts->cli_optxs[indx].optx_type : -1 );
      if ( indx < 0 )
        indx = _mas_cli_short_optx( popts, opt );
      IEVAL( r, mas_cli_make_option( popts, opt, oargv[optind - 1], indx >= 0 ? &popts->cli_optxs[indx] : NULL, indx, optarg ) );
      afterlast = optind;
      /* HMSG( "(%d) CLI %d: %d", r, opt, optind ); */
    }
  MAS_LOG( "(%d) cli options made", r );
  HMSG( "CLI r=%d; opt=%d '%s' opterr=%d ctrl.fatal=%d", r, opt, oargv[optind - 1], opterr, ctrl.fatal );
  mas_free( enabled_opts );
  return r < 0 ? r : afterlast;
}

/* int                                                                                  */
/* _mas_cli_options_init( mas_options_t * popts, const char **message, unsigned flags ) */
INIT_HANDLER( _mas_cli_options_init )
{
  int r = -1;
  int oargc;
  char *const *oargv;

  if ( popts )
  {
    CTRL_PREPARE;
    oargc = popts->argvv.c;
    oargv = popts->argvv.v;
    r = _mas_cli_options( popts, oargc, oargv );
    ctrl.argv_nonoptind = r;
    if ( popts && OPT_QFLAG( popts, help ) )
    {
      for ( int io = 0; io < popts->cli_longopts_num; io++ )
      {
        /* HMSG( "O:%s", cli_longopts[io].name ); */
        HMSG( "O:%s", popts->cli_longopts[io].name );
      }
    }
  }
  if ( message )
    *message = __func__;

  HMSG( "(%d) INIT CLI", r );
  return r;
  /* return r >= 0 ? 0 : r; */
}

/* int                                                                                 */
/* mas_cli_options_init( mas_options_t * popts, const char **message, unsigned flags ) */
INIT_HANDLER( mas_cli_options_init )
{
  int r;

  r = _mas_cli_options_init( popts, message, flags );
  return r >= 0 ? 0 : r;
}

int
mas_cli_options_argv_init( mas_options_t * popts, int oargc, char **oargv, char **env )
{
  CTRL_PREPARE;
  ctrl.launchervv.v = oargv;
  ctrl.launchervv.c = oargc;
  ctrl.launcherev.v = env;
  /* if ( oargc > 1 ) */
  {
    for ( int ia = 0; ia < oargc; ia++ )
    {
      popts->argvv.c = mas_add_argv_arg( popts->argvv.c, &popts->argvv.v, oargv[ia] );
    }
    /* for ( int ia = 0; ia < oargc; ia++ )                              */
    /* {                                                                */
    /*   mMSG( "@: %d of %d. arg:'%s'", ia, popts-> argvv.c, popts-> argvv.v[ia] ); */
    /* }                                                                */
  }
  HMSG( "INIT ARGV argc:%d", popts->argvv.c );
  return popts->argvv.c;
}

void
mas_cli_options_argv_destroy( mas_options_t * popts )
{
  mas_del_argv( popts->argvv.c, popts->argvv.v, 0 );
  popts->argvv.c = 0;
  popts->argvv.v = NULL;
}

void
mas_cli_options_destroy( mas_options_t * popts )
{
  mas_cli_options_argv_destroy( popts );
}
