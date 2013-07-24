#include <mastar/wrap/mas_std_def.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include <mastar/types/mas_control_types.h> */
/* #include <mastar/types/mas_opts_types.h>    */
/* extern mas_control_t ctrl;                  */

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel_open.h>
#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel.h>

/* #include "mas_common.h" */
/* #include <mastar/msg/mas_msg_def.h>   */
/* #include <mastar/msg/mas_msg_tools.h> */

void
testprint( int targc, char *targv[] )
{
  fprintf( stderr, ">>>>>>>>>> for (ia=0;ia < %d; ia++)\n", targc );
  for ( int ia = 0; ia < targc; ia++ )
  {
    fprintf( stderr, "%2d. '%s'\n", ia, targv[ia] );
  }
  fprintf( stderr, ">>>>>>>>>> while ( p )\n" );
  {
    char **p;
    int ia = 0;

    p = targv;
    while ( p && *p )
    {
      fprintf( stderr, "%2d. '%s'\n", ia, *p );
      ia++;
      p++;
    }
  }
}

void
test20130100( int argc, char *argv[], char *env[] )
{
  fprintf( stderr, "\n== A =====================\n" );
  {
    char **targv = NULL;
    int targc = 0;

    /* testprint( argc, argv ); */


    targc = mas_add_argv_arg( targc, &targv, "test 1" );
    targc = mas_add_argv_arg( targc, &targv, "test 2" );
    targc = mas_add_argv_argv( targc, &targv, argc, argv, 1 );
    testprint( targc, targv );
    targc = mas_add_argv_args( targc, &targv, "asta maniana x y	-r \"  \" \"u v w\"", 0 );
#ifdef MAS_TRACEMEM
    /* print_memlist( FL, stderr ); */
#endif
    testprint( targc, targv );

    fprintf( stderr, "\ntargc:%u; targv:%p\n", targc, ( void * ) targv );
    mas_del_argv( targc, targv, 0 );
#ifdef MAS_TRACEMEM
    print_memlist( __func__, __LINE__, stderr );
#endif
  }
  fprintf( stderr, "\n== B =====================\n" );
  {
    int targc = 0;
    char **targv = NULL;

    targc = mas_add_argv_args( targc, &targv, "zocromas.mastar.lan 5003", 0 );
    fprintf( stderr, "\ntargc:%u;\n", targc );

    mas_del_argv( targc, targv, 0 );

#ifdef MAS_TRACEMEM
    print_memlist( __func__, __LINE__, stderr );
#endif
  }
}

void
test20130120( void )
{
  struct timeval tv1;
  struct timeval tv2;

  /* fprintf( stderr, "\n-------------------------------------\n%lu.%06lu\n", tv1.tv_sec, tv1.tv_usec ); */
  for ( int ima2 = 0; ima2 < 10; ima2++ )
  {
    gettimeofday( &tv1, 0 );
    for ( int ima = 0; ima < 100000; ima++ )
    {
      char *p[100];

      for ( int j = 0; j < sizeof( p ) / sizeof( p[0] ); j++ )
        p[j] = malloc( 10000 );
      for ( int j = 0; j < sizeof( p ) / sizeof( p[0] ); j++ )
        free( p[j] );
    }
    gettimeofday( &tv2, 0 );
    /* fprintf( stderr, "%lu.%06lu\n", tv2.tv_sec, tv2.tv_usec ); */
    {
      tv2.tv_sec -= tv1.tv_sec;
      tv2.tv_usec -= tv1.tv_usec;
      if ( tv2.tv_usec < 0 )
      {
        tv2.tv_sec -= 1;
        tv2.tv_usec += 1000000;
      }
    }
    fprintf( stderr, "%lu.%06lu\n", tv2.tv_sec, tv2.tv_usec );
  }
}


void
test_readline( const char *to_read, const char *fdc_pathz )
{
  int r = 0;
  mas_channel_t *pchannel;
  char *fdc_path = mas_strdup( fdc_pathz );

  /* test20130100( argc, argv, env ); */
  /* test20130120(  ); */
  pchannel = mas_channel_create(  );
  if ( !( r < 0 ) )
    r = mas_channel_init( pchannel, 0, CHN_RFILE, to_read, 0, 0 );
  FMSG( "(%d) CHANNEL_INIT", r );
  if ( !( r < 0 ) )
    r = mas_channel_open( pchannel );
  FMSG( "(%d) CHANNEL_OPEN", r );
#if 0
  {
    size_t sz = 0;
    char *buffer;

    for ( int ic = 0; ic < 5; ic++ )
    {
      if ( !( r < 0 ) )
      {
        r = mas_channel_read_some( pchannel );
      }
      FMSG( "(%d) CHANNEL_READ : %lu", r, ( unsigned long ) sz );
    }
    mas_channel_buffer_strip( pchannel, 100, 0 );
    buffer = mas_channel_buffer( pchannel, &sz, 0 );
    FMSG( "Buffer[%lu]", ( unsigned long ) sz );
    fputs( buffer, stdout );
  }
#elseif 0
  {
    size_t sz = 0;
    int cnt = 0;

    while ( !mas_channel_buffer_eof( pchannel ) )
    {
      const char *s;
      char *sc;
      int peof;

      sc = NULL;
      peof = mas_channel_buffer_eof( pchannel );
      s = mas_channel_buffer_nl( pchannel, &sz );
      if ( s )
      {
        sc = mas_strndup( s, sz );
      }
      fprintf( stdout, "%d. [%02x:%d:%d] [%ld] {%s}\n", cnt, ( unsigned char ) ( s ? *s : '\xff' ), peof,
               mas_channel_buffer_eof( pchannel ), ( unsigned long ) sz, sc );
      mas_free( sc );
      cnt++;
    }
  }
#elseif 0
  {
    size_t sz = 0;
    const char *s;
    int cnt = 0;

    /* mas_channel_buffer_set_maxread( pchannel, 256 ); */
    while ( cnt < 10000 && ( s = mas_channel_buffer_nl( pchannel, &sz ) ) )
    {
      char *sc;

      if ( cnt == 20 )
      {
        mas_channel_buffer_strip( pchannel, pchannel->buffer.size / 2, 0 );
      }
      sc = mas_strndup( s, sz );
      fputs( sc, stdout );
      mas_free( sc );
      cnt++;
    }
    /* fputs( "--------------------------------------------------------\n", stdout ); */
    /* fputs( pchannel->buffer.buffer, stdout ); */
  }
#elseif 0
  {
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    /* mas_channel_read_remainder( pchannel );    */
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
    mas_channel_read_some( pchannel );
    FMSG( "BF %lu %d", pchannel->buffer.length, mas_channel_buffer_feof( pchannel ) );
  }
#elseif 0
  {
    int cnt = 0;
    char *pstring = NULL;
    ssize_t l;

    l = strlen( fdc_path ) - 5;

    FMSG( "%ld fdc_path:%s", l, fdc_path );
    fdc_path[l] = 'a';
    FMSG( "fdc_path:%s", fdc_path );
    mas_channel_set_buffer_copy( pchannel, fdc_path );
    do
    {
      pstring = mas_channel_buffer_nl_dup( pchannel );
      /* FMSG( "[%d] p: {%s}", mas_channel_buffer_eof( pchannel ), pstring ); */

      if ( cnt == 26 )
      {
        fdc_path[l] = 'b';
        FMSG( "fdc_path:%s", fdc_path );
        mas_channel_set_buffer_copy( pchannel, fdc_path );
      }
      if ( cnt == 70 )
      {
        fdc_path[l] = 'c';
        FMSG( "fdc_path:%s", fdc_path );
        mas_channel_set_buffer_copy( pchannel, fdc_path );
      }
      mas_free( pstring );
      cnt++;
    }
    while ( pstring );
  }
#elseif 0
  {
    const char *s = "\r\n-----------------------------18235086615837691481974917453";
    size_t len;
    size_t offset;

    offset = 2;
    len = strlen( s );
    mas_channel_set_buffer_copy( pchannel, "/tmp/part1.tmp" );
    {
      const char *f;
      char *f1;

      f1 = NULL;
      f = mas_channel_search( pchannel, s + offset, len - offset, NULL, NULL );
      if ( f )
      {
        char *pstring;

        f1 = mas_strndup( f, 50 );
        FMSG( "NEEDLE:'%s'", s );
        FMSG( "1 FOUND:%s ...", f1 );
        FMSG( "IPTR:%lu", pchannel->buffer.iptr );
        mas_free( f1 );
        mas_channel_set_buffer_copy( pchannel, NULL );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        FMSG( "PS:'%s'", pstring );
        mas_free( pstring );
        if ( 1 )
        {
          int b = 0;

          do
          {
            pstring = mas_channel_buffer_nl_dup( pchannel );
            FMSG( ">>>>>>>> PS:'%s'", pstring );
            b = ( pstring && *pstring != '\n' && *pstring != '\r' ? 1 : 0 );
            mas_free( pstring );
            pstring = NULL;
          }
          while ( b );
        }

        mas_channel_set_buffer_copy( pchannel, "/tmp/part2.tmp" );
      }

    }
    offset = 0;
    {
      const char *f;
      char *f1;

      f1 = NULL;
      f = mas_channel_search( pchannel, s + offset, len - offset, NULL, NULL );
      if ( f )
      {
        char *pstring;

        f1 = mas_strndup( f, 50 );
        FMSG( "NEEDLE:'%s'", s );
        FMSG( "2 FOUND:%s ...", f1 );
        FMSG( "IPTR:%lu", pchannel->buffer.iptr );
        mas_free( f1 );
        mas_channel_set_buffer_copy( pchannel, NULL );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        mas_free( pstring );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        FMSG( "PS:'%s'", pstring );
        mas_free( pstring );
        if ( 1 )
        {
          int b = 0;

          do
          {
            pstring = mas_channel_buffer_nl_dup( pchannel );
            FMSG( ">>>>>>>> PS:'%s'", pstring );
            b = ( pstring && *pstring != '\n' && *pstring != '\r' ? 1 : 0 );
            mas_free( pstring );
          }
          while ( b );
        }
        mas_channel_set_buffer_copy( pchannel, "/tmp/part3.tmp" );
      }
    }
    offset = 0;
    {
      const char *f;
      char *f1;

      f1 = NULL;
      f = mas_channel_search( pchannel, s + offset, len - offset, NULL, NULL );
      if ( f )
      {
        char *pstring;

        f1 = mas_strndup( f, 50 );
        FMSG( "NEEDLE:'%s'", s );
        FMSG( "3 FOUND:%s ...", f1 );
        FMSG( "IPTR:%lu", pchannel->buffer.iptr );
        mas_free( f1 );
        mas_channel_set_buffer_copy( pchannel, NULL );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        mas_free( pstring );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        FMSG( "PS:'%s'", pstring );
        mas_free( pstring );
        if ( 1 )
        {
          int b = 0;

          do
          {
            pstring = mas_channel_buffer_nl_dup( pchannel );
            FMSG( ">>>>>>>> PS:'%s'", pstring );
            b = ( pstring && *pstring != '\n' && *pstring != '\r' ? 1 : 0 );
            mas_free( pstring );
          }
          while ( b );
        }
        mas_channel_set_buffer_copy( pchannel, "/tmp/part4.tmp" );
      }
      else
      {
        FMSG( "3 NOT FOUND ..." );
      }
    }
  }
#else
  {
    const char *s = "\r\n-----------------------------18235086615837691481974917453";
    size_t len;
    size_t offset;
    int np;

    np = 0;
    offset = 2;
    len = strlen( s );
    while ( 1 )
    {
      char cname[512];
      const char *f;
      char *f1;

      f1 = NULL;
      snprintf( cname, sizeof( cname ), "/tmp/part%d.tmp", np++ );
      mas_channel_set_buffer_copy( pchannel, cname );
      FMSG( "FFF %s", cname );
      f = mas_channel_search( pchannel, s + offset, len - offset, NULL, NULL );
      if ( f )
      {
        char *pstring;

        f1 = mas_strndup( f, len + 2 );
        /* FMSG( "NEEDLE:'%s'", s ); */
        /* FMSG( "%d FOUND:%s ...[[ %02x %02x %02x ]]", np, f1, f[len - offset], f[len - offset + 1], f[len - offset + 2] ); */
        mas_free( f1 );
        mas_channel_set_buffer_copy( pchannel, NULL );
        if ( f[len - offset] == '-' && f[len - offset + 1] == '-' )
          break;
        /* FMSG( "IPTR:%lu", pchannel->buffer.iptr ); */
        FMSG( "ZZZ" );
        pstring = mas_channel_buffer_nl_dup( pchannel );
        FMSG( "PS:'%s'", pstring );
        mas_free( pstring );
        {
          int b = 0;

          do
          {
            pstring = mas_channel_buffer_nl_dup( pchannel );
            FMSG( "PS PLUS:'%s'", pstring );
            b = ( pstring && *pstring != '\n' && *pstring != '\r' ? 1 : 0 );
            mas_free( pstring );
            pstring = NULL;
          }
          while ( b );
        }
      }
      offset = 0;
    }
  }
#endif
  mas_channel_delete( pchannel, 1, 1 );
  mas_free( fdc_path );
}

int
main( int argc, char *argv[], char *env[] )
{
/* /home/mastar/.topvars      */
/* /home/mastar/projects.list */


  /* test_readline( "/tmp/wss.tmp", "/tmp/wss1a.tmp" ); */
  /* test_readline( "/mnt/new_misc/develop/autotools/zoc/projects/zocromas/test/13984-2-1362637472-143.dat", "/tmp/fdcopy143.tmp" ); */
  /* test_readline( "/mnt/new_misc/develop/autotools/zoc/zocromas/test1.txt", "/tmp/fdcopy1a.tmp" ); */
  test_readline( "/mnt/new_misc/develop/autotools/zoc/zocromas/test2.txt", "/tmp/fdcopy2a.tmp" );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
}
