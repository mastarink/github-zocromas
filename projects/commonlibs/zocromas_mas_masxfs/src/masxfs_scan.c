#include <stdio.h>
#include <string.h>

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

int
masxfs_scanpath_real( const char *path, masxfs_entry_callback_t * callbacks, int recursive )
{
  int r = 0;

  if ( !path )
    path = ".";
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128 );

    r = masxfs_pathinfo_scan( pi, callbacks, recursive );
    masxfs_pathinfo_delete( pi );
  }
  return r;
}
