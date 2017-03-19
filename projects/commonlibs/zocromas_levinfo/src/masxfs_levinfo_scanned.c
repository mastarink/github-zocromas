#define R_GOOD(_r) (_r>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/qstd/qstd_mstmt.h>

#include "masxfs_levinfo_structs.h"
#include "masxfs_levinfo_mode.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_db.h"
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_io_dir.h"

#include "masxfs_levinfo_scanned.h"

#if 0
const char *
masxfs_levinfo_scanned_name( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
/* rDECLBAD; */
  const char *name = NULL;

#if 1
  if ( li )
  {
    if ( flags & MASXFS_CB_MODE_FS )
    {
      if ( li->fs.scan.pde )
        name = li->fs.scan.pde->d_name;
    }
    if ( flags & MASXFS_CB_MODE_DB )
    {
      if ( li->db.scan.mstmt )
        mas_qstd_mstmt_get_result_string_na( li->db.scan.mstmt, 0, &name );
    }
  }
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.scan.pde )
        name = li->fs.scan.pde->d_name;
      break;
    case MASXFS_SCAN__MODE_DB:
      if ( li->db.scan.mstmt )
        mas_qstd_mstmt_get_result_string_na( li->db.scan.mstmt, 0, &name );
      break;
    }
  }
  else
    QRLI( li, rCODE );
#endif
  return name;
}
#endif

#if 0
masxfs_entry_type_t
masxfs_levinfo_scanned_detype( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
/* rDECLBAD; */
  int type = 0;

#if 1
  if ( li )
  {
    if ( flags & MASXFS_CB_MODE_FS )
    {
      if ( li->fs.scan.pde )
        type = masxfs_levinfo_de2entry( li->fs.scan.pde->d_type );
    }
    if ( flags & MASXFS_CB_MODE_DB )
    {
      type = ( li->db.scan.type );
    }
  }
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.scan.pde )
        type = masxfs_levinfo_de2entry( li->fs.scan.pde->d_type );
      break;
    case MASXFS_SCAN__MODE_DB:
      type = ( li->db.scan.type );
      break;
    }
  }
  else
    QRLI( li, rCODE );
#endif
  return type;
}
#endif

#if 0
ino_t
masxfs_levinfo_scanned_inode( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
/* rDECLBAD; */
  ino_t inode = 0;

#if 1
  if ( li )
  {
    if ( flags & MASXFS_CB_MODE_FS )
    {
      if ( li->fs.scan.pde )
        inode = li->fs.scan.pde->d_ino;
    }
    if ( flags & MASXFS_CB_MODE_DB )
    {
      inode = li->db.scan.inode;
    }
  }
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      if ( li->fs.scan.pde )
        inode = li->fs.scan.pde->d_ino;
      break;
    case MASXFS_SCAN__MODE_DB:
      inode = li->db.scan.inode;
      break;
    }
  }
#endif
  return inode;
}
#endif

#if 0
unsigned long long
masxfs_levinfo_scanned_nodeid( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
/* rDECLBAD; */
  unsigned long long node_id = 0;

#if 1
  if ( flags & MASXFS_CB_MODE_FS )
  {
  }
  if ( flags & MASXFS_CB_MODE_DB )
  {
    if ( li )
      node_id = li->db.scan.node_id;
  }
#else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      node_id = 0;
      break;
    case MASXFS_SCAN__MODE_DB:
      node_id = li->db.scan.node_id;
      break;
    }
  }
#endif
  return node_id;
}
#endif

#if 0
masxfs_stat_t *
masxfs_levinfo_scanned_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int unset )
{
/* rDECLBAD; */
  masxfs_stat_t *destat = NULL;

# if 1
  if ( flags & MASXFS_CB_MODE_FS )
  {
  }
  if ( flags & MASXFS_CB_MODE_DB )
  {
    if ( li )
    {
      destat = li->db.scan.stat;
      if ( unset )
        li->db.scan.stat = NULL;
    }
  }
# else
  masxfs_scan_mode_t mode = masxfs_levinfo_flags_mode( flags );

  if ( li )
  {
    switch ( mode )
    {
    case MASXFS_SCAN__MODE_NONE:
    /* rSETBAD; */
      QRLI( li, rCODE );
      break;
    case MASXFS_SCAN__MODE_FS:
      destat = NULL;                                                 /* ?? */
      break;
    case MASXFS_SCAN__MODE_DB:
      destat = li->db.scan.stat;
      if ( unset )
        li->db.scan.stat = NULL;
      break;
    }
  }
# endif
  return destat;
}

off_t
masxfs_levinfo_scanned_size( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  off_t size = 0;

  if ( li )
  {
    if ( flags & MASXFS_CB_MODE_FS )
    {
    }
    if ( flags & MASXFS_CB_MODE_DB )
    {
      if ( li->db.scan.stat )
        size = li->db.scan.stat->st_size;
      WARN( "(destat):%p SZ: %ld", li->db.scan.stat, size );
    }
  }
  return size;
}
#endif
