#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql_def.h"

/* ###################################################################### */
#include "duf_utils.h"
/* ###################################################################### */

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


typedef struct
{
  const char *name;
  duf_error_code_t code;
} duf_errdesc_t;


const char *
duf_error_name( duf_error_code_t c )
{
  static char buf[512];
  int found = 0;

#define DUF_ERROR_NAME(ername) {.name=#ername, .code=ername}

  const duf_errdesc_t table[] = {
    DUF_ERROR_NAME( DUF_SQL_ERROR ),
    DUF_ERROR_NAME( DUF_SQL_INTERNAL ),
    DUF_ERROR_NAME( DUF_SQL_PERM ),
    DUF_ERROR_NAME( DUF_SQL_ABORT ),
    DUF_ERROR_NAME( DUF_SQL_BUSY ),
    DUF_ERROR_NAME( DUF_SQL_LOCKED ),
    DUF_ERROR_NAME( DUF_SQL_NOMEM ),
    DUF_ERROR_NAME( DUF_SQL_READONLY ),
    DUF_ERROR_NAME( DUF_SQL_INTERRUPT ),
    DUF_ERROR_NAME( DUF_SQL_IOERR ),
    DUF_ERROR_NAME( DUF_SQL_CORRUPT ),
    DUF_ERROR_NAME( DUF_SQL_NOTFOUND ),
    DUF_ERROR_NAME( DUF_SQL_FULL ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN ),
    DUF_ERROR_NAME( DUF_SQL_PROTOCOL ),
    DUF_ERROR_NAME( DUF_SQL_EMPTY ),
    DUF_ERROR_NAME( DUF_SQL_SCHEMA ),
    DUF_ERROR_NAME( DUF_SQL_TOOBIG ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT ),
    DUF_ERROR_NAME( DUF_SQL_MISMATCH ),
    DUF_ERROR_NAME( DUF_SQL_MISUSE ),
    DUF_ERROR_NAME( DUF_SQL_NOLFS ),
    DUF_ERROR_NAME( DUF_SQL_AUTH ),
    DUF_ERROR_NAME( DUF_SQL_FORMAT ),
    DUF_ERROR_NAME( DUF_SQL_RANGE ),
    DUF_ERROR_NAME( DUF_SQL_NOTADB ),
    DUF_ERROR_NAME( DUF_SQL_NOTICE ),
    DUF_ERROR_NAME( DUF_SQL_WARNING ),
/*========================================*/
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN ),
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN_NODE ),
    DUF_ERROR_NAME( DUF_ERROR_MAIN ),
    DUF_ERROR_NAME( DUF_ERROR_PTR ),
    DUF_ERROR_NAME( DUF_ERROR_DATA ),
    DUF_ERROR_NAME( DUF_ERROR_NOT_OPEN ),
    DUF_ERROR_NAME( DUF_ERROR_OPENAT ),
    DUF_ERROR_NAME( DUF_ERROR_OPEN ),
    DUF_ERROR_NAME( DUF_ERROR_CLOSE ),
    DUF_ERROR_NAME( DUF_ERROR_UNLINK ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION ),
    DUF_ERROR_NAME( DUF_ERROR_SCANDIR ),
    DUF_ERROR_NAME( DUF_ERROR_CHECK_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_CLEAR_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FILE_SELECTOR ),
    DUF_ERROR_NAME( DUF_ERROR_DB_NO_PATH ),
    DUF_ERROR_NAME( DUF_ERROR_NO_STR_CB ),
    DUF_ERROR_NAME( DUF_ERROR_MAX_DEPTH ),
    DUF_ERROR_NAME( DUF_ERROR_MAX_REACHED ),
    DUF_ERROR_NAME( DUF_ERROR_GET_FIELD ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FIELD ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FIELD_OPTIONAL ),
    DUF_ERROR_NAME( DUF_ERROR_INSERT_MDPATH ),
    DUF_ERROR_NAME( DUF_ERROR_STAT ),
/*========================================*/
    DUF_ERROR_NAME( DUF_ERROR_ERROR_MAX ),
  };

  for ( int i = 0; i < sizeof( table ) / sizeof( table[9] ); i++ )
  {
    if ( c == table[i].code )
    {
      snprintf( buf, sizeof( buf ), "%s", table[i].name + 4 );
      found = 1;
      break;
    }
  }
  if ( !found )
    snprintf( buf, sizeof( buf ), "Unknown code %d", c );
  return buf;
}

static int
duf_vtrace_error( duf_trace_mode_t trace_mode, const char *name, int level, duf_error_code_t ern, const char *funcid, int linid,
                  unsigned flags, int nerr, FILE * out, const char *fmt, va_list args )
{
  int r = 0;

  if ( ern < 0 )
  {
    const char *s = duf_error_name( ern );

    if ( ern < 0 && s )
      fprintf( out, "\n  [%s (%d)]\n", duf_error_name( ern ), +ern - DUF_ERROR_ERROR_BASE );
    else
      fprintf( out, "Error rv=%d\n", ern );
  }
  return r;
}

int
duf_vtrace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags, int nerr,
            FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  if ( trace_mode == DUF_TRACE_MODE_errorr )
  {
    r = duf_vtrace_error( trace_mode, name, level, minlevel, funcid, linid, flags, nerr, out, fmt, args );
  }
  else if ( level > minlevel )
  {
    char uname[10], *puname;
    char rf = 0;

    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' )
      fmt++;

    if ( rf != '.' && rf != ';' )
    {
      puname = uname;
      for ( int i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;
      fprintf( out, "%d:%d [%-7s] %3u:%-23s: ", level, minlevel, uname, linid, funcid );
    }
    {
      r = vfprintf( out, fmt, args );
    }
    if ( flags & DUF_TRACE_FLAG_SYSTEM )
    {
      char serr[1024] = "Why?";
      char *s;

      s = strerror_r( nerr, serr, sizeof( serr ) - 1 );
      fprintf( out, "; errno:(%d) [%s]", nerr, s );
    }
    if ( rf != '.' && rf != ':' )
    {
      fprintf( out, "\n" );
    }
  }
  return r;
}

int
duf_trace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags, int nerr,
           FILE * out, const char *fmt, ... )
{
  int r;
  va_list args;

  va_start( args, fmt );
  r = duf_vtrace( trace_mode, name, level, minlevel, funcid, linid, flags, nerr, out, fmt, args );
  va_end( args );
  return r;
}


int
duf_vprintf( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  if ( level >= minlevel )
  {
    char rf = 0;

    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' || rf == '+' )
      fmt++;

    if ( rf == '+' )
    {
      fprintf( out, "%d:%d %3u:%-23s: ", level, minlevel, linid, funcid );
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
  return r;
}

int
duf_printf( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, ... )
{
  int r;
  va_list args;

  va_start( args, fmt );
  r = duf_vprintf( level, minlevel, funcid, linid, out, fmt, args );
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
