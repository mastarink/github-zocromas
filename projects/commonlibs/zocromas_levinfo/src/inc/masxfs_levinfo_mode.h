#ifndef MASXFS_LEVINFO_MODE_H
# define MASXFS_LEVINFO_MODE_H


typedef enum masxfs_scan_mode_e
{
  MASXFS_SCAN__MODE_NONE,
  MASXFS_SCAN__MODE_FS,
  MASXFS_SCAN__MODE_DB,
} masxfs_scan_mode_t;

static inline masxfs_scan_mode_t
masxfs_levinfo_flags_mode( masxfs_levinfo_flags_t flags )
{
  return ( flags & MASXFS_CB_MODE_DB ) ? MASXFS_SCAN__MODE_DB : ( ( flags & MASXFS_CB_MODE_FS ) ? MASXFS_SCAN__MODE_FS : MASXFS_SCAN__MODE_NONE );
}

#endif
