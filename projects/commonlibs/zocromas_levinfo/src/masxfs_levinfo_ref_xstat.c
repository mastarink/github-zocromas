#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
/* #include <unistd.h> */

//#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
//#include "masxfs_levinfo_format.h"
//#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_ref.h"
#include "masxfs_levinfo_ref_xstat.h"

static inline const masxfs_xstat_t *
        __attribute__ ( ( pure ) ) masxfs_levinfo_xstat_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  const masxfs_xstat_t *xst = NULL;

  li = masxfs_levinfo_offset( li, offset );
  xst = li ? ( tflags & MASXFS_CB_MODE_FS ? NULL : ( tflags & MASXFS_CB_MODE_DB ? li->db.xstat : 0 ) ) : 0;
  if ( !xst )
  {
    WARN( "STAT ERR (%s) for '%s'", tflags & MASXFS_CB_MODE_FS ? "FS" : ( tflags & MASXFS_CB_MODE_DB ? "DB" : "??" ), li->name );
  /* assert( 0 ); */
  }
  return xst;
}

unsigned long
masxfs_levinfo_nsamesize_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  unsigned long nsamesize = 0;

  li = masxfs_levinfo_offset( li, offset );
  const masxfs_xstat_t *xstat = masxfs_levinfo_xstat_val( li, 0, tflags );

  if ( xstat )
    nsamesize = xstat->nsamesize;
  return nsamesize;
}

unsigned long
masxfs_levinfo_nsamesize_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  unsigned long nsamesize = 0;

  if ( li && ( tflags & MASXFS_CB_MODE_DB ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* stat */ , NULL /* entry_pfilter */ ) );
    if ( rGOOD )
      nsamesize = masxfs_levinfo_nsamesize_val( li, 0, tflags );
  }
  return nsamesize;
}

unsigned long
masxfs_levinfo_nsamesha1_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  unsigned long nsamesha1 = 0;

  li = masxfs_levinfo_offset( li, offset );
  const masxfs_xstat_t *xstat = masxfs_levinfo_xstat_val( li, 0, tflags );

  if ( xstat )
    nsamesha1 = xstat->nsamesha1;
  return nsamesha1;
}

unsigned long
masxfs_levinfo_nsamesha1_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  unsigned long nsamesha1 = 0;

  if ( li && ( tflags & MASXFS_CB_MODE_DB ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* stat */, NULL /* entry_pfilter */  ) );
    if ( rGOOD )
      nsamesha1 = masxfs_levinfo_nsamesha1_val( li, 0, tflags );
  }
  return nsamesha1;
}

const char *
masxfs_levinfo_hexsha1_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  const char *hex_sha1 = NULL;

  li = masxfs_levinfo_offset( li, offset );
  const masxfs_xstat_t *xstat = masxfs_levinfo_xstat_val( li, 0, tflags );

  if ( xstat )
    hex_sha1 = xstat->hex_sha1;
  return hex_sha1;
}

const char *
masxfs_levinfo_hexsha1_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  const char *hex_sha1 = NULL;

  if ( li && ( tflags & MASXFS_CB_MODE_DB ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* stat */, NULL /* entry_pfilter */  ) );
    if ( rGOOD )
      hex_sha1 = masxfs_levinfo_hexsha1_val( li, 0, tflags );
  }
  return hex_sha1;
}
