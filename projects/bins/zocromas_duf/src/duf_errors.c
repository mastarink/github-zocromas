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

/* ###################################################################### */
#include "duf_errors.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */
static int noreport_error[DUF_ERROR_COUNT] = { 0 };
static int count_error[DUF_ERROR_COUNT] = { 0 };
static int max_show_count_error[DUF_ERROR_COUNT] = { 0 };

static long _made_errors = 0;
static int force_ereport = 0;

#define MAX_ERRORS 100000l

static duf_error_event_t *global_error_list = NULL;

__attribute__ ( ( destructor( 65535 ) ) )
     static void destructor_errorlist( void )
{
  mas_free( global_error_list );
  global_error_list = NULL;
}

void
duf_force_ereport( int count )
{
  force_ereport = count;
}

duf_error_index_t
duf_make_error( duf_error_code_t err, const char *funcid, int linid, const char *message )
{
  duf_error_index_t e = 0;

  if ( err < 0 )
  {
    if ( !global_error_list )
    {
      global_error_list = mas_malloc( sizeof( duf_error_event_t ) * MAX_ERRORS );
      memset( global_error_list, 0, sizeof( duf_error_event_t ) * MAX_ERRORS );
    }
    assert( _made_errors < MAX_ERRORS );
    if ( _made_errors < MAX_ERRORS )
    {
      global_error_list[_made_errors].err = err;
      global_error_list[_made_errors].funcid = funcid;
      global_error_list[_made_errors].linid = linid;
      global_error_list[_made_errors].message = message;
      e = -( _made_errors + 1 );
      assert( e < 0 );
    }
    _made_errors++;
    /* fprintf( stderr, "_made_errors:%ld (%d) %s\n", _made_errors, err, duf_error_name_c( err ) ); */
    DUF_TRACE( error, 5, "made error err:%d; e:%d; [%ld:%ld]", err, e, _made_errors, MAX_ERRORS );
    assert( e < 0 );
  }
  return e;
}

duf_error_code_t
duf_error_code_i( duf_error_index_t e )
{
  duf_error_code_t r = -1;
  long made;

  made = -e - 1;
  DUF_TRACE( error, 5, "_made_errors:%ld; made:%ld; e:%d;", _made_errors, made, e );
  if ( e == 0 )
    r = 0;
  else if ( made >= 0 && made < _made_errors )
    r = global_error_list[made].err;
  return r;
}

long
duf_made_errors( duf_error_code_t err )
{
  return _made_errors;
}

int
duf_errnumber( duf_error_code_t rtest )
{
  DUF_TRACE( error, 5, "calc error number from error code %d => %d (%s)", rtest, rtest - DUF_ERROR_ERROR_BASE, duf_error_name_c( rtest ) );
  return ( rtest - DUF_ERROR_ERROR_BASE );
}

int
duf_errnumber_sql( duf_error_code_t rtest )
{
  DUF_TRACE( error, 5, "calc error number from sql error code %d / %d", rtest, rtest - DUF_ERROR_ERROR_BASE );
  return ( rtest - MAS_SQLITE_ERROR_BASE );
}

static void
_duf_set_ereport( int once, int enable, int abs, duf_error_code_t rtest, int maxerr )
{
  assert( ( rtest >= DUF_SQL_ERROR && rtest <= DUF_SQL_WARNING_AUTOINDEX ) || ( rtest >= DUF_ERROR_ERROR_BASE && rtest <= DUF_ERROR_ERROR_MAX ) );
  if ( rtest < 0 )
  {
    int errnumber = duf_errnumber( rtest );

    DUF_TRACE( error, 5, "set report for error number %d / %d (%s)", rtest, rtest - DUF_ERROR_ERROR_BASE, duf_error_name_c( rtest ) );

    if ( errnumber >= 0 && errnumber < maxerr )
    {
      /* int b;                        */
      /*                               */
      /* b = noreport_error[errnumber]; */
      if ( abs )
        noreport_error[errnumber] = enable;
      else
        noreport_error[errnumber] += enable /* < 0 ? -1 : 1 */ ;
      /* if ( rtest == DUF_ERROR_TOO_DEEP )                                                                                         */
      /* {                                                                                                                          */
      /*   if ( b < noreport_error[errnumber] )                                                                                      */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d+%d=%d (abs:%d)\n", b, noreport_error[errnumber] - b, noreport_error[errnumber], abs ); */
      /*   else if ( b > noreport_error[errnumber] )                                                                                 */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d-%d=%d (abs:%d)\n", b, b - noreport_error[errnumber], noreport_error[errnumber], abs ); */
      /*   else                                                                                                                     */
      /*     fprintf( stderr, "$$$$$$$$$$$$ ==%d (abs:%d)\n", b, abs );                                                             */
      /* }                                                                                                                          */
    }
  }
}

void
duf_set_ereport( int once, int enable, int abs, duf_error_code_t rtest )
{
  DUF_TRACE( error, 5, "set report for error number %d / %d (%s)", rtest, rtest - DUF_ERROR_ERROR_BASE, duf_error_name_c( rtest ) );
  _duf_set_ereport( once, enable, abs, rtest, DUF_ERROR_COUNT );
}

void
duf_vset_mereport( int once, int enable, int abs, va_list args )
{
  duf_error_code_t rtest = 0;

  do
  {
    rtest = va_arg( args, int );

    if ( rtest )
      duf_set_ereport( once, enable, abs, rtest );
  }
  while ( rtest );
}

void
duf_set_mereport( int once, int enable, int abs, ... )
{
  va_list args;

  va_start( args, abs );
  duf_vset_mereport( once, enable, abs, args );
  va_end( args );
}

static int
_duf_get_ereport_n( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  if ( rtest < 0 )
  {
    int errnumber = duf_errnumber( rtest );

    if ( errnumber >= 0 && errnumber < maxerr )
      re = noreport_error[errnumber];
  }
  return re;
}

/* >0 -- report it */
static int
_duf_get_ereport( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  int errnumber = duf_errnumber( rtest );

  /* if ( rtest < 0 ) */
  /*   return 1;      */
  if ( rtest < 0 )
  {
    if ( errnumber < 0 )
    {
      re = 0;                   /* sql ? ? ? */
    }
    else if ( errnumber >= 0 && errnumber < maxerr
              && ( max_show_count_error[errnumber] <= 0 || count_error[errnumber] < max_show_count_error[errnumber] - 1 ) )
      re = noreport_error[errnumber] + 1;
  }
  return re;
}

int
duf_get_ereport_n( duf_error_code_t rtest )
{
  int re = 0;

  DOCF( re, _duf_get_ereport_n, rtest, DUF_ERROR_COUNT );
  return re;
}

int
duf_get_ereport( duf_error_code_t rtest )
{
  int re = 0;

  if ( rtest < 0 )
  {
    if ( force_ereport > 0 )
    {
      re = force_ereport--;
    }
    else
    {
      DOCF( re, _duf_get_ereport, rtest, DUF_ERROR_COUNT );
    }
  }
  return re;
}

static void
_duf_set_emax_count( int maxcount, duf_error_code_t rtest, int maxerr )
{
  if ( rtest < 0 )
  {
    int errnumber = duf_errnumber( rtest );

    if ( errnumber >= 0 && errnumber < maxerr )
      max_show_count_error[errnumber] = maxcount + 1;
  }
}

void
duf_set_emax_count( int maxcount, duf_error_code_t rtest )
{
  _duf_set_emax_count( maxcount, rtest, DUF_ERROR_COUNT );
}

void
duf_vset_emax_count( int maxcount, va_list args )
{
  duf_error_code_t rtest = 0;

  do
  {
    rtest = va_arg( args, int );

    if ( rtest )
      duf_set_emax_count( maxcount, rtest );
  }
  while ( rtest );
}

void
duf_set_memax_count( int maxcount, ... )
{
  va_list args;

  va_start( args, maxcount );
  duf_vset_emax_count( maxcount, args );
  va_end( args );
}

int
_duf_ecount( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  if ( rtest < 0 )
  {
    int errnumber = duf_errnumber( rtest );

    if ( errnumber >= 0 && errnumber < maxerr )
      re = count_error[errnumber]++;
  }
  return re;
}

int
duf_ecount( duf_error_code_t rtest )
{
  int re = 0;

  DOCF( re, _duf_ecount, rtest, DUF_ERROR_COUNT );
  return re;
}

duf_error_code_t
duf_vclear_error_c( duf_error_code_t re, va_list args )
{
  duf_error_code_t c = 0;

  do
  {
    c = va_arg( args, int );

    {
      char *en1 = mas_strdup( duf_error_name_c( re ) );
      char *en2 = mas_strdup( duf_error_name_c( c ) );

      DUF_TRACE( error, 2, "@@@clear (%d) %s ? (%d) %s (2nd from list)", re, en1, c, en2 );
      mas_free( en1 );
      mas_free( en2 );
    }
    if ( re == c )
    {
      re = 0;
      break;
    }
  }
  while ( c );
  return re;
}

duf_error_code_t
duf_clear_error_c( duf_error_code_t re, ... )
{
  va_list args;

  va_start( args, re );
  re = duf_vclear_error_c( re, args );
  va_end( args );
  return re;
}

duf_error_index_t
duf_clear_error_i( duf_error_index_t e, ... )
{
  va_list args;
  duf_error_code_t c = 0;

  va_start( args, e );
  if ( DUF_IS_ERROR( e ) )
  {
    c = duf_error_code_i( e );
    DUF_TRACE( error, 2, "c:%d <= e:%d", c, e );
    if ( DUF_IS_ERROR( c ) )
    {
      c = duf_vclear_error_c( c, args );
      DUF_TRACE( error, 2, "@(%d) %s", c, duf_error_name_c( c ) );
      e = 0;
      if ( DUF_IS_ERROR( c ) )
        DUF_MAKE_ERROR( e, c );
    }
  }
  va_end( args );
  DUF_TRACE( error, 2, "@(%d) %s %s", c, duf_error_name_c( c ), duf_error_name_i( e ) );

  return e;
}

const char *
duf_error_name_c( duf_error_code_t c )
{
  const char *pserr = NULL;

  /* static char buf[2048]; */
  int found = 0;

  for ( int i = 0; i < duf_error_descriptions_table_size; i++ )
  {
    DUF_TRACE( error, 10, "c:%d t[%d]:%d", c, i, duf_error_descriptions_table[i].code );
    if ( c == duf_error_descriptions_table[i].code )
    {
      DUF_TRACE( error, 7, "c:%d t[%d]:%d => %s", c, i, duf_error_descriptions_table[i].code, duf_error_descriptions_table[i].name );
#if 0
      snprintf( buf, sizeof( buf ), "%s", duf_error_descriptions_table[i].name + 4 );
      pserr = buf;
#else
      pserr = duf_error_descriptions_table[i].name + 4;
#endif
      found = 1;
      break;
    }
  }
  if ( !found )
    pserr = "Unknown errcode";
  /* snprintf( buf, sizeof( buf ), "Unknown errcode %d", c ); */
  return pserr;
}

const char *
duf_error_name_i( duf_error_index_t e )
{
  duf_error_code_t c;

  c = duf_error_code_i( e );
  return duf_error_name_c( c );
}

const char *
duf_error_func_i( duf_error_index_t e )
{
  return global_error_list[-e - 1].funcid;
}

int
duf_error_line_i( duf_error_index_t e )
{
  return global_error_list[-e - 1].linid;
}

const char *
duf_error_message_i( duf_error_index_t e )
{
  return global_error_list[-e - 1].message;
}
