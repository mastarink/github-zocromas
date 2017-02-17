#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_path.h"

#include "masxfs_pathinfo.h"
#include "masxfs_pathinfo_base.h"

masxfs_pathinfo_t *
masxfs_pathinfo_create( void )
{
  masxfs_pathinfo_t *pi = mas_malloc( sizeof( masxfs_pathinfo_t ) );

  memset( pi, 0, sizeof( masxfs_pathinfo_t ) );
  return pi;
}

void
masxfs_pathinfo_init( masxfs_pathinfo_t * pi, const char *path, size_t max_depth )
{
  if ( pi )
    pi->levinfo = masxfs_levinfo_path2lia( path, max_depth, &pi->pidepth );
}

masxfs_pathinfo_t *
masxfs_pathinfo_create_setup( const char *path, size_t max_depth )
{
  masxfs_pathinfo_t *pi = masxfs_pathinfo_create(  );
  char *real_path = realpath( path, NULL );

  masxfs_pathinfo_init( pi, real_path, max_depth );
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
