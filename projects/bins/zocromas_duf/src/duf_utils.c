#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql_const.h"

/* ###################################################################### */
#include "duf_utils.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */

#define FN_FMT "27s"
/* #define FN_FMT "23s" */

#define DUF_SINGLE_QUOTES_2_NOQ_ZERO
char *
duf_single_quotes_2( const char *s )
{
  char *r = NULL;

#ifdef DUF_SINGLE_QUOTES_2_NOQ_ZERO
  if ( s && strchr( s, '\'' ) )
#endif
  {
    size_t len;
    const char *ip;
    char *op;
    size_t siz;
    unsigned qcnt = 0;

    len = strlen( s );
    ip = s;
    while ( ip && *ip )
    {
      if ( *ip++ == '\'' )
        qcnt++;
    }
    if ( qcnt )
    {
      siz = ( len + 1 + qcnt );
      r = mas_malloc( siz );
      /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
      ip = s;
      op = r;
      while ( ip && *ip && op < r + siz )
      {
        if ( *ip == '\'' )
          *op++ = '\'';
        *op++ = *ip;
        ip++;
      }
      *op++ = 0;
    }
#ifndef DUF_SINGLE_QUOTES_2_NOQ_ZERO
    else
    {
      r = mas_strdup( s );
    }
#endif
  }
  return r;
}

int
duf_filename_match( duf_filter_glob_t * glob, const char *filename )
{
  int r;

  r = 1;
  if ( glob )
  {
    if ( glob->include_files.argc )
    {
      int argc = glob->include_files.argc;
      char *const *argv = glob->include_files.argv;

      r = 0;
      /* DUF_TRACE_C( cfg, match, 2, "MATCH include argc:%d; %s", argc, filename ); */
      for ( int ia = 0; ia < argc; ia++ )
      {
        if ( 0 == fnmatch( argv[ia], filename, FNM_PATHNAME ) )
        {
          r = 1;
          /* DUF_TRACE_C( cfg, match, 1, "INCLUDE %s : %s", argv[ia], filename ); */
        }
      }
    }
    if ( glob->exclude_files.argc )
    {
      int argc = glob->exclude_files.argc;
      char *const *argv = glob->exclude_files.argv;

      /* DUF_TRACE_C( cfg, match, 2, "MATCH exclude argc:%d; %s", argc, filename ); */
      for ( int ia = 0; ia < argc; ia++ )
      {
        if ( 0 == fnmatch( argv[ia], filename, FNM_PATHNAME ) )
        {
          r = 0;
          /* DUF_TRACE_C( cfg, match, 1, "EXCLUDE %s : %s", argv[ia], filename ); */
        }
      }
    }
  }
  /* DUF_TRACE_C( cfg, match, 2, "MATCH %s %s", filename, r ? "OK" : "FAIL" ); */
  return r;
}

int
duf_lim_match( duf_limits_t lim, int n )
{
  int r;

  r = 1;
  if ( lim.min )
    r = ( n >= lim.min ) ? r : 0;
  if ( lim.max )
    r = ( n <= lim.max ) ? r : 0;
  return r;
}

int
duf_lim_matchll( duf_limitsll_t lim, int n )
{
  int r;

  r = 1;
  if ( lim.min )
    r = ( n >= lim.min ) ? r : 0;
  if ( lim.max )
    r = ( n <= lim.max ) ? r : 0;
  return r;
}


int
duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id )
{
  int r;

  r = 1;
  if ( md5id_filter )
    r = ( md5id == md5id_filter ) ? r : 0;
  return r;
}

int
duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  if ( level >= minlevel )
  {
    char rf = 0;
    const char *pfuncid;

    pfuncid = funcid;
    /* if ( 0 == strncmp( pfuncid, "duf_", 4 ) ) */
    /*   pfuncid += 4;                           */

    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' || rf == '+' )
      fmt++;
    if ( ifexit )
    {
      fprintf( out, "[DIE] @ %d:%d %3u:%-" FN_FMT ": ", level, minlevel, linid, pfuncid );
    }
    else if ( rf == '+' )
    {
      fprintf( out, "%d:%d %3u:%-" FN_FMT ": ", level, minlevel, linid, funcid );
    }
    {
      r = vfprintf( out, fmt, args );
    }
    if ( rf != '.' && rf != ':' )
    {
      fprintf( out, "\n" );
    }
  }
  else
  {
    /* fprintf( out, "SKP %d %d\n", level, minlevel ); */
  }
  if ( ifexit )
    exit( ifexit );
  return r;
}

int
duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
{
  int r;
  va_list args;

  va_start( args, fmt );
  r = duf_vprintf( level, minlevel, ifexit, funcid, linid, out, fmt, args );
  va_end( args );
  return r;
}

int
duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str )
{
  int r = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
      r = fputs( str, out );
    if ( r >= 0 )
      fputs( "\n", out );
  }
  return r;
}
