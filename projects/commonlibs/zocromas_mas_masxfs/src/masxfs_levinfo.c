#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo.h"

static char *
normalize_path( const char *path, const char *name )
{
  char *npath = NULL;

  if ( path )
  {
    size_t plen = 0, nlen = 0, len = 0;

    plen = strlen( path );
    if ( name )
      nlen = strlen( name );
    len = plen + nlen;
    if ( len > 0 )
    {
      npath = mas_malloc( len + 2 /* for 0 and possible '/'  */  );
      {
        const char *p = path;
        char *np = npath;

        while ( p && *p )
        {
          char c;

          *np++ = c = *p++;
          while ( c == '/' && *p == '/' )
            p++;
        }
        *np = 0;
        if ( name )
        {
          if ( np[-1] != '/' )
            *np++ = '/';
          *np = 0;
          strcat( npath, name );
        }
      }
    }
  }
  return npath;
}

masxfs_levinfo_t *
masxfs_levinfo_path2levnfo( const char *path, size_t max_depth, masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_t *levinfo = NULL;

  char *spath = normalize_path( path, NULL );

  if ( spath && *spath )
  {
    char *stok, *ep;
    char *spatht = spath;
    int levinfo_depth = 0;

    levinfo = masxfs_levinfo_create_array_setup( max_depth, pi );

    while ( ( stok = strtok_r( spatht, "/", &ep ) ) )
    {
      masxfs_levinfo_init( levinfo + levinfo_depth, stok, path );
      fprintf( stderr, "TOK:%s\n", stok );
      spatht = NULL;
      levinfo_depth++;
    }
    pi->depth = levinfo_depth;
  }
  mas_free( spath );
  return levinfo;
}

char *
masxfs_levinfo_right_realpath( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_realpath( li - li->depth, li->depth );
}

char *
masxfs_levinfo_realpath( masxfs_levinfo_t * lia, size_t depth )
{
  size_t len = 0;
  char *path = NULL;

  if ( lia && depth )
  {
    for ( size_t i = 0; i < depth; i++ )
    {
      len += strlen( lia[i].name );
      len++;                                                         /* '/' */
    }
    len += 2;
    path = mas_malloc( len );
    {
      char *p = path;

      for ( size_t i = 0; i < depth; i++ )
      {
        *p++ = '/';
        strcpy( p, lia[i].name );
        while ( p && *p )
          p++;
      }
    }
  }
  return path;
}
