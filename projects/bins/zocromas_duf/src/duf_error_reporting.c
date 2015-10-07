#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_maintenance.h"

#include "duf_error_names.h"

/* #include "duf_sql_const.h" */

#include "duf_error_regfind.h"
/* ###################################################################### */
#include "duf_error_reporting.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */
static int noreport_error[DUF_ERROR_COUNT] = { 0 };
static int count_reported[DUF_ERROR_COUNT] = { 0 };
static int max_show_count_error[DUF_ERROR_COUNT] = { 0 };

static int force_count_ereport = 0;
static int force_fixed_ereport = 0;
static int force_offset_ereport = 0;


void
duf_force_count_ereport( int count )
{
  force_count_ereport = count;
}

void
duf_force_fixed_ereport( int count )
{
  force_fixed_ereport = count;
}

void
duf_force_offset_ereport( int count )
{
  force_offset_ereport = count;
}

static void
_duf_enable_ereport_c( int once DUF_UNUSED, int enable, int nabs, duf_error_code_t rc, int maxerr )
{
  assert( ( rc >= DUF_SQL_ERROR && rc <= DUF_SQL_WARNING_AUTOINDEX ) || ( rc >= DUF_ERROR_ERROR_BASE && rc <= DUF_ERROR_ERROR_MAX ) );
  if ( rc < 0 )
  {
    int errnumber = duf_errnumber_c( rc );

    assert( errnumber >= 0 );
    DUF_TRACE( error, 5, "set report for error number %d / %d (%s)", rc, rc - DUF_ERROR_ERROR_BASE, duf_error_name_c( rc ) );
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
duf_enable_ereport_c( int once, int enable, int nabs, duf_error_code_t rc )
{
  DUF_TRACE( error, 5, "set report for error number %d / %d (%s)", rc, rc - DUF_ERROR_ERROR_BASE, duf_error_name_c( rc ) );
  _duf_enable_ereport_c( once, enable, nabs, rc, DUF_ERROR_COUNT );
}

void
duf_venable_mereport_c( int once, int enable, int nabs, va_list args )
{
  duf_error_code_t rc = 0;

  do
  {
    rc = va_arg( args, int );

    if ( rc )
      duf_enable_ereport_c( once, enable, nabs, rc );
  }
  while ( rc );
}

void
duf_enable_mereport_c( int once, int enable, int nabs, ... )
{
  va_list args;

  va_start( args, nabs );
  duf_venable_mereport_c( once, enable, nabs, args );
  va_end( args );
}

static int
_duf_enabled_ereport_n_c( duf_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = duf_errnumber_c( rc );

    assert( errnumber >= 0 );
    if ( errnumber >= 0 && errnumber < maxerr )
      re = noreport_error[errnumber];
  }
  return re;
}


/* >0 -- report it */
static int
_duf_enabled_ereport_c( duf_error_code_t rc, int maxerr )
{
  int re = 0;

  int errnumber = duf_errnumber_c( rc );

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
_duf_enabled_ereport_i( duf_error_index_t ri, int maxerr )
{
  return _duf_enabled_ereport_c( duf_error_code_i( ri ), maxerr );
}

int
duf_enabled_ereport_n_c( duf_error_code_t rc )
{
  int re = 0;

  DOCF( re, _duf_enabled_ereport_n_c, rc, DUF_ERROR_COUNT );
  return re;
}

int
duf_enabled_ereport_n_i( duf_error_index_t ri )
{
  return duf_enabled_ereport_n_c( duf_error_code_i( ri ) );
}

#if 1
int
duf_enabled_ereport_i( duf_error_index_t ri )
{
  int re = 0;

  if ( ri < 0 )
  {
    int en;

    en = _duf_enabled_ereport_i( ri, DUF_ERROR_COUNT );
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
duf_enabled_ereport_i( duf_error_index_t ri )
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
      DOCF( re, _duf_enabled_ereport_i, ri, DUF_ERROR_COUNT );
    }
  }
  return re;
}
#endif
static void
_duf_set_emax_count_c( int maxcount, duf_error_code_t rc, int maxerr )
{
  if ( rc < 0 )
  {
    int errnumber = duf_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      max_show_count_error[errnumber] = maxcount + 1;
  }
}

void
duf_set_emax_count_c( int maxcount, duf_error_code_t rc )
{
  _duf_set_emax_count_c( maxcount, rc, DUF_ERROR_COUNT );
}

void
duf_vset_emax_count_c( int maxcount, va_list args )
{
  duf_error_code_t rc = 0;

  do
  {
    rc = va_arg( args, int );

    if ( rc )
      duf_set_emax_count_c( maxcount, rc );
  }
  while ( rc );
}

void
duf_set_memax_count_c( int maxcount, ... )
{
  va_list args;

  va_start( args, maxcount );
  duf_vset_emax_count_c( maxcount, args );
  va_end( args );
}

int
_duf_ecount_reported_c( duf_error_code_t rc, int maxerr )
{
  int re = 0;

  if ( rc < 0 )
  {
    int errnumber = duf_errnumber_c( rc );

    if ( errnumber >= 0 && errnumber < maxerr )
      re = count_reported[errnumber]++;
  }
  return re;
}

int
duf_ecount_reported_c( duf_error_code_t rc )
{
  int re = 0;

  DOCF( re, _duf_ecount_reported_c, rc, DUF_ERROR_COUNT );
  return re;
}

int
duf_ecount_reported_rev( duf_error_event_t * rev )
{
  if ( rev )
    rev->count_reported++;
  return duf_ecount_reported_c( duf_error_code_rev( rev ) );
}

int
duf_ecount_reported_i( duf_error_code_t ri )
{
  duf_error_event_t *rev = NULL;

  rev = duf_find_error_event_i( ri );
  if ( rev )
    rev->count_reported++;
  return duf_ecount_reported_c( duf_error_code_rev( rev ) );
}

void
duf_error_report_i( duf_error_code_t ri, int test, int verb )
{

  if ( ri < 0 )
  {
    duf_error_event_t *rev = duf_find_error_event_i( ri );
    int erep = duf_ecount_reported_rev( rev );
    int irep = duf_icount_reported_rev( rev );
    const char *func = duf_error_func_rev( rev );
    int line = duf_error_line_rev( rev );
    const char *msg = duf_error_message_rev( rev );
    const char *ename = duf_error_name_rev( rev );
    const char *prefix = test ? "@@ [TEST] " : "@@  ERROR";

    switch ( verb )
    {
    case 0:
      DUF_FPRINTF0( 0, stderr, ".   " );
      DUF_FPRINTF0( 0, stderr, ".@@  %s    ", ename );
      DUF_FPRINTF0( 0, stderr, "@@@@@@@@" "%s%s", msg ? "  " : "", msg );
      break;
    case 1:
      DUF_FPRINTF0( 0, stderr, ".   " );
      DUF_FPRINTF0( 0, stderr, ".%s      ", prefix );
      DUF_FPRINTF0( 0, stderr, "@@@@@@@@" "[%s]%s%s (%s:%d)", ename, msg ? " - " : "", msg, func, line );
      break;
    case 2:
      DUF_SHOW_ERROR_WP( prefix, "@@@@@@@@" "[%s]%s%s (%s:%d) verb:%d", ename, msg ? " - " : "", msg, func, line, verb );
      break;
    case 3:
      DUF_SHOW_ERROR_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                         ename, msg ? " - " : "", msg, ri, duf_enabled_ereport_n_i( ri ), duf_error_list_size(  ), erep, irep, func, line , verb );
      break;
    case 4:
      DUF_SHOW_ERROR_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                         ename, msg ? " - " : "", msg, ri, duf_enabled_ereport_n_i( ri ), duf_error_list_size(  ), erep, irep, func, line , verb );
      break;
    case 5:
      DUF_SHOW_ERROR_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                         ename, msg ? " - " : "", msg, ri, duf_enabled_ereport_n_i( ri ), duf_error_list_size(  ), erep, irep, func, line , verb );
      break;
    default:
      DUF_SHOW_ERROR_WP( prefix, "@@@@@@@@" "[%s]%s%s (ri:%d) {en:%d} lsz:%ld rep:%u:%u (%s:%d) verb:%d",
                         ename, msg ? " - " : "", msg, ri, duf_enabled_ereport_n_i( ri ), duf_error_list_size(  ), erep, irep, func, line , verb );
    }
  }
}

void
duf_error_report_p( size_t rp, int test, int verb )
{
  duf_error_event_t *rev = NULL;

  /* assert( rp >= 0 ); */
  rev = duf_find_error_event_p( rp );
  if ( rev )
    duf_error_report_i( rev->index, test, verb );
}

void
duf_error_report_all( int test, int verb )
{
  unsigned k = 0;

  for ( unsigned rp = 0; rp < duf_error_list_size(  ); rp++ )
  {
    /* if ( duf_error_code_p( rp ) != DUF_SQL_DONE && duf_error_code_p( rp ) != DUF_SQL_ROW ) */
    {
      /* T( "@@@@@@%d. %d. %s @ %s:%d %s", rp, k, duf_error_name_p( rp ), duf_error_func_p( rp ), duf_error_line_p( rp ), duf_error_message_p( rp ) ); */
#if 0
      T( "@@@@@@@%d. %s @ %s:%d %s", rp + 1, duf_error_name_p( rp ), duf_error_func_p( rp ), duf_error_line_p( rp ), duf_error_message_p( rp ) );
      DUF_SHOW_ERROR( "@@@@@@@@%d. %s @ %s:%d %s", rp + 1, duf_error_name_p( rp ), duf_error_func_p( rp ), duf_error_line_p( rp ),
                      duf_error_message_p( rp ) );
#else
      duf_error_report_p( rp, test, verb );
#endif
      k++;
    }
  }
}
