#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"


#include "duf_sql.h"
#include "duf_sql_field.h"
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"

#include "duf_dbg.h"

#include "duf_file_pathid.h"

/* ###################################################################### */
#include "duf_dir_scan.h"
/* ###################################################################### */


static int
duf_scan_file( void *str_cb_udata, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord,
               const duf_dirhandle_t * pdhu )
{
  int r;

  DUF_UFIELD( filesize );
  if ( filesize >= duf_config->u.minsize && ( !duf_config->u.maxsize || filesize < duf_config->u.maxsize ) )
  {
    /* fprintf( stderr, "+FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
    pdi->items.total++;
    pdi->items.files++;
    r = sccb->file_scan( str_cb_udata, pdi, precord );
    DUF_TRACE( action, 0, "r=%d", r );
  }
  else
  {
    /* fprintf( stderr, "-FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
  }
  return r;
}

/* duf_scan_dir_by_pi:
 * call duf_scan_items_sql with str_cb + pdi (also) as str_cb_udata and <path> sql
 *   i.e. call str_cb + str_cb_udata 
 *        for each record by standard <path> sql by pathid (i.e. children of pathid) with  corresponding args
 *
 *
 * also call duf_scan_files_by_pathid wirh sccb->file_scan
 *   i.e. call sccb->file_scan + pdi (also) as str_cb_udata 
 *        for each <file> record by pathid (i.e. children of pathid) with corresponding args
 *
 * i.e.
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <rrent> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 * */
static int
duf_scan_dir_by_pi( unsigned long long dirid, const duf_dirhandle_t * pdh, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( sccb && !r && sccb->directory_scan_before && duf_config->cli.act.dirs )
  {
    pdi->items.total++;
    pdi->items.dirs++;
    r = sccb->directory_scan_before( dirid, &pdi->levinfo[pdi->depth].dh, pdi, precord );
    if ( r == DUF_ERROR_MAX_REACHED )
    {
      if ( pdi->depth == 0 )
        DUF_TRACE( action, 0, "Maximum reached ........" );
    }
    /* else if ( r < 0 )             */
    /*   DUF_ERROR( "code: %d", r ); */
  }
  if ( pdi->depth > 0 && !pdi->levinfo[pdi->depth - 1].ndirs )
    pdi->levinfo[pdi->depth - 1].eod = 1;
  if ( r >= 0 && sccb )
  {
    {
/* duf_scan_files_by_pathid:
 * call sccb->file_scan (or sccb->file_scan wrapped by duf_scan_file) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirid with corresponding args
 *
 * duf_scan_file is called from  duf_file_scan.c:duf_sel_cb_items()
 * */
      if ( r >= 0 && sccb && sccb->file_scan )
      {
        r = duf_scan_files_by_dirid( dirid, duf_scan_file /* str_cb */ , pdi, sccb, pdh );
        /* r = duf_scan_files_by_dirid( dirid, sccb->file_scan (* str_cb *) , pdi, sccb ); */
      }
      if ( r == DUF_ERROR_MAX_REACHED )
      {
        if ( pdi->depth == 0 )
          DUF_TRACE( action, 0, "Maximum reached ........" );
      }
      /* else if ( r < 0 )             */
      /*   DUF_ERROR( "code: %d", r ); */
    }
    if ( sccb && r >= 0 && sccb->directory_scan_middle && duf_config->cli.act.dirs )
      r = sccb->directory_scan_middle( dirid, pdh, pdi, precord );

    if ( r >= 0 && sccb )
    {
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
      if ( r >= 0 && sccb )
        r = duf_scan_items_sql( DUF_NODE_NODE, str_cb, pdi, pdi, sccb, pdh, sccb->dir_selector, dirid );
      if ( r == DUF_ERROR_MAX_REACHED )
      {
        if ( pdi->depth == 0 )
          DUF_TRACE( action, 0, "Maximum reached ...." );
      }
      /* else if ( r < 0 )             */
      /*   DUF_ERROR( "code: %d", r ); */
    }
    if ( sccb && r >= 0 && sccb->directory_scan_after && duf_config->cli.act.dirs )
      r = sccb->directory_scan_after( dirid, pdh, pdi, precord );
  }
  else if ( r >= 0 )
    DUF_ERROR( "no sccb - is it an error? r: %d", r );
  /* if ( r < 0 )                  */
  /*   DUF_ERROR( "code: %d", r ); */
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}

/* duf_scan_dirs_by_parentid
 *     1. for <current> dir call sccb->directory_scan_before
 *     2. for each file in <current> dir call sccb->file_scan
 *     3. for <current> dir call sccb->directory_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->directory_scan_after
 * see duf_scan_dir_by_pi
 * */
int
duf_scan_dirs_by_parentid( unsigned long long dirid, const duf_dirhandle_t * pdh, duf_scan_callback_file_t str_cb, duf_depthinfo_t * pdi,
                           duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  DUF_UFIELD_OPT( nfiles );
  DUF_UFIELD_OPT( minsize );
  DUF_UFIELD_OPT( maxsize );
  if (  /* !nfiles || */ !dirid
       || ( ( ( nfiles >= duf_config->u.mindirfiles ) && ( !duf_config->u.maxdirfiles || nfiles < duf_config->u.maxdirfiles ) )
            && ( ( minsize >= duf_config->u.minsize ) && ( !duf_config->u.maxsize || maxsize < duf_config->u.maxsize ) ) ) )
  {
    r = duf_scan_dir_by_pi( dirid, pdh, str_cb, pdi, sccb, precord );
    /* if ( r < 0 && !r == DUF_ERROR_MAX_REACHED ) */
    /*   DUF_TRACE( action, 0, "r=%d", r );        */
    /* else if ( r < 0 )                           */
    /*   DUF_ERROR( "r=%d", r );                   */
  }
  else
  {
    /* fprintf( stderr, "-dirid: %llu : %llu : %llu\n", dirid, nfiles, duf_config->u.mindirfiles ); */
  }
  return r;
}
