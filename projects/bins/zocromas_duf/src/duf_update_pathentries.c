#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
#include "duf_path.h"
#include "duf_filedata.h"
#include "duf_filename.h"
#include "duf_dirent.h"
#include "duf_insert.h"
#include "duf_update_realpath.h"
/* #include "duf_update_path.h" */
/* #include "duf_update_pathid.h" */

#include "duf_config.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_update_pathentries.h"
/* ###################################################################### */


static unsigned long long
duf_update_realpath_file_name_inode_filter( const char *real_path, const char *fname, ino_t file_inode, dev_t dev_id,
                                            unsigned long long dir_id, duf_filter_t * pfilter )
{
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file, *pst_file = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  {
    int r;
    char *fpath;

    fpath = duf_join_path( real_path, fname );
    r = stat( fpath, &st_file );
    if ( r )
    {
/* no such entry */
      fprintf( stderr, ">>>>>>>>>>>>>%s No such entry %s\n", __func__, fpath );
    }
    else
    {
      pst_file = &st_file;
    }
    mas_free( fpath );
  }
  if ( st_file.st_size >= pfilter->u.minsize && ( !pfilter->u.maxsize || st_file.st_size < pfilter->u.maxsize ) )
  {
    resd = duf_insert_filedata( file_inode, dev_id, pst_file );
    resf = duf_insert_filename( fname, dir_id, resd );
    if ( duf_config->verbose > 2 )
    {
      /* fprintf( stderr, "\r%s\x1b[K", fname ); */
      if ( pfilter->u.maxseq )
        fprintf( stderr, "(%3d) %4d of %-4d -[%11lu] %s/%s\n", pfilter->level, pfilter->c.pseq ? *( pfilter->c.pseq ) : 0,
                 pfilter->u.maxseq, st_file.st_size, real_path, fname );
      else
        fprintf( stderr, "(%3d) %4d -[%11lu] %s/%s\n", pfilter->level, pfilter->c.pseq ? *( pfilter->c.pseq ) : 0, st_file.st_size,
                 real_path, fname );
    }
    if ( pfilter->c.pseq )
      ( *( pfilter->c.pseq ) )++;
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

static unsigned long long
duf_update_realpath_file_name_inode_filter_uni( const char *real_path, const char *fname, ino_t file_inode, dev_t dev_id,
                                                unsigned long long dir_id, duf_dirinfo_t * pdi )
{
  unsigned long long resd = 0;
  unsigned long long resf = 0;
  struct stat st_file, *pst_file = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  {
    int r;
    char *fpath;

    fpath = duf_join_path( real_path, fname );
    r = stat( fpath, &st_file );
    if ( r )
    {
/* no such entry */
      fprintf( stderr, ">>>>>>>>>>>>>%s No such entry %s\n", __func__, fpath );
    }
    else
    {
      pst_file = &st_file;
    }
    mas_free( fpath );
  }
  if ( st_file.st_size >= pdi->u.minsize && ( !pdi->u.maxsize || st_file.st_size < pdi->u.maxsize ) )
  {
    /* fprintf( stderr, "duf_insert_filedata %lu %lu\n", file_inode, dev_id ); */
    resd = duf_insert_filedata( file_inode, dev_id, pst_file );
    resf = duf_insert_filename( fname, dir_id, resd );
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

static unsigned long long
duf_update_realpath_file_direntry_filter( const char *real_path, const struct dirent *de, dev_t dev_id, unsigned long long dir_id,
                                          duf_filter_t * pfilter )
{
  unsigned long long resf = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  resf = duf_update_realpath_file_name_inode_filter( real_path, de->d_name, de->d_ino, dev_id, dir_id, pfilter );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

static unsigned long long
duf_update_realpath_file_direntry_filter_uni( const char *real_path, const struct dirent *de, dev_t dev_id, unsigned long long dir_id,
                                              duf_dirinfo_t * pdi )
{
  unsigned long long resf = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  resf = duf_update_realpath_file_name_inode_filter_uni( real_path, de->d_name, de->d_ino, dev_id, dir_id, pdi );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}


static unsigned long long
duf_update_realpath_dirent_filter( const char *real_path, struct dirent *de, dev_t dev_id, unsigned long long dir_id,
                                   duf_filter_t * pfilter )
{
  unsigned long long itemid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* fprintf( stderr, "Update dirent %s\n", real_path ); */
  if ( pfilter->u.recursive && de->d_type == DT_DIR && !( de->d_name[0] == '.' && ( de->d_name[1] == '.' || de->d_name[1] == 0 ) )
       && pfilter->level < pfilter->u.maxdepth )
  {
    char *recpath;

    recpath = duf_join_path( real_path, de->d_name );
    itemid = duf_update_realpath_down_filter( recpath, dir_id, pfilter );

    if ( recpath )
      mas_free( recpath );
  }
  else if ( pfilter->c.dofiles && de->d_type == DT_REG )
  {
    itemid = duf_update_realpath_file_direntry_filter( real_path, de, dev_id, dir_id, pfilter );
  }
  /* fprintf( stderr, "Update dirent %s done\n", real_path ); */
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, itemid );
  return itemid;
}

static unsigned long long
duf_update_realpath_dirent_filter_uni( const char *real_path, struct dirent *de, dev_t dev_id, unsigned long long dir_id,
                                       duf_dirinfo_t * pdi )
{
  unsigned long long itemid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( de->d_type == DT_REG )
  {
    itemid = duf_update_realpath_file_direntry_filter_uni( real_path, de, dev_id, dir_id, pdi );
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, itemid );
  return itemid;
}

/* TODO scan for removed files; mark as absent or remove from db */
int
duf_update_realpath_entries_filter( unsigned long long pathid, duf_filter_t * pfilter )
{
  int r = 0;
  struct dirent **list = NULL;
  int nlist;
  int items = 0;

  struct stat st_dir;
  char *real_path;

  real_path = duf_pathid_to_path( pathid );
  r = stat( real_path, &st_dir );
/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !( S_ISDIR( st_dir.st_mode ) ) )
  {
/* no such entry */
    fprintf( stderr, ">>>>>>>>>>>>>%s No such entry %s\n", __func__, real_path );
    /* TODO mark as absent or remove from db */
  }
  else
  {
    duf_dbgfunc( DBG_START, __func__, __LINE__ );
    /* fprintf( stderr, "Update path entries %s\n", real_path ); */
    nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
    if ( nlist < 0 )
      fprintf( stderr, "ERROR %s: nlist = %d\n", real_path, nlist );

    if ( duf_config->verbose > 3 )
    {
      /* fprintf( stderr, "\r%s\x1b[K", fname ); */
      fprintf( stderr, "%s\n", real_path );
    }

    pfilter->level++;
    for ( int il = 0; il < nlist; il++ )
    {
      if ( !pfilter->u.maxseq || !pfilter->c.pseq || *( pfilter->c.pseq ) <= pfilter->u.maxseq )
      {
        unsigned long long itemid = 0;

        itemid = duf_update_realpath_dirent_filter( real_path, list[il], st_dir.st_dev, pathid, pfilter );
        if ( itemid )
          items++;
      }
      if ( list[il] )
        free( list[il] );
    }
    pfilter->level--;
    if ( list )
      free( list );
    /* fprintf( stderr, "Update path entries %s done\n", real_path ); */
    r = items;
  }
  mas_free( real_path );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, items );
  return r;
}

/* to replace duf_update_realpath_entries_filter */
/* TODO scan for removed files; mark as absent or remove from db */
int
duf_fill_entries_filter_uni( unsigned long long pathid, duf_dirinfo_t * pdi )
{
  int r = 0;
  struct dirent **list = NULL;
  int nlist;
  int items = 0;

  struct stat st_dir;
  char *real_path;

  real_path = duf_pathid_to_path( pathid );
  r = stat( real_path, &st_dir );
/* check if it is really directory - by st_dir : S_ISDIR(st_dir.st_mode) */
  if ( r || !( S_ISDIR( st_dir.st_mode ) ) )
  {
/* no such entry */
    fprintf( stderr, ">>>>>>>>>>>>>%s No such entry %s\n", __func__, real_path );
    /* TODO mark as absent or remove from db */
  }
  else
  {
    duf_dbgfunc( DBG_START, __func__, __LINE__ );
    /* fprintf( stderr, "Update path entries %s\n", real_path ); */
    nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
    if ( nlist < 0 )
      fprintf( stderr, "ERROR %s: nlist = %d\n", real_path, nlist );

    if ( duf_config->verbose > 3 )
    {
      /* fprintf( stderr, "\r%s\x1b[K", fname ); */
      fprintf( stderr, "%s\n", real_path );
    }

    for ( int il = 0; il < nlist; il++ )
    {
      {
        unsigned long long itemid = 0;

        itemid = duf_update_realpath_dirent_filter_uni( real_path, list[il], st_dir.st_dev, pathid, pdi );
        if ( itemid )
          items++;
      }
      if ( list[il] )
        free( list[il] );
    }
    if ( list )
      free( list );
    /* fprintf( stderr, "Update path entries %s done\n", real_path ); */
    r = items;
  }
  mas_free( real_path );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, items );
  return r;
}
