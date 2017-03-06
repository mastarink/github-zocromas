#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_structs.h>
#include "masxfs_structs.h"

/* #include <mastar/levinfo/masxfs_levinfo_tools.h> */
#include <mastar/levinfo/masxfs_levinfo_path.h>
/* #include <mastar/levinfo/masxfs_levinfo_ref.h> */
#include <mastar/levinfo/masxfs_levinfo.h>

#include "masxfs_pathinfo.h"

char *
masxfs_pathinfo_pi2path( masxfs_pathinfo_t * pi )
{
  return masxfs_levinfo_lia2path( pi->levinfo, pi->pidepth, 0 );
}

int
masxfs_pathinfo_scan( masxfs_pathinfo_t * pi, masxfs_entry_callback_t * callbacks, void *data, unsigned long flags, masxfs_depth_t maxdepth )
{
  int r = 0, rc = 0;

/* r = masxfs_pathinfo_opendir( pi ); */
/* QRPI( pi, r );                     */

  
  /* if ( flags & MASXFS_CB_SCAN_DEPTH ) */
  /* {                                   */
  /*   masxfs_pathinfo_scan_depth(  );   */
  /* }                                   */


/* if ( r >= 0 ) */
  {
    masxfs_levinfo_t *li = masxfs_pathinfo_last_li( pi );

    if ( !( flags & MASXFS_CB_RECURSIVE ) )
      DIE( "%s", "NOREC" );
    r = masxfs_levinfo_scandirn_cb( li, callbacks, data, flags, pi->pidepth + maxdepth );
    QRPI( pi, r );
  /* rc = masxfs_pathinfo_closedir_all( pi ); */
    if ( r >= 0 )
      r = rc;
    QRPI( pi, r );
  }
  return r;
}

masxfs_levinfo_t *
masxfs_pathinfo_tail( masxfs_pathinfo_t * pi, masxfs_depth_t offset )
{
  return pi && pi->levinfo ? pi->levinfo + pi->pidepth - 1 - offset : NULL;
}

masxfs_levinfo_t *
masxfs_pathinfo_last_li( masxfs_pathinfo_t * pi )
{
  return masxfs_pathinfo_tail( pi, 0 );
}
/* TODO : same cb format as masxfs_pathinfo_scan */
int
masxfs_pathinfo_scan_depth( masxfs_pathinfo_t * pi, masxfs_scan_fun_simple_t cb, void *udata _uUu_, unsigned long flags )
{
  int r = 0;

  if ( pi && pi->levinfo )
  {
    for ( masxfs_levinfo_t * li = pi->levinfo; li < pi->levinfo + pi->pidepth; li++ )
    {
      if ( li->lidepth == ( size_t ) ( li - pi->levinfo ) )
      {
        int r _uUu_ = 0;

        r = cb( li, flags, udata );
        if ( r )
          break;
      }
      else
      {
        DIE( "FATAL ERROR" );
      }
    }
  }
  return r;
}
