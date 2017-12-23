#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_levinfo_defs.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"
#include "masxfs_levinfo_base.h"
#include "masxfs_levinfo_io.h"

#include "masxfs_levinfo_fs.h"
#include "masxfs_levinfo_fs_dir.h"

/*
exiternal functions used:

 fdopendir
 closedir
 readdir
 rewinddir
*/

int
masxfs_levinfo_fs_rewinddir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  rC( masxfs_levinfo_fs_opendir( li ) );
  if ( rGOOD && li && li->fs.scan.pdir )
  {
    errno = 0;
    rewinddir( li->fs.scan.pdir );
    if ( !errno )
      rSETGOOD;
    else
      QRLI( li, rCODE );
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_opendir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li )
  {
    rSETGOOD;

    if ( !li->fs.scan.pdir )
    {
      int fd = masxfs_levinfo_fs_open( li );

      if ( fd > 0 )
      {
        errno = 0;
        li->fs.scan.pdir = fdopendir( fd );
        if ( !li->fs.scan.pdir && errno )
          rSETBAD;
        QRLI( li, rCODE );
        if ( rGOOD && li->fs.scan.pdir )
        {
          li->detype = MASXFS_ENTRY_DIR_NUM;
          rC( masxfs_levinfo_fs_rewinddir( li ) );
        }
        else
        {
          rSETBAD;
          QRLI( li, rCODE );
        }
      }
      else
      {
        rSETBAD;
        QRLI( li, rCODE );
      }
    }
  }
  else
    QRLI( li, rCODE );
  rRET;
}

int
masxfs_levinfo_fs_closedir( masxfs_levinfo_t * li )
{
  rDECLBAD;

  if ( li && li->fs.scan.pdir )
  {
    errno = 0;
    rC( closedir( li->fs.scan.pdir ) );
    QRLI( li, rCODE );
    li->fs.scan.pdir = NULL;
    li->fd = 0;                                                      /*  closedir closes fd!  */
  /* rC( masxfs_levinfo_fs_close( li )); */
  }
  else
    rC( masxfs_levinfo_close( li, MASXFS_CB_MODE_FS ) );
  rRET;
}

static inline int
name_valid( const char *name )
{
  int b = 0;

  b = name && !( name[0] == '.' && ( ( name[1] == '.' && name[2] == 0 ) || name[1] == 0 ) );
  return b;
}

int
masxfs_levinfo_fs_readdir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags _uUu_, int *phas_data )
{
  rDECLBAD;
  masxfs_dirent_t *de = NULL;
  int has_data = 0;

  if ( li )
  {
    if ( li->fs.scan.pdir )
    {
      do
      {
        errno = 0;
        de = readdir( li->fs.scan.pdir );

        if ( de || !errno )
          rSETGOOD;
      /* TODO #include <fnmatch.h>                                             */
      /* int fnmatch(const char *pattern, const char *string, int flags); */
      } while ( de && !name_valid( de->d_name ) );
    /* WARN( "%s", de ? de->d_name : NULL ); */
    /* WARN( "%p D%d (%d) X HAS DATA:%d '%s' :: '%s'", li, li->lidepth, li->no_more, has_data, li->name, de ? de->d_name : NULL ); */
      if ( rGOOD && de )
      {
        const char *dename = de->d_name;
        masxfs_entry_type_t detype = masxfs_levinfo_de2entry( de->d_type );

      /* have de->d_name, de->d_type and de->d_ino */
        masxfs_levinfo_init( li + 1, li->lidepth + 1, dename, detype, NULL, NULL, 0 );
        assert( li[0].lidepth + 1 == li[1].lidepth );
        has_data = de ? 1 : 0;
      }
      QRLI( li, rCODE );
    }
  /* if ( !has_data )     */
  /*   WARN( "NO DATA" ); */
    if ( phas_data )
      *phas_data = has_data;
  }
  else
    QRLI( li, rCODE );
  rRET;
}
