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

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo.h"

masxfs_levinfo_t *
masxfs_levinfo_path2levnfo( const char *path, size_t max_depth, size_t * pdepth )
{
  char *stok, *ep;
  char *spath = mas_strdup( path );
  char *spatht = spath;
  int levinfo_depth = 0;

  masxfs_levinfo_t *levinfo = masxfs_levinfo_create_array( max_depth );

  while ( ( stok = strtok_r( spatht, "/", &ep ) ) )
  {
    masxfs_levinfo_init( levinfo + levinfo_depth, stok, path );
    fprintf( stderr, "TOK:%s\n", stok );
    spatht = NULL;
    levinfo_depth++;
  }
  if ( pdepth )
    *pdepth = levinfo_depth;
  mas_free( spath );
  return levinfo;
}

char *
masxfs_levinfo_realpath( masxfs_levinfo_t * lia, size_t depth )
{
  size_t len = 0;
  char *path = NULL;

  for ( size_t i = 0; i < depth; i++ )
  {
    len += strlen( lia[i].name );
    len++;                                                           /* '/' */
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
  return path;
}
