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

#include "masxfs_levinfo_ref.h"

masxfs_levinfo_t * __attribute__ ( ( pure ) ) masxfs_levinfo_offset( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  return li ? li + offset : NULL;
}

static inline const struct stat *
        __attribute__ ( ( pure ) ) masxfs_levinfo_stat_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  const struct stat *st = NULL;

  li = masxfs_levinfo_offset( li, offset );
  st = li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.stat : ( tflags & MASXFS_CB_MODE_DB ? li->db.stat : 0 ) ) : 0;
  if ( !st )
  {
    WARN( "STAT ERR (%s) for '%s'", tflags & MASXFS_CB_MODE_FS ? "FS" : ( tflags & MASXFS_CB_MODE_DB ? "DB" : "??" ), li->name );
  }
  return st;
}

#if 1
const struct stat *
masxfs_levinfo_stat_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags)
{
  rDECLBAD;
  const struct stat *st = NULL;

  if ( li && ( tflags & MASXFS_CB_STAT ) )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* entry_pfilter */, NULL /* stat */  ) );
    if ( rGOOD )
      st = masxfs_levinfo_stat_val( li, 0, tflags );
    else
    {
      WARN( "STAT ERR" );
    }
  }
  return st;
}
#endif

#if 1
off_t
masxfs_levinfo_size_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  off_t size = 0;

  li = masxfs_levinfo_offset( li, offset );
  const struct stat *stat = masxfs_levinfo_stat_val( li, 0, tflags );

  if ( stat )
    size = stat->st_size;
  return size;
}

off_t
masxfs_levinfo_size_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags)
{
  rDECLBAD;
  off_t size = 0;

  if ( li )
  {
  /* if ( tflags & MASXFS_CB_MODE_DB )                                               */
  /*   WARN( "DETYPE * : %d '%s' (%d)", li->detype, li->name, li->db.stat ? 1 : 0 ); */
    rC( masxfs_levinfo_stat( li, tflags, NULL /* entry_pfilter */, NULL /* stat */  ) );
  /* if ( tflags & MASXFS_CB_MODE_DB )                                               */
  /*   WARN( "DETYPE * : %d '%s' (%d)", li->detype, li->name, li->db.stat ? 1 : 0 ); */
    if ( rGOOD )
      size = masxfs_levinfo_size_val( li, 0, tflags );
  }
  return size;
}
#endif

#if 1
ino_t
masxfs_levinfo_inode_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  ino_t inode = 0;

  li = masxfs_levinfo_offset( li, offset );
  const struct stat *stat = masxfs_levinfo_stat_val( li, 0, tflags );

  if ( stat )
    inode = stat->st_ino;
  return inode;
}

ino_t
masxfs_levinfo_inode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags)
{
  rDECLBAD;
  ino_t inode = 0;

  if ( li )
  {
    rC( masxfs_levinfo_stat( li, tflags, NULL /* entry_pfilter */, NULL /* stat */  ) );
    if ( rGOOD )
      inode = masxfs_levinfo_inode_val( li, 0, tflags );
  }
  return inode;
}
#else
ino_t __attribute__ ( ( pure ) ) masxfs_levinfo_deinode_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags _uUu_ )
{
  return li ? li->deinode : 0;
}
#endif

#if 1
int
masxfs_levinfo_fd_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->fd : 0;
}

#endif
#if 1
int
masxfs_levinfo_fd_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  int fd = 0;

  if ( li && !( tflags & MASXFS_CB_NO_FD ) )
  {
    int r = 0;

    r = masxfs_levinfo_open( li, tflags );
    if ( r >= 0 )
      fd = li->fd /*masxfs_levinfo_fd_val( li, 0 ) */ ;
  }
  return fd;
}
#endif
#if 1
masxfs_dir_t * __attribute__ ( ( pure ) ) masxfs_levinfo_pdir_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.scan.pdir : NULL ) : 0;
}
#endif
#if 0
masxfs_dirent_t * __attribute__ ( ( pure ) ) masxfs_levinfo_pde_val( masxfs_levinfo_t * li, masxfs_depth_t offset, masxfs_levinfo_flags_t tflags )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? ( tflags & MASXFS_CB_MODE_FS ? li->fs.scan.pde : li->db.pde ) : 0;
}

int masxfs_levinfo_is_open( masxfs_levinfo_t * li ) __attribute__ ( ( alias( "masxfs_levinfo_fd_val" ) ) );

#endif

masxfs_depth_t
masxfs_levinfo_depth_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->lidepth : 0;
}

masxfs_depth_t __attribute__ ( ( pure ) ) masxfs_levinfo_depth_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags _uUu_ )
{
  return masxfs_levinfo_depth_val( li, 0 );
}

const char * __attribute__ ( ( pure ) ) masxfs_levinfo_name_val( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  li = masxfs_levinfo_offset( li, offset );
  return li ? li->name : NULL;
}

const char * __attribute__ ( ( pure ) ) masxfs_levinfo_name_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  const char *name = NULL;

  if ( li )
  {
    if ( !( tflags & MASXFS_CB_OFF_NAME ) )
    {
      name = masxfs_levinfo_name_val( li, 0 );
    }
  }
  return name;
}

const char *
masxfs_levinfo_path_ref( masxfs_levinfo_t * li, masxfs_levinfo_flags_t tflags )
{
  const char *path = NULL;

  if ( li )
  {
    if ( ( tflags & MASXFS_CB_PATH ) )
    {
      if ( li->path )
        mas_free( li->path );
      path = li->path = masxfs_levinfo_li2path_up( li, ( li->detype == MASXFS_ENTRY_DIR_NUM && ( tflags & MASXFS_CB_TRAILINGSLASH ) ) ? '/' : 0 );
    }
  }
  return path;
}

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
    if ( li->detype == MASXFS_ENTRY_UNKNOWN_NUM )
    {
    /* WARN( "%d: '%s'", li->detype, li->name ); */
      rC( masxfs_levinfo_stat( li, tflags, NULL /* entry_pfilter */, NULL /* stat */  ) );

      if ( rGOOD )
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

void
masxfs_levinfo_set_node_id( masxfs_levinfo_t * li, unsigned long node_id )
{
  if ( li )
    li->db.node_id = node_id;
}

unsigned long
masxfs_levinfo_node_id_val( masxfs_levinfo_t * li )
{
  return li ? li->db.node_id : 0;
/* return li ? ( li->lidepth == 0 ? 1 : li->db.node_id ) : 0; */
}

static unsigned long
masxfs_levinfo_node_id_off( masxfs_levinfo_t * li, masxfs_depth_t offset )
{
  unsigned long node_id = 0;

  if ( li )
  {
    masxfs_depth_t lidepth = -offset;

    if ( li->lidepth >= lidepth )
    {
    /* masxfs_levinfo_open( li-offset, tflags ); */
    /* node_id = li[offset].db.node_id; */
      node_id = masxfs_levinfo_node_id_val( li + offset );
    }
  }
  return node_id;
}

int
masxfs_levinfo_parent_id( masxfs_levinfo_t * li )
{
  return masxfs_levinfo_node_id_off( li, -1 );
}
