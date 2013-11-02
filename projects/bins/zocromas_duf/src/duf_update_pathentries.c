#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"
#include "duf_path.h"
#include "duf_dirent.h"
#include "duf_insert.h"

#include "duf_update_realpath.h"
#include "duf_update_path.h"
#include "duf_update_pathid.h"
#include "duf_update_pathentries.h"


static unsigned long long
duf_update_realpath_file_name_inode( const char *real_path, const char *fname, ino_t file_inode, struct stat *pst_dir,
                                     unsigned long long dir_id )
{
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file, *pst_file = NULL;

  {
    int r;
    char *fpath;

    fpath = duf_join_path( real_path, fname );
    r = stat( fpath, &st_file );
    if ( !r )
      pst_file = &st_file;
    mas_free( fpath );
  }
  resd = duf_insert_filedata( file_inode, pst_dir, pst_file );
  resf = duf_insert_filename( fname, dir_id, resd );
  return resf;
}

static unsigned long long
duf_update_realpath_file_direntry( const char *real_path, const struct dirent *de, struct stat *pst_dir, unsigned long long dir_id )
{
  return duf_update_realpath_file_name_inode( real_path, de->d_name, de->d_ino, pst_dir, dir_id );
}

static unsigned long long
duf_update_realpath_dirent( const char *real_path, struct dirent *de, struct stat *pst_dir, unsigned long long dir_id, int recursive,
                            int dofiles )
{
  unsigned long long itemid = 0;

  /* fprintf( stderr, "Update dirent %s\n", real_path ); */
  if ( recursive && de->d_type == DT_DIR && !( de->d_name[0] == '.' && ( de->d_name[1] == '.' || de->d_name[1] == 0 ) ) )
  {
    char *recpath;

    recpath = duf_join_path( real_path, de->d_name );
    itemid = duf_update_realpath_down( recpath, dir_id, recursive, dofiles );

    if ( recpath )
      mas_free( recpath );
  }
  else if ( dofiles && de->d_type == DT_REG )
    itemid = duf_update_realpath_file_direntry( real_path, de, pst_dir, dir_id );
  /* fprintf( stderr, "Update dirent %s done\n", real_path ); */
  return itemid;
}

int
duf_update_realpath_entries( const char *real_path, struct stat *pst_dir, unsigned long long dir_id, int recursive, int dofiles )
{
  struct dirent **list = NULL;
  int nlist;
  int items = 0;

  /* fprintf( stderr, "Update path entries %s\n", real_path ); */
  nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
  if ( nlist < 0 )
    fprintf( stderr, "ERROR %s: nlist = %d\n", real_path, nlist );
  for ( int il = 0; il < nlist; il++ )
  {
    unsigned long long itemid = 0;

    /* fprintf( stderr, "to update dirent %d @ %s\n", il, real_path ); */
    itemid = duf_update_realpath_dirent( real_path, list[il], pst_dir, dir_id, recursive, dofiles );
    if ( itemid )
      items++;
    /* fprintf( stderr, "to free list[%d] @ %s\n", il, real_path ); */
    free( list[il] );
    /* fprintf( stderr, "freed list[%d] @ %s\n", il, real_path ); */
  }
  free( list );
  /* fprintf( stderr, "Update path entries %s done\n", real_path ); */
  return items;
}
