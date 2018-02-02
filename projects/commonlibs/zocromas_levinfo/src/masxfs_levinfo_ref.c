#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <sys/types.h> */
#include <sys/stat.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_format.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_levinfo_refdef.h"
#include "masxfs_levinfo_ref.h"

#if 1
const struct stat *
masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  const struct stat *st = NULL;

  if ( li && ( tflags & MASXFS_CB_STAT ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* entry_pfilter */ , NULL /* stat */  ) );
    if ( rGOOD )
      st = masxfs_levinfo_stat_val( li, 0, tflags );
    else
    {
    /* WARN( "STAT ERR" ); */
    }
  }
  return st;
}
#endif

#if 1
masxfs_dir_t * __attribute__ ( ( pure ) ) masxfs_levinfo_pdir_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.scan.pdir : NULL ) : 0;
}
#endif

const char *
masxfs_levinfo_prefix_ref( masxfs_levinfo_t * li, char *p1, char *p2, char *p3, char *p4, masxfs_depth_t top_depth, masxfs_levinfo_flags_t tflags )
{
  const char *prefix = NULL;

  if ( li )
  {
    if ( ( tflags & MASXFS_CB_PREFIX ) )
    {
      if ( li->prefix )
        mas_free( li->prefix );
      prefix = li->prefix = masxfs_levinfo_prefix( li, p1, p2, p3, p4, top_depth, 0 /* test! - not flags */  );
    }
  }
  return prefix;
}

masxfs_entry_type_t
masxfs_levinfo_detype( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  rDECLBAD;
  masxfs_entry_type_t detype = MASXFS_ENTRY_UNKNOWN_NUM;

  if ( li )
  {
    rSETGOOD;
    if ( li->detype == MASXFS_ENTRY_UNKNOWN_NUM )
    {
      if ( masxfs_levinfo_stat_ref( li, tflags ) )
        detype = li->detype;
    }
    else
    {
      detype = li->detype;
    }
  /* assert( detype == li->detype ); */
  }
  else
    QRLI( li, rCODE );
  return detype;
}

int
masxfs_levinfo_has_stat( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  const struct stat *stat = masxfs_levinfo_stat_ref( li, tflags );

  return stat ? 1 : 0;
}
