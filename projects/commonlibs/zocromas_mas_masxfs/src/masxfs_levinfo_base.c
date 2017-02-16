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
masxfs_levinfo_create_array( size_t sz )
{
  size_t size = sz * sizeof( masxfs_levinfo_t );
  masxfs_levinfo_t *li = mas_malloc( size );

  memset( li, 0, size );
  return li;
}

masxfs_levinfo_t *
masxfs_levinfo_create_array_setup( size_t sz, masxfs_pathinfo_t * pi )
{
  masxfs_levinfo_t *lia = masxfs_levinfo_create_array( sz );

  for ( size_t i = 0; i < sz; i++ )
  {
    lia[i].pi = pi;
    lia[i].depth = i;
  }
  return lia;
}

masxfs_levinfo_t *
masxfs_levinfo_create( void )
{
  return masxfs_levinfo_create_array( 0 );
}

void
masxfs_levinfo_init( masxfs_levinfo_t * li, const char *name, const char *path _uUu_ )
{
  if ( li )
    li->name = mas_strdup( name );
}

void
masxfs_levinfo_close( masxfs_levinfo_t * li )
{
  if ( li && li->name )
    mas_free( li->name );
}

void
masxfs_levinfo_close_array( masxfs_levinfo_t * lia, size_t sz )
{
  if ( lia )
  {
    for ( size_t il = 0; il < sz && ( lia + il )->name; il++ )
    {
      masxfs_levinfo_close( lia + il );
    }
  }
}

void
masxfs_levinfo_delete_array( masxfs_levinfo_t * li, size_t sz )
{
  if ( li )
  {
    masxfs_levinfo_close_array( li, sz );
    mas_free( li );
  }
}
