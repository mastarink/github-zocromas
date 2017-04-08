#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/* #include <mastar/wrap/mas_std_def.h> */
#include <mastar/wrap/mas_memory.h>

#include "mas_arg_tools.h"
#include "mas_argvc_tools.h"

/* ###################################################################### */
#include "mas_utils_path.h"
/* ###################################################################### */

int
mas_pathdepth( const char *path )
{
  int r = 0;
  int depth = 0;
  const char *bd;

  bd = path;
  if ( path && *path )
  {
    while ( bd && *bd && *bd == '/' )
      bd++;
    depth = 0;
    while ( bd && *bd )
    {
      const char *ed;

      ed = bd;
      while ( ed && *ed && *ed != '/' )
        ed++;
      while ( ed && *ed && *ed == '/' )
        ed++;

      bd = ed;
      depth++;
    }
  }
/* else                                   */
/* {                                      */
/*   DUF_MAKE_ERROR( r, DUF_ERROR_PATH ); */
/* }                                      */
  return r < 0 ? r : depth;
}

char *
mas_realpath( const char *path /*, int *pr */  )
{
/* int r = 0; */
  char *real_path = NULL;
  char *rp = realpath( path, NULL );

/* DUF_TRACE( path, 2, ">> path:%s; rp:%s", path, rp ); */

  if ( rp )
  {
    real_path = mas_strdup( rp );
    free( rp );
  }
/* else                                   */
/* {                                      */
/*   DUF_MAKE_ERROR( r, DUF_ERROR_PATH ); */
/* }                                      */

/* if ( pr )  */
/*   *pr = r; */
  return real_path;
}

/* 20151007.135909 */
/* allocates! */
char *
mas_concat_path( const char *path, const char *subpath )
{
  char *npath = NULL;

  if ( path && subpath && *path && *subpath )
  {
    npath = mas_normalize_path( path );
    npath = mas_strcat_x( npath, subpath );
  }
  return npath;
}

/* 20151021.185047 */
char *
mas_vnormalize_path_wd( const char *path, const char *cwd, va_list args )
{
  char *npath = NULL;

  if ( path && *path )
  {
#if 0
    size_t len;

    len = strlen( path );
    if ( path[len - 1] != '/' )
    {
      npath = mas_strdup( path );
      npath = mas_strcat_x( npath, "/" );
    }
    else
    {
      while ( path[len - 1] == '/' )
        len--;
      npath = mas_strndup( path, len + 1 );
    }
#else
    const char *ppath;
    const char *sl;

    if ( *path != '/' && cwd )
    {
      npath = mas_normalize_path( cwd );
    }
    ppath = path;
  /* npath = mas_strdup( "" ); */
    while ( ( sl = strchr( ppath, '/' ) ) )
    {
      npath = mas_strncat_x( npath, ppath, sl - ppath );
      npath = mas_strcat_x( npath, "/" );
      ppath = sl + 1;
      while ( *ppath == '/' )
        ppath++;
    }
    if ( ppath && *ppath /* && !( ppath[0] == '.' && ppath[1] == '\0' ) */  )
    {
      npath = mas_strcat_x( npath, ppath );
      npath = mas_strcat_x( npath, "/" );
    }
    {
      const char *s = NULL;
      while ( ( s = va_arg( args, const char * ) ) )
      {
        char *st = NULL;

        st = mas_normalize_path( s );
        npath = mas_strcat_x( npath, st );
        mas_free( st );
      }
    }
#endif
  }
  return npath;
}

/* 20151021.185044 */

char *
mas_normalize_path_plus( const char *path, ... )
{
  char *rpath = NULL;
  va_list args;

  {
    va_start( args, path );
    rpath = mas_vnormalize_path_wd( path, NULL, args );
    va_end( args );
  }
  return rpath;
}

char *
mas_normalize_path_plus_wd( const char *path, const char *cwd, ... )
{
  char *rpath = NULL;
  va_list args;

  {
    va_start( args, cwd );
    rpath = mas_vnormalize_path_wd( path, cwd, args );
    va_end( args );
  }
  return rpath;
}

/* 20151021.185042 */
/* allocates! */
char *
mas_normalize_path( const char *path )
{
  return mas_normalize_path_plus( path, NULL );
}

char *
mas_normalize_path_wd( const char *path, const char *cwd )
{
  return mas_normalize_path_plus_wd( path, cwd, NULL );
}

char *
mas_normalize_path_cwd( const char *path )
{
  char buffer[PATH_MAX + 1];

  return mas_normalize_path_wd( path, getcwd( buffer, sizeof( buffer ) - 1 ) );
}

char *
mas_remove_path_dots( const char *path )
{
  char *npath = NULL;
  int done = 0;
  mas_argvc_t targ = { 0 };

  mas_add_argvc_args_d( &targ, path, 0, "/" );

  do
  {
    done = 0;
    for ( int i = 0; i < targ.argc; i++ )
    {
      if ( 0 == strcmp( targ.argv[i], "." ) )
      {
        done++;
        mas_free( targ.argv[i] );
        targ.argv[i] = NULL;
      }
      else if ( i > 0 && targ.argv[i] && targ.argv[i - 1] && targ.argv[i - 1][0] && 0 == strcmp( targ.argv[i], ".." )
                && 0 != strcmp( targ.argv[i - 1], ".." ) )
      {
        done++;
        mas_free( targ.argv[i - 1] );
        targ.argv[i - 1] = NULL;
        mas_free( targ.argv[i] );
        targ.argv[i] = NULL;
      }
    }
    {
      int d = 0;

      for ( int i = 0; i < targ.argc; i++ )
      {
        if ( d != i )
        {
          targ.argv[d] = targ.argv[i];
          done++;
        }
        if ( targ.argv[i] )
          d++;
      }
      targ.argc = d;
    }
  } while ( done );
  npath = mas_argvc_join( &targ, 0, '/' );
  mas_argvc_delete( &targ );
  return npath;
}

char *
mas_normalize_path_dots( const char *path )
{
  char *npath = NULL;
  char *p = mas_normalize_path( path );

  npath = mas_remove_path_dots( p );
  mas_free( p );
  return npath;
}

char *
mas_normalize_path_cwd_dots( const char *path )
{
  char *npath = NULL;
  char *p = mas_normalize_path_cwd( path );

  npath = mas_remove_path_dots( p );
  mas_free( p );
  return npath;
}
