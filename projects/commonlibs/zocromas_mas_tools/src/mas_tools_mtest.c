#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>
#include "mas_arg_tools.h"
#include "mas_tools.h"

int
main( int argc, char *argv[] )
{
  char *d;
  const char *s = "0123456789abcdefabcdefghijkl\r\nmnopqzABCDEF\tX";

  {
    struct tm *t;
    char buf[512];

    t = mas_xgmtime(  );
    strftime( buf, sizeof( buf ), "%a, %d %b %Y %T GMT", t );
    fprintf( stderr, "GMTime: %s\n", buf );
  }
  d = mas_dump1( s, strlen( s ) - 1, 0 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump1( s, strlen( s ) - 1, 16 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump1( s, strlen( s ) - 1, 32 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }

  d = mas_dump2( s, strlen( s ) - 1, 0 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump2( s, strlen( s ) - 1, 16 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump2( s, strlen( s ) - 1, 32 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  {
    char cmdline[] = "abc\0def\0ghi";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  {
    char cmdline[] = "abc\0def\0ghi\0";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  {
    char cmdline[] = "abc\0def\0ghi\0\0";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  fprintf( stderr, "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n" );
  {
    char cmdline[] =
          "xautolock -locker 'xlock -mode blank -resetsaver -startCmd /home/mastar/.mas/bin/xautolock/startcmd.sh -endCmd /home/mastar/.mas/bin/xautolock/endcmd.sh' -time 5 -notify 20 -cornerdelay 20 -notifier '/home/mastar/.mas/bin/xautolock/notify.sh' -corners '000+'";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_args( targc, &targv, cmdline, 0 );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  fprintf( stderr, "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n" );
  {
    const char *a;
    const char *this;
    size_t len;
    char cmdline[] =
          "xautolock -locker 'xlock -mode blank -resetsaver -startCmd /home/mastar/.mas/bin/xautolock/startcmd.sh -endCmd /home/mastar/.mas/bin/xautolock/endcmd.sh' -time 5 -notify 20 -cornerdelay 20 -notifier '/home/mastar/.mas/bin/xautolock/notify.sh' -corners '000+'";
    a = cmdline;
    fprintf( stderr, "[%s]\n", cmdline );
    while ( a )
    {
      char *t;

      a = mas_find_next_arg( a, &this, &len, NULL );
      t = mas_strndup( this, len );
      fprintf( stderr, "[%2u] t=[%s]\n", ( unsigned ) len, t );
      /* fprintf( stderr, "     a=[%s]\n", t ); */
      mas_free( t );
    }
  }
  fprintf( stderr, "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n" );
  {
    const char *a;
    const char *this;
    size_t len;
    char cmdline[] = "calibre-server --daemonize --thread-pool=10 --with-library='/mnt/miscn/mastar/Calibre Library/'";

    a = cmdline;
    fprintf( stderr, "[%s]\n", cmdline );
    while ( a )
    {
      char *t;

      a = mas_find_next_arg( a, &this, &len, NULL );
      t = mas_strndup( this, len );
      fprintf( stderr, "[%2u] t=[%s]\n", ( unsigned ) len, t );
      /* fprintf( stderr, "     a=[%s]\n", t ); */
      mas_free( t );
    }
  }
  fprintf( stderr, "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n" );
  {
    const char *a;
    const char *this;
    size_t len;
    char cmdline[] = "calibre-server --daemonize --thread-pool=10 --with-library=/mnt/miscn/mastar/Calibre\\ Library/";

    a = cmdline;
    fprintf( stderr, "[%s]\n", cmdline );
    while ( a )
    {
      char *t;

      a = mas_find_next_arg( a, &this, &len, NULL );
      t = mas_strndup( this, len );
      fprintf( stderr, "[%2u] t=[%s]\n", ( unsigned ) len, t );
      /* fprintf( stderr, "     a=[%s]\n", t ); */
      mas_free( t );
    }
  }
  fprintf( stderr, "()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()\n" );
  {
    char cmdline[] = "calibre-server --daemonize --thread-pool=10 --with-library=/mnt/miscn/mastar/Calibre\\ Library/";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_args( targc, &targv, cmdline, 0 );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  {
    char *s;
    const char *s0 = "this is a test$MSH_SHN_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test$$MSH_SHN_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test$.MSH_SHN_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a testMSH_SHN_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test${MSH_SHN_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test$${MSH_SHN_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a $HOMEtest${MSH_SHN_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a $HOME/test${MSH_SHN_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
{
    char *s;
    const char *s0 = "this is a ${HOME}test${MSH_SHN_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test$MSH_XY_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
{
    char *s;
    const char *s0 = "this is a test$$MSH_XY_BASHPID.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
{
    char *s;
    const char *s0 = "this is a test$HOME.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }
  {
    char *s;
    const char *s0 = "this is a test${MSH_XY_BASHPID}.db";

    s = mas_expand_string( s0 );
    fprintf( stderr, "@@@@@@@@@@@@@@@@ %s\t=> %s\n", s0, s );
    mas_free( s );
  }

  if ( 0 )
  {
    char *segfault = NULL;

    printf( "%c\n", *segfault );
  }

#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif


  return 0;
}
