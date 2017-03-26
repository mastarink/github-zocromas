#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_fs.h"

#include "masxfs_levinfo_io.h"

static int _uUu_
masxfs_levinfo_open_at( masxfs_levinfo_t * li, int fdparent, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

#if 1
  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_open_at( li, fdparent ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_open_at( li, fdparent ) );
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_open_at( li, fdparent ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_open_at( li, fdparent ) );
    break;
  }
#endif
  rRET;
}

int
masxfs_levinfo_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  /* WARN( "%d: '%s'", li->detype, li->name ); */
#if 1
  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_open( li, flags ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_open( li, flags ) );
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_open( li, flags ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_open( li, flags ) );
    break;
  }
#endif
  rRET;
}

int
masxfs_levinfo_close( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

#if 1
  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_close( li ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_close( li ) );
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_close( li ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_close( li ) );
    break;
  }
#endif
  rRET;
}

int
masxfs_levinfo_close_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  if ( li )
  {
    do
    {
      rC( masxfs_levinfo_close( li, flags ) );
    /* test li->lidepth BEFORE li-- */
    } while ( rGOOD && li->lidepth && li-- );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
#if 1
  if ( flags & MASXFS_CB_MODE_FS )
    rC( masxfs_levinfo_fs_stat( li, flags ) );
  if ( flags & MASXFS_CB_MODE_DB )
    rC( masxfs_levinfo_db_stat( li, flags ) );
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  switch ( mode )
  {
  case MASXFS_SCAN__MODE_NONE:
  /* rSETBAD; */
    QRLI( li, rCODE );
    break;
  case MASXFS_SCAN__MODE_FS:
    rC( masxfs_levinfo_fs_stat( li, flags ) );
    break;
  case MASXFS_SCAN__MODE_DB:
    rC( masxfs_levinfo_db_stat( li, flags ) );
    break;
  }
#endif
  rRET;
}
