#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>
/* #include <stdlib.h> */
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>
/* #include <mastar/levinfo/masxfs_levinfo_structs.h> */
#include <mastar/levinfo/masxfs_levinfo_tools.h>

#include "masxfs_pathinfo_base.h"
#include "masxfs_pathinfo.h"

#include "masxfs_scan.h"

/*
exiternal functions used:

 get_current_dir_name

*/

int
masxfs_scanpath_real( const char *path, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * callbacks, void *userdata, masxfs_levinfo_flags_t flags,
                      masxfs_depth_t maxdepth )
{
  int r = 0;

  if ( !( flags & MASXFS_CB_RECURSIVE ) )
    DIE( "%s", "NOREC" );
  if ( !path )
    path = ".";                                                      /* getcwd */
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128, 0 );

    r = masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, userdata, flags, maxdepth );
    masxfs_pathinfo_delete( pi, flags );
  }
  return r;
}

int
masxfs_scanpath_real2( const char *cpath, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * callbacks, void *userdata, masxfs_levinfo_flags_t flags,
                       masxfs_depth_t maxdepth )
{
  int r = 0;
  char *path = NULL;

  if ( cpath )
  {
    path = mas_strdup( cpath );
  }
  else
  {
    char *freepath = NULL;

/* memory control trick */
    freepath = get_current_dir_name(  );
    path = mas_strdup( freepath );
    if ( freepath )
      free( freepath );
  }
  if ( path )
  {
    masxfs_pathinfo_t *pi = masxfs_pathinfo_create_setup( path, 128, flags );

    r = masxfs_pathinfo_scan_cbs( pi, typeflags, callbacks, userdata, flags, maxdepth );
    masxfs_pathinfo_delete( pi, flags );
  }
  mas_free( path );
  return r;
}
