#include <assert.h>
#include <signal.h>

/* #include <mastar/trace/mas_trace.h> */
/* #include "mas_tracen_maintenance.h" */

#include "mas_error_tracen_defs_preset.h"

/* #include <mastar/trace/mas_print_defs.h>  */
/* #include <mastar/trace/mas_utils_print.h> */
/* #include <mastar/trace/mas_trace_defs.h>  */
#include <mastar/trace/mas_trace.h>
#include <mastar/trace/mas_trace_utils_print.h>

#include "mas_error_defs_show.h"
#include "mas_error_base.h"
#include "mas_error_names.h"

#include "mas_error_regfind.h"
/* ###################################################################### */
#include "mas_error_reporting.h"
/* ###################################################################### */

/* #define MAS_NOTIMING */
static int report_level[DUF_ERROR_COUNT] = { 0 };
static int count_reported[DUF_ERROR_COUNT] = { 0 };
static int max_count_reported[DUF_ERROR_COUNT] = { 0 };

/* static int force_count_ereport = 0; */
/* static int force_fixed_ereport = 0; */
static int force_offset_ereport = 0;

/* void                                 */
/* mas_force_count_ereport( int count ) */
/* {                                    */
/*   force_count_ereport = count;       */
/* }                                    */

/* void                                 */
/* mas_force_fixed_ereport( int count ) */
/* {                                    */
/*   force_fixed_ereport = count;       */
/* }                                    */

void
mas_force_offset_ereport( int count )
{
  force_offset_ereport = count;
}

static void
_mas_set_ereport_level_c( int once __attribute__ ( ( unused ) ), int enable, int nabs, mas_error_code_t rc, int maxerr )
{
  assert( ( rc >= DUF_SQL_ERROR && rc <= DUF_SQL_WARNING_AUTOINDEX ) || ( rc >= DUF_ERROR_ERROR_BASE && rc <= DUF_ERROR_ERROR_MAX ) );
  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    assert( errnumber >= 0 );
    if ( errnumber >= 0 && errnumber < maxerr )
    {
      if ( nabs )
        report_level[errnumber] = enable /* -1 TODO ?? */ ;
      else
        report_level[errnumber] += enable /* < 0 ? -1 : 1 */ ;
    }
  }
}

int
_mas_ereport_level_c( mas_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    assert( errnumber >= 0 );
    if ( errnumber >= 0 && errnumber < maxerr )
    {
      re = report_level[errnumber] + 1;
    }
  }
  return re;
}

void
mas_set_ereport_level_c( int once, int enable, int nabs, mas_error_code_t rc )
{
/////  MAST_TRACE( errors, 5, "set report for error number %d / %d (%s)", rc, rc - MASE_ERROR_ERROR_BASE, mas_error_name_c( rc ) );
  _mas_set_ereport_level_c( once, enable, nabs, rc, DUF_ERROR_COUNT );
}

int
mas_ereport_level_c( mas_error_code_t rc )
{
  return _mas_ereport_level_c( rc, DUF_ERROR_COUNT );
}

void
mas_vmset_ereport_level_c( int once, int enable, int nabs, va_list args )
{
  mas_error_code_t rc = 0;

  do
  {
    rc = va_arg( args, int );

    if ( rc )
      mas_set_ereport_level_c( once, enable, nabs, rc );
  }
  while ( rc );
}

void
mas_mset_ereport_level_c( int once, int enable, int nabs, ... )
{
  va_list args;

  va_start( args, nabs );
  mas_vmset_ereport_level_c( once, enable, nabs, args );
  va_end( args );
}

/* >0 -- report it */
static int
_mas_enabled_ereport_level_c( mas_error_code_t rc, int maxerr )
{
  int re = 0;
  int errnumber = mas_errnumber_c( rc );

/* if ( rc < 0 ) */
/*   return 1;      */
  if ( rc < 0 )
  {
    assert( errnumber >= 0 );
    if ( errnumber < 0 )
    {
      re = 0;                                                        /* sql ? ? ? */
    }
#if 0
    else if ( errnumber >= 0 && errnumber < maxerr
              && ( max_count_reported[errnumber] <= 0 || count_reported[errnumber] < max_count_reported[errnumber] - 1 ) )
      re = report_level[errnumber];
#else
    else if ( mas_emax_count_c( rc ) <= 0 || count_reported[errnumber] < mas_emax_count_c( rc ) )
      re = _mas_ereport_level_c( rc, maxerr );
#endif
  }
  return re;
}

int
mas_enabled_ereport_level_c( mas_error_code_t rc )
{
  return _mas_enabled_ereport_level_c( rc, DUF_ERROR_COUNT );
}

static int
_mas_enabled_ereport_level_i( mas_error_index_t ri, int maxerr )
{
  return _mas_enabled_ereport_level_c( mas_error_code_i( ri ), maxerr );
}

/* int                                                       */
/* mas_ereport_level_n_c( mas_error_code_t rc )              */
/* {                                                         */
/*   int re = 0;                                             */
/*                                                           */
/*   re = _mas_ereport_level_c( rc, DUF_ERROR_COUNT );       */
/*   return re;                                              */
/* }                                                         */
/*                                                           */
/* int                                                       */
/* mas_ereport_level_n_i( mas_error_index_t ri )             */
/* {                                                         */
/*   return mas_ereport_level_n_c( mas_error_code_i( ri ) ); */
/* }                                                         */

int
mas_enabled_ereport_level_i( mas_error_index_t ri )
{
  int re = 0;

  if ( ri < 0 )
  {
    int en;

    en = _mas_enabled_ereport_level_i( ri, DUF_ERROR_COUNT );
    /* if ( force_count_ereport > 0 )  */
    /*   re = force_count_ereport--;   */
    /* else if ( force_fixed_ereport ) */
    /*   re = force_fixed_ereport;     */
    /* else                            */
      re = en;
    re += force_offset_ereport;
  }
  return re;
}

static void
_mas_set_emax_count_c( int maxcount, mas_error_code_t rc, int maxerr )
{
  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      max_count_reported[errnumber] = maxcount + 1;
  }
}

static int
_mas_emax_count_c( mas_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      re = max_count_reported[errnumber] - 1;
  }
  return re;
}

int
mas_emax_count_c( mas_error_code_t rc )
{
  return _mas_emax_count_c( rc, DUF_ERROR_COUNT );
}

void
mas_set_emax_count_c( int maxcount, mas_error_code_t rc )
{
  _mas_set_emax_count_c( maxcount, rc, DUF_ERROR_COUNT );
}

void
mas_vset_emax_count_c( int maxcount, va_list args )
{
  mas_error_code_t rc = 0;

  do
  {
    rc = va_arg( args, int );

    if ( rc )
      mas_set_emax_count_c( maxcount, rc );
  }
  while ( rc );
}

void
mas_set_memax_count_c( int maxcount, ... )
{
  va_list args;

  va_start( args, maxcount );
  mas_vset_emax_count_c( maxcount, args );
  va_end( args );
}

int
_mas_ecount_reported_plus_c( mas_error_code_t rc, int test, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    if ( !test && errnumber >= 0 && errnumber < maxerr )
      re = count_reported[errnumber]++;
  }
  return re;
}

int
mas_ecount_reported_plus_c( mas_error_code_t rc, int test )
{
  int re = 0;

  re = _mas_ecount_reported_plus_c( rc, test, DUF_ERROR_COUNT );
  return re;
}

int
mas_ecount_reported_plus_rev( mas_error_event_t * rev, int test )
{
  if ( rev && !test )
    rev->count_reported++;
  return mas_ecount_reported_plus_c( mas_error_code_rev( rev ), test );
}

/* get err reported counter by error list pointer */
int
mas_ecount_reported_rev( const mas_error_event_t * rev )
{
  int cnt = 0;

  if ( rev )
    cnt = rev->count_reported;
/* msg = global_error_list[-ri - 1].message; */
  return cnt;
}

/* get err reported counter by error unique id ~ index */
int
mas_ecount_reported_i( mas_error_index_t ri )
{
  return mas_ecount_reported_rev( mas_find_error_event_i( ri ) );
}

int
mas_ecount_reported_plus_i( mas_error_index_t ri, int test )
{
/* mas_error_event_t *rev = NULL;      */
/* rev = mas_find_error_event_i( ri ); */
  return mas_ecount_reported_plus_rev( mas_find_error_event_i( ri ), test );
/* if ( rev )                                                 */
/*   rev->count_reported++;                                   */
/* return mas_ecount_reported_plus_c( mas_error_code_rev( rev ) ); */
}

void
mas_error_report_rev( const mas_config_trace_t * tcfg, int trace_errindex, mas_error_event_t * rev, int test, FILE * out, int verb )
{
  if ( rev )
  {
  /* mas_error_event_t *rev = mas_find_error_event_i( ri ); */
    int erep __attribute__ ( ( unused ) ) = mas_ecount_reported_plus_rev( rev, test );
    int irep __attribute__ ( ( unused ) ) = mas_ecount_reported_rev( rev );

    const char *func = mas_error_func_rev( rev );
    int line = mas_error_line_rev( rev );
    mas_error_code_t rc __attribute__ ( ( unused ) ) = rev->code;
    mas_error_index_t ri __attribute__ ( ( unused ) ) = rev->index;
    const char *msg = mas_error_message_rev( rev );
    const char *ename = mas_error_name_rev( rev );
    const char *prefix = test ? "@@ [TEST] " : "@@  ERROR";

    int re;

    re = mas_enabled_ereport_level_c( rc );
    if ( re - test > 0 )
    {
      switch ( verb )
      {
      case 0:
        mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, ".   " );

        mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, ".%s      ", prefix );
        if ( verb )
        {
          mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, "@@@@@@@@" "[%s]%s%s (v%d)", ename, msg ? " - " : "", msg,
                       verb );
        }
        else
        {
          mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, "@@@@@@@@" "[%s]%s%s", ename, msg ? " - " : "", msg );
        }
        break;
      case 1:
        mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, ".   " );
        mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, ".%s      ", prefix );
        mas_printfo( 0, 0 /*noeol */ , 0 /* _min */ , 0, __func__, __LINE__, out, 1, 1, "@@@@@@@@" "[%s]%s%s (%s:%d) (v%d)", ename, msg ? " - " : "",
                     msg, func, line, verb );
        break;
      case 2:
        mas_trace( tcfg, "errors", trace_errindex, 0, func, line, '*' /*signum */ , 0 /*flags */ , 0 /*nerr */ , prefix,
                   "@@@@@@@@" "[%s]%s%s (%s:%d) verb:%d", ename, msg ? " - " : "", msg, func, line, verb );
      /* fprintf( stderr, "*********** %d\n", __LINE__ ); */
        break;
      case 3:
        mas_trace( tcfg, "errors", trace_errindex, 0, func, line, '*' /*signum */ , 0 /*flags */ , 0 /*nerr */ , prefix,
                   "[%s]%s%s (rc:%d) (ri:%d) {en:%d/%d/%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                   ename, msg ? " - " : "", msg, rc, ri, mas_ereport_level_c( rc ), mas_emax_count_c( rc ), mas_enabled_ereport_level_c( rc ),
                   mas_error_list_size(  ), erep, irep, func, line, verb );
      /* fprintf( stderr, "*********** %d\n", __LINE__ ); */
        break;
      case 4:
        mas_trace( tcfg, "errors", trace_errindex, 0, func, line, '*' /*signum */ , 0 /*flags */ , 0 /*nerr */ , prefix,
                   "@@@@@@@@" "[%s]%s%s (rc:%d) (ri:%d) {en:%d/%d/%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                   ename, msg ? " - " : "", msg, rc, ri, mas_ereport_level_c( rc ), mas_emax_count_c( rc ), mas_enabled_ereport_level_c( rc ),
                   mas_error_list_size(  ), erep, irep, func, line, verb );
      /* fprintf( stderr, "*********** %d\n", __LINE__ ); */
        break;
      case 5:
        mas_trace( tcfg, "errors", trace_errindex, 0, func, line, '*' /*signum */ , 0 /*flags */ , 0 /*nerr */ , prefix,
                   "@@@@@@@@" "[%s]%s%s (rc:%d) (ri:%d) {en:%d/%d/%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                   ename, msg ? " - " : "", msg, rc, ri, mas_ereport_level_c( rc ), mas_emax_count_c( rc ), mas_enabled_ereport_level_c( rc ),
                   mas_error_list_size(  ), erep, irep, func, line, verb );
      /* fprintf( stderr, "*********** %d\n", __LINE__ ); */
        break;
      default:
        mas_trace( tcfg, "errors", trace_errindex, 0, func, line, '*' /*signum */ , 0 /*flags */ , 0 /*nerr */ , prefix,
                   "@@@@@@@@" "[%s]%s%s (rc:%d) (ri:%d) {en:%d/%d/%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                   ename, msg ? " - " : "", msg, rc, ri, mas_ereport_level_c( rc ), mas_emax_count_c( rc ), mas_enabled_ereport_level_c( rc ),
                   mas_error_list_size(  ), erep, irep, func, line, verb );
      /* fprintf( stderr, "*********** %d\n", __LINE__ ); */
        break;
      }
    /* raise( SIGABRT );                                              (* To continue from here in GDB: "signal 0". *) */
      /* assert( 0 ); */
    }
    assert( func );
    assert( *func );
    assert( line );
  }
}

void
mas_error_report_i( const mas_config_trace_t * tcfg, int trace_errindex, mas_error_index_t ri, int test, FILE * out, int verb )
{
  if ( ri < 0 )
  {
    mas_error_event_t *rev = mas_find_error_event_i( ri );

    mas_error_report_rev( tcfg, trace_errindex, rev, test, out, verb );
  }
}

void
mas_error_report_p( const mas_config_trace_t * tcfg, int trace_errindex, size_t rp, int test, FILE * out, int verb )
{
  mas_error_event_t *rev = NULL;

/* assert( rp >= 0 ); */
  rev = mas_find_error_event_p( rp );
  if ( rev )
    mas_error_report_rev( tcfg, trace_errindex, rev, test, out, verb );
}

void
mas_error_report_all( const mas_config_trace_t * tcfg, int trace_errindex, int test, FILE * out, int verb )
{
  unsigned k = 0;

  for ( unsigned rp = 0; rp < mas_error_list_size(  ); rp++ )
  {
  /* if ( mas_error_code_p( rp ) != MAS_SQL_DONE && mas_error_code_p( rp ) != MAS_SQL_ROW ) */
    {
    /* T( "@@@@@@%d. %d. %s @ %s:%d %s", rp, k, mas_error_name_p( rp ), mas_error_func_p( rp ), mas_error_line_p( rp ), mas_error_message_p( rp ) ); */
#if 0
      T( "@@@@@@@%d. %s @ %s:%d %s", rp + 1, mas_error_name_p( rp ), mas_error_func_p( rp ), mas_error_line_p( rp ), mas_error_message_p( rp ) );
      MASE_SHOW_ERROR( "@@@@@@@@%d. %s @ %s:%d %s", rp + 1, mas_error_name_p( rp ), mas_error_func_p( rp ), mas_error_line_p( rp ),
                       mas_error_message_p( rp ) );
#else
      mas_error_report_p( tcfg, trace_errindex, rp, test, out, verb );
#endif
      k++;
    }
  }
}
