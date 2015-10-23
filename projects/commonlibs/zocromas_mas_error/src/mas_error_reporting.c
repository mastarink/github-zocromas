#include <assert.h>

/* #include <mastar/trace/mas_trace.h> */
/* #include "mas_tracen_maintenance.h" */

#include "mas_error_tracen_defs_preset.h"


#include <mastar/trace/mas_print_defs.h>
#include <mastar/trace/mas_utils_print.h>
#include <mastar/trace/mas_trace_defs.h>
#include <mastar/trace/mas_trace.h>

#include "mas_error_defs_show.h"
#include "mas_error_base.h"
#include "mas_error_names.h"

#include "mas_error_regfind.h"
/* ###################################################################### */
#include "mas_error_reporting.h"
/* ###################################################################### */


/* #define MAS_NOTIMING */
static int noreport_error[DUF_ERROR_COUNT] = { 0 };
static int count_reported[DUF_ERROR_COUNT] = { 0 };
static int max_show_count_error[DUF_ERROR_COUNT] = { 0 };

static int force_count_ereport = 0;
static int force_fixed_ereport = 0;
static int force_offset_ereport = 0;


void
mas_force_count_ereport( int count )
{
  force_count_ereport = count;
}

void
mas_force_fixed_ereport( int count )
{
  force_fixed_ereport = count;
}

void
mas_force_offset_ereport( int count )
{
  force_offset_ereport = count;
}

static void
_mas_enable_ereport_c( int once __attribute__ ( ( unused ) ), int enable, int nabs, mas_error_code_t rc, int maxerr )
{
  assert( ( rc >= DUF_SQL_ERROR && rc <= DUF_SQL_WARNING_AUTOINDEX ) || ( rc >= DUF_ERROR_ERROR_BASE && rc <= DUF_ERROR_ERROR_MAX ) );
  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    assert( errnumber >= 0 );
/////    MAST_TRACE( errors, 5, "set report for error number %d / %d (%s)", rc, rc - DUF_ERROR_ERROR_BASE, mas_error_name_c( rc ) );
    if ( errnumber >= 0 && errnumber < maxerr )
    {
      /* int b;                        */
      /*                               */
      /* b = noreport_error[errnumber]; */
      if ( nabs )
        noreport_error[errnumber] = enable;
      else
        noreport_error[errnumber] += enable /* < 0 ? -1 : 1 */ ;
      /* if ( rc == DUF_ERROR_TOO_DEEP )                                                                                         */
      /* {                                                                                                                          */
      /*   if ( b < noreport_error[errnumber] )                                                                                      */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d+%d=%d (nabs:%d)\n", b, noreport_error[errnumber] - b, noreport_error[errnumber], nabs ); */
      /*   else if ( b > noreport_error[errnumber] )                                                                                 */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d-%d=%d (nabs:%d)\n", b, b - noreport_error[errnumber], noreport_error[errnumber], nabs ); */
      /*   else                                                                                                                     */
      /*     fprintf( stderr, "$$$$$$$$$$$$ ==%d (nabs:%d)\n", b, nabs );                                                             */
      /* }                                                                                                                          */
    }
  }
}

void
mas_enable_ereport_c( int once, int enable, int nabs, mas_error_code_t rc )
{
/////  MAST_TRACE( errors, 5, "set report for error number %d / %d (%s)", rc, rc - MASE_ERROR_ERROR_BASE, mas_error_name_c( rc ) );
  _mas_enable_ereport_c( once, enable, nabs, rc, DUF_ERROR_COUNT );
}

void
mas_venable_mereport_c( int once, int enable, int nabs, va_list args )
{
  mas_error_code_t rc = 0;

  do
  {
    rc = va_arg( args, int );

    if ( rc )
      mas_enable_ereport_c( once, enable, nabs, rc );
  }
  while ( rc );
}

void
mas_enable_mereport_c( int once, int enable, int nabs, ... )
{
  va_list args;

  va_start( args, nabs );
  mas_venable_mereport_c( once, enable, nabs, args );
  va_end( args );
}

static int
_mas_enabled_ereport_n_c( mas_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    assert( errnumber >= 0 );
    if ( errnumber >= 0 && errnumber < maxerr )
      re = noreport_error[errnumber];
  }
  return re;
}


/* >0 -- report it */
static int
_mas_enabled_ereport_c( mas_error_code_t rc, int maxerr )
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
      re = 0;                   /* sql ? ? ? */
    }
    else if ( errnumber >= 0 && errnumber < maxerr
              && ( max_show_count_error[errnumber] <= 0 || count_reported[errnumber] < max_show_count_error[errnumber] - 1 ) )
      re = noreport_error[errnumber];
  }
  return re;
}

static int
_mas_enabled_ereport_i( mas_error_index_t ri, int maxerr )
{
  return _mas_enabled_ereport_c( mas_error_code_i( ri ), maxerr );
}

int
mas_enabled_ereport_n_c( mas_error_code_t rc )
{
  int re = 0;

  re = _mas_enabled_ereport_n_c( rc, DUF_ERROR_COUNT );
  return re;
}

int
mas_enabled_ereport_n_i( mas_error_index_t ri )
{
  return mas_enabled_ereport_n_c( mas_error_code_i( ri ) );
}

#if 1
int
mas_enabled_ereport_i( mas_error_index_t ri )
{
  int re = 0;

  if ( ri < 0 )
  {
    int en;

    en = _mas_enabled_ereport_i( ri, DUF_ERROR_COUNT );
    if ( force_count_ereport > 0 )
      re = force_count_ereport--;
    else if ( force_fixed_ereport )
      re = force_fixed_ereport;
    else
      re = en;
    re += force_offset_ereport;
    /* if ( re > 0 )                      */
    /*   T( "@@@@@re:%d;en:%d", re, en ); */
#  if 0
    {
      fprintf( stderr, "ri:%d;en:%d;force_offset_ereport:%d;force_fixed_ereport:%d;force_count_ereport:%d;re:%d\n", ri, en, force_offset_ereport,
               force_fixed_ereport, force_count_ereport, re );
    }
#  endif
  }
  return re;
}
#else
int
mas_enabled_ereport_i( mas_error_index_t ri )
{
  int re = 0;

  if ( ri < 0 )
  {
    if ( force_count_ereport > 0 )
    {
      re = force_count_ereport--;
    }
    else
    {
      DOCF( re, _mas_enabled_ereport_i, ri, DUF_ERROR_COUNT );
    }
  }
  return re;
}
#endif
static void
_mas_set_emax_count_c( int maxcount, mas_error_code_t rc, int maxerr )
{
  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      max_show_count_error[errnumber] = maxcount + 1;
  }
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
_mas_ecount_reported_c( mas_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = mas_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      re = count_reported[errnumber]++;
  }
  return re;
}

int
mas_ecount_reported_c( mas_error_code_t rc )
{
  int re = 0;

  re = _mas_ecount_reported_c( rc, DUF_ERROR_COUNT );
  return re;
}

int
mas_ecount_reported_rev( mas_error_event_t * rev )
{
  if ( rev )
    rev->count_reported++;
  return mas_ecount_reported_c( mas_error_code_rev( rev ) );
}

int
mas_ecount_reported_i( mas_error_code_t ri )
{
  mas_error_event_t *rev = NULL;

  rev = mas_find_error_event_i( ri );
  if ( rev )
    rev->count_reported++;
  return mas_ecount_reported_c( mas_error_code_rev( rev ) );
}

void
mas_error_report_i( mas_error_code_t ri, int test, FILE * out, int verb )
{
  if ( ri < 0 )
  {
    mas_error_event_t *rev = mas_find_error_event_i( ri );
    int erep __attribute__ ( ( unused ) ) = mas_ecount_reported_rev( rev );
    int irep __attribute__ ( ( unused ) ) = mas_icount_reported_rev( rev );
    const char *func = mas_error_func_rev( rev );
    int line = mas_error_line_rev( rev );
    const char *msg = mas_error_message_rev( rev );
    const char *ename = mas_error_name_rev( rev );
    const char *prefix = test ? "@@ [TEST] " : "@@  ERROR";

    switch ( verb )
    {
    case 0:
      MAST_FPRINTF0( 0, out, ".   " );
      MAST_FPRINTF0( 0, out, ".%s      ", prefix );
      /* MAST_FPRINTF0( 0, out, ".@@  %s    ", ename ); */
      /* MAST_FPRINTF0( 0, out, "@@@@@@@@" "%s%s", msg ? "  " : "", msg ); */
      MAST_FPRINTF0( 0, out, "@@@@@@@@" "[%s]%s%s (%d)", ename, msg ? " - " : "", msg, verb );
      break;
    case 1:
      MAST_FPRINTF0( 0, out, ".   " );
      MAST_FPRINTF0( 0, out, ".%s      ", prefix );
      MAST_FPRINTF0( 0, out, "@@@@@@@@" "[%s]%s%s (%s:%d) (%d)", ename, msg ? " - " : "", msg, func, line, verb );
      break;
    case 2:
      MASE_SHOW_ERRORO_WP( prefix, "@@@@@@@@" "[%s]%s%s (%s:%d) verb:%d", ename, msg ? " - " : "", msg, func, line, verb );
      break;
    case 3:
      MASE_SHOW_ERRORO_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                           ename, msg ? " - " : "", msg, ri, mas_enabled_ereport_n_i( ri ), mas_error_list_size(  ), erep, irep, func, line, verb );
      break;
    case 4:
      MASE_SHOW_ERRORO_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                           ename, msg ? " - " : "", msg, ri, mas_enabled_ereport_n_i( ri ), mas_error_list_size(  ), erep, irep, func, line, verb );
      break;
    case 5:
      MASE_SHOW_ERRORO_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                           ename, msg ? " - " : "", msg, ri, mas_enabled_ereport_n_i( ri ), mas_error_list_size(  ), erep, irep, func, line, verb );
      break;
    default:
      MASE_SHOW_ERRORO_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                           ename, msg ? " - " : "", msg, ri, mas_enabled_ereport_n_i( ri ), mas_error_list_size(  ), erep, irep, func, line, verb );
      break;
    }
  }
}

void
mas_error_report_p( size_t rp, int test, FILE * out, int verb )
{
  mas_error_event_t *rev = NULL;

  /* assert( rp >= 0 ); */
  rev = mas_find_error_event_p( rp );
  if ( rev )
    mas_error_report_i( rev->index, test, out, verb );
}

void
mas_error_report_all( int test, FILE * out, int verb )
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
      mas_error_report_p( rp, test, out, verb );
#endif
      k++;
    }
  }
}
