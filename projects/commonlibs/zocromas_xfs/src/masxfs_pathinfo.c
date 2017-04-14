#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>
#include <mastar/levinfo/masxfs_levinfo_io_dir.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>
#include <mastar/levinfo/masxfs_levinfo_scan.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_lia2path( pi->levinfo, pi->pidepth, 0 );
}

int
masxfs_pathinfo_open( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  rC( masxfs_levinfo_opendir( li, flags ) );
  rRET;
}

int
masxfs_pathinfo_scan_cbs( masxfs_pathinfo_t * pi, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *data,
                          masxfs_levinfo_flags_t flags, masxfs_depth_t maxdepth )
{
  rDECLBAD;
  int rc = 0;

/* if ( r >= 0 ) */
  {
    maxdepth = maxdepth ? pi->pidepth + maxdepth : 0;
    if ( ( pi->flags | flags ) & MASXFS_CB_FROM_ROOT )
    {
      masxfs_depth_t reldepth = 1 - pi->pidepth;

    /* WARN( "D%d; '%s'",  masxfs_levinfo_depth_val( pi->levinfo, 0 ), masxfs_levinfo_name_val( pi->levinfo, 0 ) ); */
      rC( masxfs_levinfo_scan_tree_cbs( pi->levinfo, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    }
    else
    {
      masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );
      masxfs_depth_t reldepth = 0;

      flags |= pi->flags;
#if 1
    /* rC( masxfs_levinfo_scan_dirn_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) ); */
      rC( masxfs_levinfo_scan_tree_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
#else
      rC( masxfs_levinfo_scan_entry_cbs( li, typeflags, cbs, data, flags, maxdepth, reldepth ) );
    /* rC( masxfs_levinfo_scan_entry_single_cbs( li, typeflags, cbs, data, flags, reldepth ) ); */

    /* rC( masxfs_levinfo_scan_li_cbs( li, typeflags, cbs, data, flags, maxdepth,  reldepth ) ); */
#endif
    }
  /* rc = masxfs_pathinfo_closedir_all( pi ); */
    if ( rGOOD )
    {
      rCODE = rc;
      QRPI( pi, rCODE );
    }
  }
  rRET;
}

masxfs_levinfo_t *
masxfs_pathinfo_tail( masxfs_pathinfo_t * pi, masxfs_depth_t offset )
{
/* return pi && pi->levinfo ? pi->levinfo + pi->pidepth - 1 - offset : NULL; */
  return pi && pi->levinfo ? masxfs_levinfo_offset( pi->levinfo, pi->pidepth - 1 - offset ) : NULL;
}

masxfs_levinfo_t *
masxfs_pathinfo_last_li( masxfs_pathinfo_t * pi )
{
  return masxfs_pathinfo_tail( pi, 0 );
}

#if 0
/* TODO : same cb format as masxfs_pathinfo_scan_cbs */
int
masxfs_pathinfo_scan_depth_cbf( masxfs_pathinfo_t * pi, masxfs_scan_fun_simple_t cb, void *udata, masxfs_levinfo_flags_t flags )
{
  int r = 0;

  if ( pi && pi->levinfo )
  {
    masxfs_levinfo_scan_depth_cbf( pi->levinfo, pi->pidepth, cb, udata, flags );
  }
  return r;
}
#endif
