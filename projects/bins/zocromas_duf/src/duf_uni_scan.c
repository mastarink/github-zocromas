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
#include "duf_utils.h"
#include "duf_config.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_file.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
/* #include "duf_file.h" */

#include "duf_file_pathid.h"
#include "duf_dir_scan.h"

#include "duf_dbg.h"

/* #include "duf_dir_print_uni.h" */
/* #include "duf_fill_uni.h" */
/* #include "duf_sample_uni.h" */
/* #include "duf_md5_uni.h" */

/* ###################################################################### */
#include "duf_uni_scan.h"
/* ###################################################################### */


/*
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print  --files
 * run   --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree --print
*/

/* duf_uni_scan_dir:
 * this is callback of type: duf_scan_callback_file_t (second range):
 *
 * if recursive flag set, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <current> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 * */
int
duf_uni_scan_dir( void *str_cb_udata, duf_dirinfo_t * xpdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  DUF_UFIELD( dirid );
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( DUF_IF_TRACE( scan ) )
  {
    /* unsigned long long dirid = duf_sql_ull_by_name( "dirid", precord, 0 ); */
    /* DUF_UFIELD( filenameid ); */
    /* unsigned long long filenameid = duf_sql_ull_by_name( "filenameid", precord, 0 ); */
    char *path = duf_pathid_to_path( dirid );

    DUF_TRACE( scan, 0, "TOP B %llu:%s", dirid, path );
    mas_free( path );
  }
  {
    duf_dirinfo_t *pdi;

    pdi = ( duf_dirinfo_t * ) str_cb_udata;
    if ( pdi->u.recursive && ( !pdi->u.maxdepth || pdi->depth <= pdi->u.maxdepth ) )
    {
      if ( pdi->levinfo )
      {
#if 0
        if ( !precord->nrow )
          pdi->levinfo[pdi->depth + 1] = precord->nrows;
/* #else                                                                  */
/*         pdi->levinfo[pdi->depth + 1] = precord->nrows - precord->nrow; */
#endif

        if ( !pdi->levinfo )
        {
          fprintf( stderr, "Error!!\n" );
          exit( 5 );
        }

/* duf_scan_fil_by_pi:
 * call duf_uni_scan_dir + pdi (also) as str_cb_udata for each <dir> record by pathid (i.e. children of pathid) with corresponding args
 *
 * i.e. if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata) with duf_uni_scan_dir
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 *         otherwise do nothing
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <current> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 * */
        r = duf_scan_dirs_by_parentid( dirid, duf_uni_scan_dir, pdi, sccb, precord );
        if ( r == DUF_ERROR_MAX_REACHED )
        {
          if ( pdi->depth == 0 )
            DUF_TRACE( action, 0, "Maximum reached ...." );
        }
        else if ( r < 0 )
          DUF_TRACE( action, 0, "r=%d", r );
#if 0
        pdi->levinfo[pdi->depth + 1]--;
#endif
      }
    }
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/*
 *   i.e. 
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <current> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 */
int
duf_uni_scan( const char *path, duf_ufilter_t u, duf_scan_callbacks_t * sccb )
{
  int r;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( sccb )
  {
    duf_dirinfo_t di = {.depth = 0,
      .seq = 0,
      .levinfo = NULL,
      .u = u,
      /* .name = path, */
    };
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( di.u.maxdepth && !di.levinfo )
    {
      size_t lsz = sizeof( di.levinfo[0] ) * ( di.u.maxdepth + 3 );

      di.levinfo = mas_malloc( lsz );

      memset( di.levinfo, 0, lsz );
    }

    {
      unsigned long long pathid;

      pathid = duf_path_to_pathid( path, &di );
      if ( DUF_IF_TRACE( scan ) )
      {
        char *path = duf_pathid_to_path( pathid );

        DUF_TRACE( scan, 0, "TOP A %llu:%s", pathid, path );
        mas_free( path );
      }


      /* fprintf( stderr, "%s:PATH %s => %llu / %llu\n", __func__, path, pathid, di.levinfo[di.depth].dirid ); */
      if ( pathid || !path )
      {
        DUF_TRACE( scan, 0, "%llu:%s  duf_scan_dirs_by_parentid with str_cb=duf_uni_scan_dir(%p)", pathid,
                   path, ( void * ) ( unsigned long long ) duf_uni_scan_dir );
/* duf_uni_scan_dir:
 * if recursive, call duf_scan_dirs_by_parentid + pdi (built from str_cb_udata)
 *       for each <dir> record by pathid (i.e. children of pathid) with corresponding args 
 * otherwise do nothing
 *
 * call duf_uni_scan_dir with pdi for each dir at db by pathid (i.e. children of pathid) 
 *
 *   i.e.
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <current> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *   recursively from <current> dir (if recursive flag set):
 *     4. for each dir in <current> dir call duf_uni_scan_dir + &di as str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 * */
        r = duf_scan_dirs_by_parentid( pathid, duf_uni_scan_dir, &di, sccb, ( duf_record_t * ) NULL /* precord */  );
      }
      else
      {
      }
      mas_free( di.levinfo );
    }
    if ( duf_config->cli.totals )
    {
      FILE *f;

      f = duf_config->cli.trace.out ? duf_config->cli.trace.out : stdout;
      fprintf( f, "\n\n************************************************************\n" );
      fprintf( f, " seq:%llu", di.seq );
      if ( duf_config->u.maxseq )
        fprintf( f, " of %llu", duf_config->u.maxseq );
      fprintf( f, "\n" );
    }
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

int
duf_uni_scan_targ( duf_scan_callbacks_t * sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( sccb && sccb->init_scan )
    sccb->init_scan( sccb );
  if ( duf_config )
  {
    if ( duf_config->targc > 0 )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_uni_scan( duf_config->targv[ia], duf_config->u, sccb );
    else
      duf_uni_scan( NULL, duf_config->u, sccb );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
duf_uni_scan_all( void )
{
  int r = 0;
  duf_scan_callbacks_t **ppscan_callbacks;
  int max_steps = 100;
  int steps = 0;

  extern duf_scan_callbacks_t duf_fill_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_fill_md5_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;
  extern duf_scan_callbacks_t duf_fill_mdpath_callbacks /* __attribute( ( weak ) ) */ ;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  DUF_TRACE( action, 0, "prep" );
  ppscan_callbacks = mas_malloc( max_steps * sizeof( duf_scan_callbacks_t * ) );
  if ( steps < max_steps && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep fill ..." );
    ppscan_callbacks[steps++] = &duf_fill_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.mdpath && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep mdpath" );
    ppscan_callbacks[steps++] = &duf_fill_mdpath_callbacks;
  }
  if ( steps < max_steps && duf_config->cli.act.md5 && duf_config->cli.act.fill )
  {
    DUF_TRACE( action, 0, "prep fill md5" );
    ppscan_callbacks[steps++] = &duf_fill_md5_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.md5 && duf_config->cli.act.print )
  {
    DUF_TRACE( action, 0, "prep print md5" );
    ppscan_callbacks[steps++] = &duf_print_md5_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.print && duf_config->cli.act.tree && !duf_config->cli.act.md5 )
  {
    DUF_TRACE( action, 0, "prep print tree" );
    ppscan_callbacks[steps++] = &duf_print_tree_callbacks;
  }

  if ( steps < max_steps && duf_config->cli.act.print && !duf_config->cli.act.tree && !duf_config->cli.act.md5 )
    ppscan_callbacks[steps++] = &duf_print_dir_callbacks;

  if ( steps < max_steps && duf_config->cli.act.sample )
    ppscan_callbacks[steps++] = &duf_sample_callbacks;

  for ( int step = 0; step < steps; step++ )
  {
    DUF_TRACE( action, 0, "step %d", step );
    r = duf_uni_scan_targ( ppscan_callbacks[step] );
  }
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
