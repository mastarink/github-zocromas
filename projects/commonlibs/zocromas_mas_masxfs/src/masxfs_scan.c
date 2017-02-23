#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

int
masxfs_scanpath_real( const char *path, masxfs_entry_callback_t * callbacks, unsigned long flags, size_t maxdepth )
{
  int r = 0;

  if ( !( flags & MASXFS_CB_RECURSIVE ) )
    DIE( "%s", "NOREC" );
  if ( !path )
    path = ".";                                                      /* getcwd */
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 );

    if ( !( flags & MASXFS_CB_RECURSIVE ) )
      DIE( "%s", "NOREC" );
    r = masxfs_pathinfo_scan( pi, callbacks, flags, maxdepth );
    masxfs_pathinfo_delete( pi );
  }
  return r;
}

int
masxfs_scanpath_real2( const char *cpath, masxfs_entry_callback_t * callbacks, unsigned long flags, size_t maxdepth )
{
  int r = 0;
  char *path = NULL;

  if ( !( flags & MASXFS_CB_RECURSIVE ) )
    DIE( "%s", "NOREC" );

  if ( cpath )
  {
    path = mas_strdup( cpath );
  }
  else
  {
    char *freepath = NULL;

    freepath = get_current_dir_name(  );
    path = mas_strdup( freepath );
    if ( freepath )
      free( freepath );
  }
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 );

    r = masxfs_pathinfo_scan( pi, callbacks, flags, maxdepth );
    masxfs_pathinfo_delete( pi );
  }
  mas_free( path );
  return r;
}
