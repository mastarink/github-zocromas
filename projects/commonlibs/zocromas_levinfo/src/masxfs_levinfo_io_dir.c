#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_db.h"

#include "masxfs_levinfo_fs_dir.h"
#include "masxfs_levinfo_io_dir.h"

/*
exiternal functions used:

 fdopendir
 closedir
 readdir
 rewinddir
*/

int
masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_rewinddir( li, flags ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_rewinddir( li, flags ) );
  rRET;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_opendir( li, flags ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_opendir( li, flags ) );
  rRET;
}

int
masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_closedir( li ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_closedir( li ) );
  rRET;
}

int
masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  do
  {
    rC( masxfs_levinfo_closedir( li, flags ) );
    if ( !li->lidepth )
      break;
    li--;
  } while ( rGOOD );

  rRET;
}

int
masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int *phas_data )
{
  rDECLBAD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_readdir( li, flags, phas_data ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_readdir( li, flags, phas_data ) );
  rRET;
}
