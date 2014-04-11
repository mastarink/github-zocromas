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
#include "duf_dir_fill_uni.h"

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt) -- see duf_scan_dirs_by_parentid
 * this is callback of type: duf_scan_callback_file_t (second range):
 * */
int
duf_sql_uni_scan_dir( unsigned long long pathid, unsigned long long filenameid, const char *name,
                       void *str_cb_udata, duf_dirinfo_t * pdi1, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;
  duf_dirinfo_t *pdi;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pdi = ( duf_dirinfo_t * ) str_cb_udata;
  if ( duf_config->verbose > 1 )
  {
    fprintf( stderr, "%s:%s='%s' -- r[%d]='%s'\n", __func__, precord->pnames[2], precord->presult[2],
             duf_sql_pos_by_name( "pathid", precord,0 ), duf_sql_str_by_name( "pathid", precord,0 ) );
  }
  /* pdi->name = precord->presult[2]; */
  pdi->name = duf_sql_str_by_name( "dirname", precord,0 );
  if ( pdi->u.recursive && ( !pdi->u.maxdepth || pdi->level < pdi->u.maxdepth ) )
  {
    if ( pdi->levinfo )
    {
      if ( !precord->nrow )
        pdi->levinfo[pdi->level + 1] = precord->nrows;
      r = duf_scan_dirs_by_parentid( pathid, duf_sql_uni_scan_dir, pdi, sccb );
      pdi->levinfo[pdi->level + 1]--;
    }
  }
  /* printf( "** L%u >files by %llu\n", pdi->level, pathid );                            */
  /* if ( sccb && !r )                                                                   */
  /*   r = duf_scan_files_by_pathid( pathid, sccb->file_scan (* str_cb *) , pdi, sccb ); */
  /* printf( "** L%u <files by %llu\n", pdi->level, pathid );                            */

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}


/* int                                                                                              */
/* duf_uni_scan_by_pathid( unsigned long long pathid, duf_ufilter_t u, duf_scan_callbacks_t * sccb ) */
/* {                                                                                                */
/* }                                                                                                */

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

        /*
         * call duf_sql_uni_scan_dir with pdi for each dir at db with given parentID
         * */
        r = duf_scan_dirs_by_parentid( pathid, duf_sql_uni_scan_dir, &di, sccb );
        mas_free( di.levinfo );
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
    scan_callbacks.directory_scan = duf_sql_scan_fill_dir_uni;
    if ( config->files )
      scan_callbacks.file_scan = duf_sql_uni_scan_fill_files_uni;
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
  r = duf_uni_scan_targ( config, &scan_callbacks );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
