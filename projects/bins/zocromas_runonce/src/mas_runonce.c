#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>
#include "mas_runonce.h"

static prec_t *pidarray;
static size_t pidarray_size = 32767;

const char *
cbstatus( pid_t pid, const char *line, const char *etext )
{
  const char *pline;
  const char *eline;
  const char *match1 = "Name:";
  unsigned lmatch1 = strlen( match1 );
  const char *match2 = "PPid:";
  unsigned lmatch2 = strlen( match2 );

  pline = line;
  while ( *pline && *pline != ':' )
    pline++;
  pline++;
  while ( *pline && *pline <= ' ' )
    pline++;
  eline = pline;
  while ( eline && *eline && *eline != '\n' && *eline != '\r' )
    eline++;
  while ( eline && *eline && eline < eline && ( *eline == '\n' || *eline == '\r' ) )
    eline++;
  if ( 0 == strncmp( ( const char * ) match1, ( const char * ) line, lmatch1 ) )
  {
    pidarray[pid].progname = mas_strndup( pline, eline - pline );
    /* printf( "STATUS '%s'\n", pidarray[pid].progname ); */
  }
  if ( 0 == strncmp( ( const char * ) match2, ( const char * ) line, lmatch2 ) )
  {
    /* printf( "STATUS %s\n", line ); */
    sscanf( pline, "%u", ( unsigned * ) &pidarray[pid].ppid );
  }
  return eline;
}

const char *
cbcmdline( pid_t pid, const char *line, const char *etext )
{
  /* printf( "CMD %s\n", line ); */
  /* pidarray[pid].cmdline = mas_strdup( line ); */
  pidarray[pid].argc = mas_add_argv_cmdline( pidarray[pid].argc, &pidarray[pid].argv, line, etext - line );
  return NULL;
}

void
read_pid_file( pid_t pid, const char *name, fun_t cb )
{
  FILE *f;
  char path[512];
  char *buff;
  unsigned bufsz = 1024 * 10;
  int l = 0;

  buff = mas_malloc( bufsz );
  sprintf( path, "/proc/%u/%s", pid, name );
  f = fopen( path, "r" );
  if ( f )
  {
    l = fread( buff, 1, bufsz, f );
    do
    {
      l--;
      buff[l] = 0;
    }
    while ( buff[l - 1] && buff[l - 1] <= ' ' && l > 0 );
    if ( l > 0 )
    {
      const char *zbuff;
      const char *bline;

      zbuff = buff + l;
      bline = buff;
      while ( bline && bline < zbuff )
      {
        if ( cb )
          bline = ( cb ) ( pid, bline, bline + l );
      }
    }
    fclose( f );
  }
  mas_free( buff );
}

int
init_pid( const char *name )
{
  pid_t pid = 0;

  sscanf( name, "%u", ( unsigned * ) &pid );
  {
    read_pid_file( pid, "status", cbstatus );
    read_pid_file( pid, "cmdline", cbcmdline );
  }
  return 0;
}

int
init( void )
{
  DIR *fproc;

  fproc = opendir( "/proc" );
  if ( fproc )
  {
    /* int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result); */
    struct dirent de;
    struct dirent *pde;

    while ( 0 == readdir_r( fproc, &de, &pde ) && pde )
    {
      int isnum = 1;

      for ( const char *p = pde->d_name; *p; p++ )
      {
        if ( !isdigit( *p ) )
        {
          isnum = 0;
          break;
        }
      }
      if ( isnum )
      {
        init_pid( pde->d_name );
      }
    }
    closedir( fproc );
  }
  return 0;
}

int
pidof( pid_t * pids, size_t num, const char *name, const char *subname, const char *path )
{
  size_t cnt = 0;

  for ( pid_t pid = 0; pid < pidarray_size; pid++ )
  {
    pid_t fpid = 0;

    if ( pidarray[pid].progname )
    {
      /* printf( "%d [%s]\n", __LINE__, name ); */
      if ( name && 0 == strcmp( name, pidarray[pid].progname ) )
      {
        if ( subname )
        {
          if ( pidarray[pid].argc > 0 )
          {
            char *s;
            size_t slen;
            size_t sublen;

            s = pidarray[pid].argv[1];
            slen = strlen( s );
            sublen = strlen( subname );
            if ( 0 == strcmp( subname, s + slen - sublen ) )
            {
              if ( path )
              {
                size_t pathlen;

                pathlen = strlen( path );
                if ( 0 == strncmp( path, s, pathlen ) )
                {
                  fpid = pid;
                }
                else
                {
                }
              }
              else
              {
                fpid = pid;
              }
            }
          }
        }
        else
        {
          fpid = pid;
        }
      }
    }
    if ( cnt < num && fpid )
    {
      *pids++ = fpid;
      cnt++;
    }
  }
  return cnt;
}

int
main( int argc, char *argv[] )
{
  pidarray = mas_malloc( pidarray_size * sizeof( prec_t ) );
  memset( pidarray, 0, pidarray_size * sizeof( prec_t ) );
  for ( int i = 0; i < argc; i++ )
  {
    printf( "%d. %s\n", i, argv[i] );
  }
  init(  );








  /* for ( unsigned i = 0; i < pidarray_size; i++ )                                                             */
  /* {                                                                                                  */
  /*   if ( pidarray[i].progname )                                                                      */
  /*   {                                                                                                */
  /*     printf( "%u@%u: [%s(%s)]\n", i, pidarray[i].ppid, pidarray[i].progname, pidarray[i].cmdline ); */
  /*   }                                                                                                */
  /* }                                                                                                  */

  printf( "--------------------------------------------------------------------------------\n" );
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2", NULL, NULL );
    printf( " python2 n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "/usr/bin/python2", NULL, NULL );
    printf( " /usr/bin/python2 n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", NULL, NULL );
    printf( " python2.7 n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "/usr/bin/python2.7", NULL, NULL );
    printf( " /usr/bin/python2.7 n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2", "guake", NULL );
    printf( " guake n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2", "guake", "/usr/bin" );
    printf( " guake n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "sonata", NULL, NULL );
    printf( " sonata n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "apache2", NULL, NULL );
    printf( " apache2 n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "screengrab", NULL, NULL );
    printf( " screengrab n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "pamusb-agent", NULL, NULL );
    printf( " pamusb-agent n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", "pamusb-agent", NULL );
    printf( " pamusb-agent n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", "pamusb-agent", "/usr/bin/" );
    printf( " pamusb-agent n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "calibre-server", NULL, NULL );
    printf( " calibre-server n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }
  {
    pid_t pids[512];
    int n;

    n = pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", "calibre-server", "/usr/bin/" );
    printf( " calibre-server n:%d\n", n );
    for ( int i = 0; i < n; i++ )
      printf( "%d: %u\n", i, pids[i] );
    printf( "--------------------------------------------------------------------------------\n" );
  }




  for ( unsigned i = 0; i < pidarray_size; i++ )
  {
    if ( pidarray[i].progname )
    {
      mas_free( pidarray[i].progname );
      pidarray[i].progname = NULL;
    }
    if ( pidarray[i].argv )
    {
      mas_del_argv( pidarray[i].argc, pidarray[i].argv, 0 );
      pidarray[i].argc = 0;
      pidarray[i].argv = NULL;
    }
  }
  mas_free( pidarray );
  pidarray = NULL;

#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif


  return 0;
}
