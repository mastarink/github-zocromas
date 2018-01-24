#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

/* #include "masxfs_levinfo_db.h" */
#include "masxfs_levinfo_db_dir.h"

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
  rDECLGOOD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_rewinddir( li ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_rewinddir( li ) );
  rRET;
}

int
masxfs_levinfo_opened_dir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int def )
{
  int f = 0;

  if ( flags & MASXFS_CB_MODE_FS )
    f = masxfs_levinfo_fs_opened_dir( li );
  if ( flags & MASXFS_CB_MODE_DB )
    f = masxfs_levinfo_db_opened_dir( li );
  if ( !( flags & ( MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB ) ) )
    f = def;
  return f;
}

int
masxfs_levinfo_closedir_force( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int rcode )
{
  rDECLGOOD;
  int rc = 0;

  rCODE = rcode;
  rc = masxfs_levinfo_closedir( li, flags );                         /* sic! */
  if ( rGOOD )
    rCODE = rc;
  rRET;
}

int
masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, masxfs_entry_filter_t * entry_pfilter )
{
  rDECLGOOD;
  assert( !masxfs_levinfo_opened_dir( li, flags, 0 ) );

  if ( flags & MASXFS_CB_MODE_FS )
  {
    rC( masxfs_levinfo_fs_opendir( li ) );
  /* WARN("[%d] %s", li->lidepth, li->name); */
    assert( masxfs_levinfo_opened_dir( li, flags, 1 ) );
  }
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_opendir( li, entry_pfilter ) );
  QRLI( li, rCODE );
/* assert(  (*any mode *) flags & ( MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB ) ); */
  assert( rBAD || masxfs_levinfo_opened_dir( li, flags, 1 ) );
  rRET;
}

/* flags: only MASXFS_CB_MODE_FS or/and MASXFS_CB_MODE_DB used */
int
masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLGOOD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_closedir( li ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_closedir( li ) );
/* assert(  (*any mode *) flags & ( MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB ) ); */
  rRET;
}

int
masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
#if 1
  rDECLGOOD;
  while ( li->lidepth && rGOOD )
  {
    rC( masxfs_levinfo_closedir( li, flags ) );
    li--;
  }
#else
  rDECLBAD;
  do
  {
    rC( masxfs_levinfo_closedir( li, flags ) );
    if ( !li->lidepth )
      break;
    li--;
  } while ( rGOOD );
#endif
  rRET;
}

int
masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_levinfo_flags_t flags, int *phas_data )
{
  rDECLGOOD;

  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_readdir( li, entry_pfilter, phas_data ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_readdir( li, entry_pfilter, phas_data ) );
  rRET;
}
