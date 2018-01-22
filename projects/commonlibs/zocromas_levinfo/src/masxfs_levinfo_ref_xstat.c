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
masxfs_levinfo_nsamedigest_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  unsigned long nsamedigest = 0;

  li = masxfs_levinfo_offset( li, offset );
  const masxfs_xstat_t *xstat = masxfs_levinfo_xstat_val( li, 0, tflags );

  if ( xstat )
    nsamedigest = xstat->nsamedigest;
  return nsamedigest;
}

unsigned long
masxfs_levinfo_nsamedigest_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  unsigned long nsamedigest = 0;

  if ( li && ( tflags & MASXFS_CB_MODE_DB ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* stat */, NULL /* entry_pfilter */  ) );
    if ( rGOOD )
      nsamedigest = masxfs_levinfo_nsamedigest_val( li, 0, tflags );
  }
  return nsamedigest;
}

const char *
masxfs_levinfo_hexdigest_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  const char *hex_digest = NULL;

  li = masxfs_levinfo_offset( li, offset );
  const masxfs_xstat_t *xstat = masxfs_levinfo_xstat_val( li, 0, tflags );

  if ( xstat )
    hex_digest = xstat->hex_digest;
  return hex_digest;
}

const char *
masxfs_levinfo_hexdigest_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  const char *hex_digest = NULL;

  if ( li && ( tflags & MASXFS_CB_MODE_DB ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* stat */, NULL /* entry_pfilter */  ) );
    if ( rGOOD )
      hex_digest = masxfs_levinfo_hexdigest_val( li, 0, tflags );
  }
  return hex_digest;
}
