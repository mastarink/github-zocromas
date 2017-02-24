#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_io_dir.h"

#include "masxfs_levinfo_base.h"

masxfs_levinfo_t *
masxfs_levinfo_create_array( masxfs_depth_t sz )
{
  masxfs_levinfo_t *li = mas_calloc( sz, sizeof( masxfs_levinfo_t ) );

  return li;
}

masxfs_levinfo_t *
masxfs_levinfo_create_array_setup( masxfs_depth_t sz )
{
  masxfs_levinfo_t *lia = masxfs_levinfo_create_array( sz );

  return lia;
}

masxfs_levinfo_t *
masxfs_levinfo_create( void )
{
  return masxfs_levinfo_create_array( 0 );
}

void
masxfs_levinfo_n_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, size_t len, masxfs_entry_type_t d_type, ino_t d_inode )
{
  if ( li )
  {
    if ( li->name )
      mas_free( li->name );
    li->name = mas_strndup( name, len );
    li->detype = d_type;
    li->deinode = d_inode;
    li->lidepth = lidepth;
  }
}

void
masxfs_levinfo_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, masxfs_entry_type_t d_type, ino_t d_inode )
{
  masxfs_levinfo_n_init( li, lidepth, name, name ? strlen( name ) : 0, d_type, d_inode );
}

void
masxfs_levinfo_reset( masxfs_levinfo_t * li )
{
  if ( li )
  {
    masxfs_levinfo_closedir( li );
    masxfs_levinfo_close( li );
    li->fd = 0;
    if ( li->name )
      mas_free( li->name );
    li->name = NULL;
    if ( li->stat )
      mas_free( li->stat );
    li->stat = NULL;
    if ( li->path )
      mas_free( li->path );
    li->path = NULL;
    if ( li->prefix )
      mas_free( li->prefix );
    li->prefix = NULL;
    memset( li, 0, sizeof( masxfs_levinfo_t ) );
  }
}

void
masxfs_levinfo_reset_lia( masxfs_levinfo_t * lia, masxfs_depth_t sz )
{
  if ( lia )
    for ( masxfs_depth_t il = 0; il < sz && ( lia + il )->name; il++ )
      masxfs_levinfo_reset( lia + il );
}

void
masxfs_levinfo_delete_lia( masxfs_levinfo_t * li, masxfs_depth_t sz )
{
  if ( li )
  {
    masxfs_levinfo_reset_lia( li, sz );
    mas_free( li );
  }
}
