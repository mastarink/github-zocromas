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
static int count_error[DUF_ERROR_COUNT] = { 0 };
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
              && ( max_show_count_error[errnumber] <= 0 || count_error[errnumber] < max_show_count_error[errnumber] - 1 ) )
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
      re = count_error[errnumber]++;
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
duf_ecount_reported_i( duf_error_code_t ri )
{
  return duf_ecount_reported_c( duf_error_code_i( ri ) );
}
