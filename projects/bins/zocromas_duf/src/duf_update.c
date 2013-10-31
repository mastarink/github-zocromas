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

#include "duf_update.h"



static unsigned long long
duf_update_file_ni( const char *path, const char *fname, ino_t file_inode, struct stat *pst_dir, unsigned long long dir_id )
{
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file, *pst_file = NULL;

  {
    int r;
    char *fpath;

    fpath = duf_join_path( path, fname );
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
duf_update_file_de( const char *path, const struct dirent *de, struct stat *pst_dir, unsigned long long dir_id )
{
  return duf_update_file_ni( path, de->d_name, de->d_ino, pst_dir, dir_id );
}

static void
duf_update_dirent( const char *wpath, struct dirent *de, struct stat *pst_dir, unsigned long long dir_id, int recurse, int dofiles )
{
  /* fprintf( stderr, "Update dirent %s\n", wpath ); */
  if ( recurse && de->d_type == DT_DIR && !( de->d_name[0] == '.' && ( de->d_name[1] == '.' || de->d_name[1] == 0 ) ) )
  {
    char *recpath;

    recpath = duf_join_path( wpath, de->d_name );
    duf_update_path( recpath, dir_id, recurse, dofiles, 0 /* added */  );
    if ( recpath )
      mas_free( recpath );
  }
  else if ( dofiles && de->d_type == DT_REG )
    duf_update_file_de( wpath, de, pst_dir, dir_id );
  /* fprintf( stderr, "Update dirent %s done\n", wpath ); */
}

static int
duf_update_path_entries( const char *wpath, struct stat *pst_dir, unsigned long long dir_id, int recurse, int dofiles )
{
  struct dirent **list = NULL;
  int nlist;

  /* fprintf( stderr, "Update path entries %s\n", wpath ); */
  nlist = scandir( wpath, &list, duf_direntry_filter, alphasort );
  if ( nlist < 0 )
    fprintf( stderr, "ERROR %s: nlist = %d\n", wpath, nlist );
  for ( int il = 0; il < nlist; il++ )
  {
    /* fprintf( stderr, "to update dirent %d @ %s\n", il, wpath ); */
    duf_update_dirent( wpath, list[il], pst_dir, dir_id, recurse, dofiles );
    /* fprintf( stderr, "to free list[%d] @ %s\n", il, wpath ); */
    free( list[il] );
    /* fprintf( stderr, "freed list[%d] @ %s\n", il, wpath ); */
  }
  free( list );
  /* fprintf( stderr, "Update path entries %s done\n", wpath ); */
  return nlist;
}

unsigned long long
duf_update_rpath( char *rpath, unsigned long long parentid, int recurse, int dofiles, const char *tail, int added )
{
  unsigned long long dir_id = 0;
  char *dir_name;
  const char *base_name = NULL;

  /* fprintf( stderr, "Update real path %s\n", rpath ); */

  base_name = basename( rpath );
  dir_name = dirname( rpath );
  if ( base_name && !*base_name && dir_name && *dir_name == '/' && dir_name[1] == 0 )
    base_name = tail;
  if ( base_name )
  {
    char *wpath = NULL;

    /* fprintf( stderr, " -- Update real path; to join %s and %s\n", dir_name, base_name ); */
    wpath = duf_join_path( dir_name, base_name );
    /* fprintf( stderr, " -- Update wpath %s\n", wpath ); */
    {
      int r;
      struct stat st_dir;

      r = stat( wpath, &st_dir );
      if ( !r )
      {
        int items = 0;

        if ( added )
          parentid = duf_update_path( dir_name, 0, 0, 0, 2 /* added */  );
        dir_id = duf_insert_path( base_name, &st_dir, parentid, added );

        if ( dir_id > 0 && *dir_name && !( *dir_name == '.' && ( !dir_name[1] || dir_name[1] == '.' ) ) )
          items = duf_update_path_entries( wpath, &st_dir, dir_id, recurse, dofiles );
        else
          fprintf( stderr, "didn't update path entries %llu\n", dir_id );
        duf_sql( "UPDATE duf_paths SET items='%u' WHERE id='%lu'", items, dir_id );
      }
    }
    if ( wpath )
      mas_free( wpath );
  }
  /* fprintf( stderr, "Update real path %s done\n", rpath ); */
  return dir_id;
}

unsigned long long
duf_update_path( const char *path, unsigned long long parentid, int recurse, int dofiles, int added )
{
  unsigned long long dir_id = 0;
  char *rpath;

  /* fprintf( stderr, "Update path %s\n", path ); */

  rpath = mas_malloc( PATH_MAX );
  if ( rpath )
  {
    ( void ) realpath( path, rpath );
    if ( strlen( rpath ) > 1 )
      dir_id = duf_update_rpath( rpath, parentid, recurse, dofiles, path + 1, added );

    mas_free( rpath );
  }
  return dir_id;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *                  int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan );
 * */
static int
duf_sql_update_mdpaths_path( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  MD5_CTX *pctx;
  unsigned long long md5s1, md5s2, pathid;
  unsigned long long *md64;

  pathid = va_arg( args, unsigned long long );

  pctx = ( MD5_CTX * ) sel_cb_udata;
  if ( presult[0] && presult[1] )
  {
    md5s1 = strtoll( presult[0], NULL, 10 );
    md5s2 = strtoll( presult[1], NULL, 10 );
    MD5_Update( pctx, &md5s1, sizeof( md5s1 ) );
    MD5_Update( pctx, &md5s2, sizeof( md5s2 ) );
  }

  {
    unsigned char md[MD5_DIGEST_LENGTH];

    memset( &md, 0, sizeof( md ) );
    MD5_Final( md, pctx );
    md64 = ( unsigned long long * ) md;
  }
  duf_sql( "UPDATE duf_paths SET md5dir1='%lld', md5dir2='%lld' WHERE id='%lu'", md64[1], md64[0], pathid );
  return 0;
}

int
duf_update_mdpaths_path( unsigned long long pathid )
{
  int r = 0;
  MD5_CTX ctx;

  r = duf_sql_select( duf_sql_update_mdpaths_path, &ctx, NULL, 0,
                      "SELECT md5sum1, md5sum2 FROM duf_md5 LEFT JOIN duf_keydata on (duf_keydata.md5id=duf_md5.id) "
                      " WHERE pathid='%llu' ORDER by md5sum1, md5sum2 ", pathid );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 *             int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan );
 * */
static int
duf_sql_update_mdpaths( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long pathid;

  pathid = strtol( presult[0], NULL, 10 );
  fprintf( stderr, "duf_update_mdpaths %d\n", nrow );
  duf_update_mdpaths_path( pathid );
  return 0;
}

int
duf_update_mdpaths( void )
{
  int r;

  fprintf( stderr, "Start duf_update_mdpaths\n" );
  r = duf_sql_select( duf_sql_update_mdpaths, NULL, NULL, 0, "SELECT id " " FROM duf_paths " /* " WHERE ... IS NULL " */ " ORDER BY id" );
  fprintf( stderr, "End duf_update_mdpaths\n" );
  return r;
}
