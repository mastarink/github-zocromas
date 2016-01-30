#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>


/* #include "mas_maintenance.h" */

/* ###################################################################### */
#include "mas_utils_print.h"
/* ###################################################################### */

const char *
mas_color_s( int is_atty, const char *s )
{
  return ( is_atty ) ? s : "";
}

const char *
mas_fcoloro_s( FILE * out, int force_color, int nocolor, const char *s )
{
  return mas_color_s( ( force_color || ( !nocolor && isatty( fileno( out ) ) ) ) ? 1 : 0, s );
  /* return mas_color_s( isatty( fileno( out ) ), s ); */
}

int
mas_vsncolor_s( int is_atty, char *buf, size_t size, const char *fmt, va_list args )
{
  int ry = -1;

  if ( buf && size > 0 )
  {
    *buf = 0;
    if ( is_atty )
      ry = vsnprintf( buf, size, fmt, args );
  }
  return ry;
}

int
mas_sncolor_s( int is_atty, char *buf, size_t size, const char *fmt, ... )
{
  int ry = 0;
  va_list args;

  va_start( args, fmt );
  ry = mas_vsncolor_s( is_atty, buf, size, fmt, args );
  va_end( args );
  return ry;
}

int
mas_vprint_coloro_s( FILE * out, int force_color, int nocolor, const char *fmt, va_list args )
{
  int ry = -1;
  char buf[2048];

  ry = mas_vsncolor_s( ( force_color || ( !nocolor && isatty( fileno( out ) ) ) ) ? 1 : 0, buf, sizeof( buf ), fmt, args );
  if ( *buf )
    ry = fputs( buf, out );
  /* ry = fwrite( buf, 1, strlen( buf ), out ); */
  return ry;
}

int
mas_print_coloro_s( FILE * out, int force_color, int nocolor, const char *fmt, ... )
{
  int ry = 0;
  va_list args;

  va_start( args, fmt );
  ry = mas_vprint_coloro_s( out, force_color, nocolor, fmt, args );
  va_end( args );
  return ry;
}

int
mas_print_color_s( FILE * out, const char *fmt, ... )
{
  int ry = 0;
  va_list args;
  int force_color = 0;
  int nocolor = 0;

  va_start( args, fmt );
  ry = mas_vprint_coloro_s( out, force_color, nocolor, fmt, args );
  va_end( args );
  return ry;
}

int
mas_vprintfo( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, int force_color, int nocolor,
              const char *fmt, va_list args )
{
  int ry = -1;
  int colorize = 1;

  if ( level >= minlevel )
  {
    char rf = 0;
    const char *pfuncid;
    unsigned highlight = 0;

    pfuncid = funcid;
    /* if ( 0 == strncmp( pfuncid, "mas_", 4 ) ) */
    /*   pfuncid += 4;                           */

    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' || rf == '+' )
      fmt++;


    if ( !out )
      out = stderr;

    if ( ifexit )
    {
      fprintf( out, "[DIE] @ %d:%d %3u:%-" P_FN_FMT "s: ", level, minlevel, linid, pfuncid );
    }
    else if ( rf == '+' )
    {
      fprintf( out, "%d:%d %3u:%-" P_FN_FMT "s: ", level, minlevel, linid, funcid );
    }
    if ( colorize )
    {
      int valid;

#if 0
      do
      {
        valid = 0;
        if ( *fmt == '@' )
        {
          valid = 1;
          highlight++;
        }
      }
      while ( fmt += valid, valid );
      {
        static char *hls[] = {
          "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47"
        };
        if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
          fprintf( out, "\x1b[%sm ", hls[highlight] );
        else if ( highlight )
          fprintf( out, "\x1b[%sm ", hls[0] );
        else
          fprintf( out, " " );
      }
      ry = vfprintf( out, fmt, args );
      if ( highlight )
        fprintf( out, "\x1b[m" );
#else
      /* TODO make new fmt with preset colors */
      char buf[1024 * 64];
      const char *pbuf = buf;

      ry = vsnprintf( buf, sizeof( buf ), fmt, args );
      do
      {
        valid = 0;
        if ( *pbuf == '@' )
        {
          valid = 1;
          highlight++;
        }
        {
          static const char *uliml = "∈";
          static const char *ulimr = "∋";
          static size_t lr = 0;
          static size_t ll = 0;

          if ( !ll )
            ll = strlen( uliml );
          if ( !lr )
            lr = strlen( ulimr );

          if ( !valid && pbuf[0] == uliml[0] && ulimr[0] == uliml[0] )
          {
            if ( 0 == strncmp( uliml, pbuf, ll ) )
              valid += ll;
            if ( valid )
            {
              char *e = NULL;

              if ( sscanf( &pbuf[valid], "%d", &highlight ) )
              {
                if ( ( e = strstr( &pbuf[valid], ulimr ) ) )
                  valid = e - pbuf + lr;
                else
                  while ( pbuf[valid] >= '0' && pbuf[valid] <= '9' )
                    valid++;
              }
              else
                valid = 0;
            }
          }
        }
      }
      while ( pbuf += valid, valid );
      {
        static char *hls[] = {
          "1;39", "1;7;37;40", "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;106;33", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47", "7;94;109",
          "7;96;109", "1;102;97", "1;104;96"
        };
        if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
#  if 0
          fprintf( out, "\x1b[%sm", hls[highlight] );
#  else
          mas_print_coloro_s( out, force_color, nocolor, "\x1b[;%sm", hls[highlight] );
#  endif
        else if ( highlight )
#  if 0
          fprintf( out, "\x1b[%sm", hls[0] );
#  else
          mas_print_coloro_s( out, force_color, nocolor, "\x1b[;%sm", hls[0] );
#  endif
        /* fprintf( out, "%s", pbuf ); */
#  if 0
        {
          unsigned char *ttt = ( unsigned char * ) "◎";

          fprintf( out, "{%lu:%2x:%2x:%2x}", strlen( ( char * ) ttt ), ttt[0], ttt[1], ttt[2] );
        }
#  endif
        fwrite( pbuf, 1, strlen( pbuf ), out );
        if ( highlight )
#  if 0
          fprintf( out, "\x1b[m" );
#  else
          mas_print_coloro_s( out, force_color, nocolor, "\x1b[;%sm", "" );
#  endif
      }
#endif
    }
    if ( rf != '.' && rf != ':' && !noeol )
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
  return ry;
}

int
mas_printfo( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, int force_color, int nocolor,
             const char *fmt, ... )
{
  int ry = 0;
  va_list args;

  va_start( args, fmt );
  ry = mas_vprintfo( level, noeol, minlevel, ifexit, funcid, linid, out, force_color, nocolor, fmt, args );
  va_end( args );
  return ry;
}

int
mas_puts( int level, int minlevel, const char *funcid __attribute__ ( ( unused ) ), int linid
          __attribute__ ( ( unused ) ), FILE * out, const char *str )
{
  int ry = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
      ry = fputs( str, out );
    if ( ry >= 0 )
      fputs( "\n", out );
  }
  return ry;
}

int
mas_writes( int level, int minlevel, const char *funcid __attribute__ ( ( unused ) ), int linid
            __attribute__ ( ( unused ) ), FILE * out, const char *str )
{
  int ry = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
    {
      ry = fwrite( str, 1, strlen( str ), out );
    }
  }
  return ry;
}
