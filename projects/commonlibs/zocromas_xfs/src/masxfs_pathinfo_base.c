#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
/* #include <stdio.h> */
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include <mastar/levinfo/masxfs_levinfo_enums.h>
#include <mastar/levinfo/masxfs_levinfo_base.h>
#include <mastar/levinfo/masxfs_levinfo_path.h>

#include "masxfs_structs.h"

#include "masxfs_pathinfo_base.h"

void
masxfs_pathinfo_set_flags( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  if ( pi )
    pi->flags |= flags;
}

void
masxfs_pathinfo_clear_flags( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  if ( pi )
    pi->flags &= ~flags;
}

masxfs_pathinfo_t *
masxfs_pathinfo_create( void )
{
#if 0
  masxfs_pathinfo_t *pi = mas_malloc( sizeof( masxfs_pathinfo_t ) );

  memset( pi, 0, sizeof( masxfs_pathinfo_t ) );
#else
  masxfs_pathinfo_t *pi = mas_calloc( 1, sizeof( masxfs_pathinfo_t ) );
#endif
  QRPI( pi, pi ? 0 : -1 );
  return pi;
}

void
masxfs_pathinfo_init( masxfs_pathinfo_t * pi, const char *path, masxfs_depth_t depth_limit, masxfs_levinfo_flags_t flags )
{
  if ( pi )
  {
    masxfs_pathinfo_set_flags( pi, flags );
    pi->levinfo = masxfs_levinfo_path2lia( path, depth_limit, &pi->pidepth );
  }
}

masxfs_pathinfo_t *
masxfs_pathinfo_create_setup( const char *path, masxfs_depth_t depth_limit, masxfs_levinfo_flags_t flags )
{
  masxfs_pathinfo_t *pi = NULL;

  errno = 0;
#if 0
  char *real_path = realpath( path, NULL );
#else
  char *real_path = canonicalize_file_name( path );
#endif
  if ( !real_path && errno )
  {
    RGES;
  }
  {
  /* fprintf( stderr, "REAL_PATH: %s (%d)\n", real_path, errno ); */
    pi = masxfs_pathinfo_create(  );
    if ( real_path && pi )
    {
      masxfs_pathinfo_init( pi, real_path, depth_limit, flags );
      if ( real_path )
        free( real_path );
    }
    else
      pi->error = -1;
  }
  return pi;
}

masxfs_pathinfo_t *
masxfs_pathinfo_create_setup_realpath( const char *real_path, masxfs_depth_t depth_limit, masxfs_levinfo_flags_t flags )
{
  masxfs_pathinfo_t *pi = NULL;

  pi = masxfs_pathinfo_create(  );
  if ( real_path && pi )
    masxfs_pathinfo_init( pi, real_path, depth_limit, flags );
  else
    pi->error = -1;
  return pi;
}

void
masxfs_pathinfo_reset( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  if ( pi )
  {
    masxfs_levinfo_delete_lia( pi->levinfo, pi->pidepth, pi->flags | flags );
    pi->levinfo = 0;
    pi->pidepth = 0;
  }
}

void
masxfs_pathinfo_delete( masxfs_pathinfo_t * pi, masxfs_levinfo_flags_t flags )
{
  if ( pi )
  {
    masxfs_pathinfo_reset( pi, flags );
    mas_free( pi );
  }
}
