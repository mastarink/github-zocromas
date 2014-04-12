#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_config.h"

/* #include "duf_sql.h" */
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */

#include "duf_filedata.h"
#include "duf_filename.h"
#include "duf_dirent.h"
#include "duf_file_scan.h"
#include "duf_update_pathid.h"

#include "duf_sql.h"
#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_fill_uni.h"
/* ###################################################################### */



/* callback of type duf_scan_callback_file_t */
int
duf_fill_files_uni( unsigned long long pathid, unsigned long long filenameid,
                    const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *cb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* stat */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );


/* static int                                                                                                                        */
/* duf_sql_update_filedatas(  duf_record_t * precord, va_list args, void *sel_cb_udata,                          */
/*                           duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb ) */


  /* 
   * --uni-scan   -R   --fill   --files   -FF
   *                   ^^^^^^   ^^^^^^^
   * */


  if ( duf_config->fill_trace )
  {
    fprintf( stderr, "[FILL ] %20s: %s\n", __func__, name );
  }
  return 0;
}

/* callback of type duf_scan_callback_dir_t */
int
duf_fill_dir_uni( unsigned long long pathid, const char *name, unsigned long long items, duf_dirinfo_t * pdi,
                  struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( duf_config->fill_trace > 1 )
    fprintf( stderr, "[FILL ] %20s: pathid=%llu\n", __func__, pathid );

  if ( pdi && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) )
  {
    char *path = duf_pathid_to_path( pathid );

    if ( duf_config->fill_trace > 1 )
      fprintf( stderr, "[FILL ] %20s: path=%s\n", __func__, path );

/* TODO additionally 
 * for each direntry:
 *  -- 
 * */

/* stat */
    if ( !pdi->u.noself_dir )
    {
      /* pathid = duf_update_realpath_self_up( real_path, NULL, !pfilter->u.noupper_dirs ); */
    }
/* duf_fill_pathid_filter_uni:
 * update (collected below) information for path
 * */
    duf_fill_pathid_filter_uni( pathid, pdi );


    if ( duf_config->fill_trace > 1 )
      fprintf( stderr, "[FILL ] %20s: end path=%s\n", __func__, path );
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

static unsigned long long
duf_update_realpath_file_name_inode_filter_uni( const char *real_path, const char *fname, unsigned long long pathid, duf_dirinfo_t * pdi )
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
    /* fprintf( stderr, "duf_insert_filedata %lu %lu\n", file_inode, pst_file->st_dev ); */
    resd = duf_insert_filedata_uni( pst_file );
    resf = duf_insert_filename( fname, pathid, resd );
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, resf );
  return resf;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_ctrnt_uni( duf_record_t * precord, va_list args, void *sel_cb_udata,
                   duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  unsigned long long *pdir_id;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* fprintf( stderr, "Constraint selected %d\n", nrow1 ); */
  pdir_id = ( unsigned long long * ) sel_cb_udata;
  if ( precord->nrow == 0 )
  {
    if ( duf_config->verbose > 1 )
    {
      fprintf( stderr, "%s='%s' -- r[%d]='%s' / %llu\n", precord->pnames[0], precord->presult[0], duf_sql_pos_by_name( "id", precord, 0 ),
               duf_sql_str_by_name( "id", precord, 0 ), duf_sql_ull_by_name( "id", precord, 0 ) );
    }
    *pdir_id = strtoll( precord->presult[0], NULL, 10 );
    if ( duf_config->fill_trace > 2 )
      fprintf( stderr, "[FILL ] %20s: %llu\n", __func__, *pdir_id );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* insert path into db; return id */
static unsigned long long
duf_insert_path_uni( const char *name, dev_t dev_id, ino_t dir_ino, unsigned long long parentid )
{
  unsigned long long pathid = 0;
  int r;

  char *qbase_name;
  const char *dirname;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  qbase_name = duf_single_quotes_2( name );
  dirname = qbase_name ? qbase_name : name;
  r = duf_sql_c( "INSERT INTO duf_paths (dev, inode, dirname, parentid, ucnt, now) " " VALUES ('%lu','%lu','%s','%lu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, dev_id, dir_ino, dirname, parentid );
  if ( duf_config->fill_trace > 1 )
    fprintf( stderr, "[FILL ] %20s: INSERT: r=%d; dev_id=%lu; dir_ino=%lu; dirname=%s; parentid=%llu\n", __func__, r, dev_id, dir_ino,
             dirname, parentid );
  mas_free( qbase_name );
  /* sql = NULL; */
  if ( r == duf_constraint )
  {
    r = duf_sql_select( duf_sql_ctrnt_uni, &pathid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT id as pathid " " FROM duf_paths " " WHERE dev='%lu' and inode='%lu'", dev_id, dir_ino );
    if ( duf_config->fill_trace > 1 )
      fprintf( stderr, "[FILL ] %20s: insert (SQLITE_CONSTRAINT) r=%d; %llu:'%s'\n", __func__, r, pathid, name );
  }
  else if ( !r /* assume SQLITE_OK */  )
  {
    pathid = duf_last_insert_rowid(  );
    if ( duf_config->fill_trace > 1 )
      fprintf( stderr, "[FILL ] %20s: inserted (SQLITE_OK) pathid=%llu:'%s'\n", __func__, pathid, name );
  }
  else
    fprintf( stderr, "error duf_insert_path %d\n", r );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

static unsigned long long
duf_fill_rfl_flt_uni( const char *real_path, struct dirent *de, unsigned long long pathid, duf_dirinfo_t * pdi )
{
  unsigned long long itemid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  switch ( de->d_type )
  {
  case DT_REG:
    {
      if ( duf_config->fill_trace > 1 )
      {
        fprintf( stderr, "[FILL ] %20s: regfile='%s/%s'\n", __func__, real_path, de->d_name );
      }
      itemid = duf_update_realpath_file_name_inode_filter_uni( real_path, de->d_name, pathid, pdi );
    }
    break;
  case DT_DIR:
    {
      struct stat st, *pst = NULL;

      {
        int r;
        char *fpath;

        fpath = duf_join_path( real_path, de->d_name );
        r = stat( fpath, &st );
        if ( r )
          fprintf( stderr, ">>>>>>>>>>>>>%s No such entry %s\n", __func__, fpath );
        else
          pst = &st;
        mas_free( fpath );
      }


      if ( duf_config->fill_trace )
        fprintf( stderr, "[FILL ] %20s: >>> dir='%s/%s'\n", __func__, real_path, de->d_name );

      /* don't return itemid ??? */
      /* itemid = */
      ( void ) duf_insert_path_uni( de->d_name, pst->st_dev, pst->st_ino, pathid );
    }
    break;
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, itemid );
  return itemid;
}

/* to replace duf_update_realpath_entries_filter */
/* TODO scan for removed files; mark as absent or remove from db */
static int
duf_fill_ent_flt_uni( unsigned long long pathid, duf_dirinfo_t * pdi )
{
  int r = 0;
  int items = 0;

  struct stat st_dir;
  char *real_path;

  real_path = duf_pathid_to_path( pathid );
  if ( duf_config->fill_trace > 1 )
    fprintf( stderr, "[FILL ] %20s: pathid=%llu; real_path='%s'\n", __func__, pathid, real_path );

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
    int nlist;
    struct dirent **list = NULL;

    duf_dbgfunc( DBG_START, __func__, __LINE__ );
    /* fprintf( stderr, "Update path entries %s\n", real_path ); */
    if ( duf_config->fill_trace > 1 )
      fprintf( stderr, "[FILL ] %20s: pathid=%llu; scandir\n", __func__, pathid );

    nlist = scandir( real_path, &list, duf_direntry_filter, alphasort );
    if ( nlist < 0 )
      fprintf( stderr, "ERROR %s: nlist= %d\n", real_path, nlist );

    if ( duf_config->fill_trace > 1 )
    {
      /* fprintf( stderr, "\r%s\x1b[K", fname ); */
      fprintf( stderr, "[FILL ] %20s: pathid=%llu; nlist=%d\n", __func__, pathid, nlist );
    }

    for ( int il = 0; il < nlist; il++ )
    {
      {
        unsigned long long itemid = 0;

        if ( duf_config->fill_trace > 1 )
        {
          /* fprintf( stderr, "\r%s\x1b[K", fname ); */
          fprintf( stderr, "[FILL ] %20s: pathid=%llu; entry='%s'\n", __func__, pathid, list[il]->d_name );
        }

        itemid = duf_fill_rfl_flt_uni( real_path, list[il], pathid, pdi );
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

int
duf_fill_entries_filter_uni( unsigned long long pathid, duf_dirinfo_t * pdi )
{
  return duf_fill_ent_flt_uni( pathid, pdi );
}

/* to replace duf_update_pathid_down_filter */
/* duf_fill_pathid_filter_uni:
 * update (collected below) information for path
 * */
static unsigned long long
duf_fill_pi_flt_uni( unsigned long long pathid, duf_dirinfo_t * pdi )
{
  unsigned long items = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( duf_config->fill_trace > 1 )
    fprintf( stderr, "[FILL ] %20s: pathid=%llu\n", __func__, pathid );

  items = duf_fill_entries_filter_uni( pathid, pdi );

  if ( duf_config->fill_trace > 1 )
    fprintf( stderr, "[FILL ] %20s: end pathid=%llu; items=%lu\n", __func__, pathid, items );

  if ( items >= 0 )
  {
    duf_sql( "UPDATE duf_paths " " SET items='%u', last_updated=datetime() " " WHERE id='%lu'", items, pathid );
    /* TODO group is really TAG */
    duf_pathid_group( "updated", pathid, +1 );
  }
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );
  return pathid;
}

unsigned long long
duf_fill_pathid_filter_uni( unsigned long long pathid, duf_dirinfo_t * pdi )
{
  return duf_fill_pi_flt_uni( pathid, pdi );
}
