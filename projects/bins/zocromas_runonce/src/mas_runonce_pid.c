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

#include "mas_runonce_config_types.h"

#include "mas_runonce_config.h"
#include "mas_runonce_test.h"
#include "mas_runonce_base.h"
#include "mas_runonce.h"

pids_t runonce_pids = {.size = 32767,.array = NULL };

static const char *
cbcmdline( pid_t pid, const char *line, const char *etext )
{
  /* printf( "CMD %s\n", line ); */
  /* pidarray[pid].cmdline = mas_strdup( line ); */
  runonce_pids.array[pid].argc = mas_add_argv_cmdline( runonce_pids.array[pid].argc, &runonce_pids.array[pid].argv, line, etext - line );
  return NULL;
}

static const char *
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
    runonce_pids.array[pid].progname = mas_strndup( pline, eline - pline );
    /* printf( "STATUS '%s'\n", runonce_pids.array[pid].progname ); */
  }
  if ( 0 == strncmp( ( const char * ) match2, ( const char * ) line, lmatch2 ) )
  {
    /* printf( "STATUS %s\n", line ); */
    sscanf( pline, "%u", ( unsigned * ) &runonce_pids.array[pid].ppid );
  }
  return eline;
}

static void
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

static int
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
runonce_pids_create( void )
{
  DIR *fproc;

  runonce_pids.array = mas_malloc( runonce_pids.size * sizeof( prec_t ) );
  memset( runonce_pids.array, 0, runonce_pids.size * sizeof( prec_t ) );

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

void
runonce_pids_delete( void )
{
  for ( unsigned i = 0; i < runonce_pids.size; i++ )
  {
    if ( runonce_pids.array[i].progname )
    {
      mas_free( runonce_pids.array[i].progname );
      runonce_pids.array[i].progname = NULL;
    }
    if ( runonce_pids.array[i].argv )
    {
      mas_del_argv( runonce_pids.array[i].argc, runonce_pids.array[i].argv, 0 );
      runonce_pids.array[i].argc = 0;
      runonce_pids.array[i].argv = NULL;
    }
  }
  mas_free( runonce_pids.array );
  runonce_pids.array = NULL;
}

void
runonce_pids_reset( void )
{
  runonce_pids_delete(  );
  runonce_pids_create(  );
}

int
runonce_pidof( pid_t * pids, size_t num, const char *name, const char *subname, const char *path, int argc, char **argv )
{
  size_t cnt = 0;

  for ( pid_t pid = 0; pid < runonce_pids.size; pid++ )
  {
    pid_t fpid = 0;

    if ( runonce_pids.array[pid].progname && name )
    {
      /* printf( "%d [%s:%s:%s]\n", __LINE__, name, subname, runonce_pids.array[pid].progname ); */
      /* 15 !!! */
      /* speech-dispatch(er) */
      /* notification-da(emon) */
      /* 123456789012345 */
      /* printf( "A %d [%s:%s:%s] %d\n", __LINE__, name, subname, runonce_pids.array[pid].progname, runonce_pids.array[pid].argc ); */
      if ( 0 == strncmp( name, runonce_pids.array[pid].progname, 15 ) )
      {
        /* printf( "B %d [%s:%s:%s] %d\n", __LINE__, name, subname, runonce_pids.array[pid].progname, runonce_pids.array[pid].argc ); */
        if ( subname )
        {
          if ( runonce_pids.array[pid].argc > 0 )
          {
            char *s;
            size_t slen;
            size_t sublen;

            s = runonce_pids.array[pid].argv[1];
            slen = strlen( s );
            sublen = strlen( subname );
            /* printf( ">%d [%s:%s:%s] %d [%s:%s:%s]\n", __LINE__, name, subname, runonce_pids.array[pid].progname,                         */
            /*         runonce_pids.array[pid].argc, runonce_pids.array[pid].argv[0], runonce_pids.array[pid].argv[1], s + slen - sublen ); */
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
        /* printf( "oooooooooooooooooo %s : %d\n", name, argc ); */
        if ( argc && argv )
        {
          for ( int i = 0; i < argc; i++ )
          {
            /* printf( "OOOO %s ? %s\n", argv[i], runonce_pids.array[pid].argv[i] ); */
            if ( 0 != strcmp( argv[i], runonce_pids.array[pid].argv[i] ) )
            {
              fpid = 0;
            }
          }
          /* compare args -- runonce_pids.array[pid].argv */
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
