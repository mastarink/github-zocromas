#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>
#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include <mastar/levinfo/masxfs_levinfo_io_dir.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>

#include <mastar/levinfo/masxfs_levinfo_refdef.h>
#include <mastar/levinfo/masxfs_levinfo_ref.h>

#include <mastar/levinfo/masxfs_levinfo_scan.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_lia2path( pi->levinfo, pi->pidepth, 0 );
}
#if 0
int
masxfs_pathinfo_open( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  rDECLBAD;
  masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

  rC( masxfs_levinfo_opendir( li, flags, NULL ) );
  rRET;
}
#endif

int
masxfs_pathinfo_scanf_scanner( masxfs_pathinfo_t * pi, masxfs_scanner_t * scanner, void *userdata )
{
  rDECLBAD;
  int rc = 0;

//masxfs_entry_filter_t *entry_pfilter = scanner->entry_pfilter;
//masxfs_entry_callback_t *cbs = scanner->cbs;
  masxfs_levinfo_flags_t flags = scanner->flags;

//masxfs_depth_t maxdepth = scanner->maxdepth; 

/* if ( r >= 0 ) */
  {
//  maxdepth = maxdepth ? pi->pidepth + maxdepth : 0;

    if ( ( pi->flags | flags ) & MASXFS_CB_FROM_ROOT ) // XXX not working!? needless?!
    {
      masxfs_depth_t reldepth = 1 - pi->pidepth;

      rC( masxfs_levinfo_scanf_tree_scanner( pi->levinfo, scanner, userdata, reldepth ) );
    }
    else
    {
      masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );
      masxfs_depth_t reldepth = 0;

      flags |= pi->flags;
      rC( masxfs_levinfo_scanf_tree_scanner( li, scanner, userdata, reldepth ) );
    }
    if ( rGOOD )
    {
      rCODE = rc;
      QRPI( pi, rCODE );
    }
  }
  rRET;
}

int
masxfs_pathinfo_scanf_cbs( masxfs_pathinfo_t * pi, masxfs_entry_filter_t * entry_pfilter, masxfs_entry_callback_t * cbs, void *userdata,
                           masxfs_levinfo_flags_t flags )
{
  masxfs_scanner_t scanner = {.entry_pfilter = entry_pfilter,.cbs = cbs,.flags = flags };
  return masxfs_pathinfo_scanf_scanner( pi, &scanner, userdata );
}

int
masxfs_pathinfo_scan_cbs( masxfs_pathinfo_t * pi, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * cbs, void *userdata,
                          masxfs_levinfo_flags_t flags )
{
  masxfs_entry_filter_t entry_filter = {.typeflags = typeflags };
  return masxfs_pathinfo_scanf_cbs( pi, &entry_filter, cbs, userdata, flags );
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
