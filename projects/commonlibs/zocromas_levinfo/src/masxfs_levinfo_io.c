#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"

#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_fs.h"
#include "masxfs_levinfo_io.h"

/*
exiternal functions used:

 openat
 open
 close
 fstatat
 fstat
 
 */

static inline masxfs_scan_mode_t
masxfs_levinfo_flags_mode( masxfs_levinfo_flags_t flags )
{
  return ( flags & MASXFS_CB_MODE_DB ) ? MASXFS_SCAN__MODE_DB : ( ( flags & MASXFS_CB_MODE_FS ) ? MASXFS_SCAN__MODE_FS : MASXFS_SCAN__MODE_NONE );
}

static int _uUu_
masxfs_levinfo_open_at( masxfs_levinfo_t * li, int fdparent, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

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
  rRET;
}

int
masxfs_levinfo_open( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

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
  rRET;
}

int
masxfs_levinfo_close( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

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
  rRET;
}

int
masxfs_levinfo_close_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;

  do
  {
    rC( masxfs_levinfo_close( li, flags ) );
  /* test li->lidepth BEFORE li-- */
  } while ( rGOOD && li->lidepth && li-- );

  rRET;
}

int
masxfs_levinfo_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
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
  rRET;
}
