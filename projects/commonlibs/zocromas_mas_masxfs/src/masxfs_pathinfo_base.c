#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_pathinfo_base.h"

masxfs_pathinfo_t *
masxfs_pathinfo_create( void )
{
#if 0
  masxfs_pathinfo_t *pi = mas_malloc( sizeof( masxfs_pathinfo_t ) );

  memset( pi, 0, sizeof( masxfs_pathinfo_t ) );
#else
  masxfs_pathinfo_t *pi = mas_calloc( 1, sizeof( masxfs_pathinfo_t ) );
#endif
  return pi;
}

void
masxfs_pathinfo_init( masxfs_pathinfo_t * pi, const char *path, masxfs_depth_t depth_limit )
{
  if ( pi )
    pi->levinfo = masxfs_levinfo_path2lia( path, depth_limit, &pi->pidepth );
}

masxfs_pathinfo_t *
masxfs_pathinfo_create_setup( const char *path, masxfs_depth_t depth_limit )
{
#if 0
  char *real_path = realpath( path, NULL );
#else
  char *real_path = canonicalize_file_name( path );
#endif
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create(  );

  QRPI( pi,  ( real_path ? 0 : -1 ) );
  QRPI( pi,  pi ? 0 : -1 );
  if ( real_path && pi )
  {
    masxfs_pathinfo_init( pi, real_path, depth_limit );
    if ( real_path )
      free( real_path );
  }
  else
    pi->error=-1;
  return pi;
}

void
masxfs_pathinfo_reset( masxfs_pathinfo_t * pi )
{
  if ( pi )
  {
    masxfs_levinfo_delete_lia( pi->levinfo, pi->pidepth );
    pi->levinfo = 0;
    pi->pidepth = 0;
  }
}

void
masxfs_pathinfo_delete( masxfs_pathinfo_t * pi )
{
  if ( pi )
  {
    masxfs_pathinfo_reset( pi );
    mas_free( pi );
  }
}
