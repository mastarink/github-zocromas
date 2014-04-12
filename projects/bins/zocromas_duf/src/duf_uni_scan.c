#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"
#include "duf_config.h"

#include "duf_sql.h"

#include "duf_file.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
/* #include "duf_file.h" */

#include "duf_file_pathid.h"
#include "duf_dir_scan.h"

#include "duf_dbg.h"

#include "duf_dir_print_uni.h"
#include "duf_fill_uni.h"
#include "duf_sample_uni.h"

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

/* duf_sql_uni_scan_dir:
 * this is callback of type: duf_scan_callback_file_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_sql_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan
 *   recursively from <current> dir (if recursive flag set):
 *     2. for each dir in dir call duf_sql_uni_scan_dir + str_cb_udata
 *     3. for each file in dir call sccb->file_scan
 * */
int
duf_sql_uni_scan_dir( unsigned long long pathid, unsigned long long filenameid_needless, const char *name, void *str_cb_udata,
                      duf_dirinfo_t * pdi1, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->verbose > 1 )
  {
    fprintf( stderr, "%s:%s='%s' -- r[%d]='%s'\n", __func__, precord->pnames[2], precord->presult[2],
             duf_sql_pos_by_name( "pathid", precord, 0 ), duf_sql_str_by_name( "pathid", precord, 0 ) );
  }
  if ( duf_config->scan_trace )
  {
    char *path = duf_pathid_to_path( pathid );

    fprintf( stderr, "[SCAN ]   %-30s %llu:%s; %llu\n", __func__, pathid, path, filenameid_needless );
    mas_free( path );
  }
  {
    duf_dirinfo_t *pdi;

    pdi = ( duf_dirinfo_t * ) str_cb_udata;
    if ( pdi->u.recursive && ( !pdi->u.maxdepth || pdi->level < pdi->u.maxdepth ) )
    {
      if ( pdi->levinfo )
      {
        if ( !precord->nrow )
          pdi->levinfo[pdi->level + 1] = precord->nrows;
        pdi->name = duf_sql_str_by_name( "dirname", precord, 0 );
/* duf_scan_fil_by_pi:
 * call duf_sql_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by pathid (i.e. children of pathid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_sql_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan
 *   recursively from <current> dir (if recursive flag set):
 *     2. for each dir in dir call duf_sql_uni_scan_dir + str_cb_udata
 *     3. for each file in dir call sccb->file_scan
 * */
        r = duf_scan_dirs_by_parentid( pathid, duf_sql_uni_scan_dir, pdi, sccb );
        pdi->levinfo[pdi->level + 1]--;
      }
    }
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}
/*
 *   i.e. 
 *     1. for the path dir call sccb->directory_scan
 *   recursively from the path dir (if recursive flag set):
 *     2. for each dir in path call duf_sql_uni_scan_dir
 *     3. for each file in the path call sccb->file_scan
 */
int
duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( sccb )
  {
    duf_dirinfo_t di = {.level = 0,
      .seq = 0,
      .levinfo = NULL,
      .u = u,
      .name = path,
    };
    {
      unsigned long long pathid;

      pathid = duf_path_to_pathid( path );


      if ( pathid || !path )
      {
        if ( di.u.maxdepth && !di.levinfo )
        {
          size_t lsz = sizeof( di.levinfo[0] ) * ( di.u.maxdepth + 1 );

          di.levinfo = mas_malloc( lsz );

          memset( di.levinfo, 0, lsz );
        }

        if ( duf_config->scan_trace )
          fprintf( stderr, "[SCAN ] %20s: %llu:%s  duf_scan_dirs_by_parentid with str_cb=duf_sql_uni_scan_dir(%p)\n", __func__, pathid,
                   path, ( void * ) ( unsigned long long ) duf_sql_uni_scan_dir );
/* duf_sql_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_sql_uni_scan_dir with pdi for each dir at db by pathid (i.e. children of pathid) 
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan
 *   recursively from <current> dir (if recursive flag set):
 *     2. for each dir in dir call duf_sql_uni_scan_dir + &di as str_cb_udata
 *     3. for each file in dir call sccb->file_scan
 * */
        r = duf_scan_dirs_by_parentid( pathid, duf_sql_uni_scan_dir, &di, sccb );
        mas_free( di.levinfo );
      }
      else
      {
        if ( duf_config->scan_trace )
          fprintf( stderr, "[SCAN ] %20s: %llu:%s\n", __func__, pathid, path );
      }
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_uni_scan_targ( duf_config_t * config, duf_scan_callbacks_t * sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( config )
  {
    if ( config->targc > 0 )
      for ( int ia = 0; ia < config->targc; ia++ )
        duf_uni_scan( config->targv[ia], config->u, sccb );
    else
      duf_uni_scan( NULL, config->u, sccb );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_uni_scan_all( duf_config_t * config )
{
  int r = 0;

  static duf_scan_callbacks_t scan_callbacks = {
    .directory_scan = NULL,
    /* .file_scan = duf_sql_uni_scan_print_files_uni, */
    .file_scan = NULL,
  };
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( config->fill )
  {
    scan_callbacks.directory_scan = duf_fill_dir_uni;
    if ( config->files )
      scan_callbacks.file_scan = duf_fill_files_uni;
#if 0
    if ( config->md5 )
      scan_callbacks.file_scan = duf_sql_uni_scan_fill_md5_uni;
#endif
  }
  if ( config->print )
  {
    if ( config->tree )
    {
      scan_callbacks.directory_scan = duf_sql_scan_print_tree_uni;
      if ( config->files )
        scan_callbacks.file_scan = duf_sql_uni_scan_print_files_tree_uni;
    }
    else
    {
      scan_callbacks.directory_scan = duf_sql_scan_print_dir_uni;
      if ( config->files )
        scan_callbacks.file_scan = duf_sql_uni_scan_print_files_plain_uni;
    }
  }
  if ( config->sample )
  {
      scan_callbacks.directory_scan = duf_sample_dir_uni;
      if ( config->files )
        scan_callbacks.file_scan = duf_sample_files_uni;
  }




  r = duf_uni_scan_targ( config, &scan_callbacks );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
