#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_io_dir.h"

#include "masxfs_levinfo_digest.h"

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
masxfs_levinfo_n_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, size_t len,
                       masxfs_entry_type_t d_type /*, ino_t d_inode _uUu_ */ ,
                       unsigned long long node_id, masxfs_stat_t * stat )
{
  if ( li && name )
  {
    char *newname = mas_strndup( name, len );

    if ( li->name )
      mas_free( li->name );
    li->name = newname;
    li->detype = d_type;
  /* li->deinode = d_inode; (* TODO take from destat OR needless!!! *) */
    li->lidepth = lidepth;
    li->db.node_id = node_id;
    if ( li->db.stat )
      mas_free( li->db.stat );
    li->db.stat = NULL;
    if ( stat )
    {
      li->db.stat = mas_calloc( 1, sizeof( masxfs_stat_t ) );
      *( li->db.stat ) = *stat;                                      /* memcpy */
    }
  }
  else
    QRLI( li, -1 );
}

void
masxfs_levinfo_init( masxfs_levinfo_t * li, masxfs_depth_t lidepth, const char *name, masxfs_entry_type_t d_type /*, ino_t d_inode */ ,
                     masxfs_stat_t * stat, unsigned long long node_id )
{
  masxfs_levinfo_n_init( li, lidepth, name, name ? strlen( name ) : 0, d_type /*, d_inode */ , node_id, stat );
}

void
masxfs_levinfo_reset( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags )
{
  if ( li )
  {
    masxfs_levinfo_closedir( li, flags | MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    masxfs_levinfo_close( li, flags | MASXFS_CB_MODE_FS | MASXFS_CB_MODE_DB );
    li->fd = 0;
    if ( li->name )
      mas_free( li->name );
    li->name = NULL;
    {
    /* if ( flags & MASXFS_CB_MODE_FS ) */
      {
        if ( li->fs.stat )
          mas_free( li->fs.stat );
        li->fs.stat = NULL;
      }
    /* if ( flags & MASXFS_CB_MODE_DB ) */
      {
        if ( li->db.stat )
          mas_free( li->db.stat );
        li->db.stat = NULL;
      }
    }
    if ( li->path )
      mas_free( li->path );
    li->path = NULL;
    if ( li->prefix )
      mas_free( li->prefix );
    li->prefix = NULL;

    masxfs_digests_delete( li->digests );

    li->digests = NULL;
    memset( li, 0, sizeof( masxfs_levinfo_t ) );
  }
}

void
masxfs_levinfo_reset_lia( masxfs_levinfo_t * lia, masxfs_depth_t sz, masxfs_levinfo_flags_t flags )
{
  if ( lia )
    for ( masxfs_depth_t il = 0; il < sz && ( lia + il )->name; il++ )
    {
      masxfs_levinfo_reset( lia + il, flags );
    /* if ( il < sz - 1 )                   */
    /*   li->detype = MASXFS_ENTRY_DIR_NUM; */
    }
}

void
masxfs_levinfo_delete_lia( masxfs_levinfo_t * li, masxfs_depth_t sz, masxfs_levinfo_flags_t flags )
{
  if ( li )
  {
    masxfs_levinfo_reset_lia( li, sz, flags );
    mas_free( li );
  }
}
