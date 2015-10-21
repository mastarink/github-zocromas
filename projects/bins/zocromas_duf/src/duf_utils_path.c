#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>

/* #include <mastar/wrap/mas_std_def.h> */
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>


/* ###################################################################### */
#include "duf_utils_path.h"
/* ###################################################################### */

int
duf_pathdepth( const char *path )
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
duf_realpath( const char *path /*, int *pr */  )
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
duf_concat_path( const char *path, const char *subpath )
{
  char *npath = NULL;

  if ( path && subpath && *path && *subpath )
  {
    npath = duf_normalize_path( path );
    npath = mas_strcat_x( npath, subpath );
  }
  return npath;
}

/* 20151021.185047 */
char *
duf_vnormalize_path( const char *path, va_list args )
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
    if ( ppath && *ppath )
    {
      npath = mas_strcat_x( npath, ppath );
      npath = mas_strcat_x( npath, "/" );
    }
    {
      const char *s = NULL;
      while ( ( s = va_arg( args, const char * ) ) )
      {
        char *st = NULL;

        st = duf_normalize_path( s );
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
duf_normalize_path_plus( const char *path, ... )
{
  char *rpath = NULL;
  va_list args;

  {
    va_start( args, path );
    rpath = duf_vnormalize_path( path, args );
    va_end( args );
  }
  return rpath;
}

/* 20151021.185042 */
/* allocates! */
char *
duf_normalize_path( const char *path )
{
  return duf_normalize_path_plus( path, NULL );
}
