#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

/* #include "duf_sql.h" */
#include "duf_utils.h"

#include "duf_path.h"
/* #include "duf_file.h" */
#include "duf_file_scan.h"

#include "duf_dir_scan.h"
#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_pathid.h"
/* ###################################################################### */


/* static int                                                                                                                */
/* duf_sql_scan_print_files( unsigned long long pathid, unsigned long long filenameid, const char *name,                     */
/*                           void *str_cb_udata, duf_dirinfo_t * pdi1, duf_scan_callbacks_t * sccb, duf_record_t * precord ) */
/* {                                                                                                                         */
/*   int r = 0;                                                                                                              */
/*   duf_dirinfo_t *pdi;                                                                                                     */
/*                                                                                                                           */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                           */
/*   pdi = ( duf_dirinfo_t * ) str_cb_udata;                                                                                 */
/*   if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && pdi->depth < pdi->u.maxdepth )                                   */
/*   {                                                                                                                       */
/*     unsigned long long items = 0;                                                                                         */
/*                                                                                                                           */
/*     r = duf_print_files_by_pathid( pathid, pdi, sccb, items );                                                            */
/*   }                                                                                                                       */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                             */
/*   return r;                                                                                                               */
/* }                                                                                                                         */

/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid (i.e. children of pathid) with corresponding args
 * */
static int
duf_scan_files_by_di( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DUF_TRACE( scan, 0, "L%u >duf_scan_items_sql FIELDSET:%s; str_cb=%p", pdi->depth, sccb->fieldset ? sccb->fieldset : "-none-",
             ( void * ) ( unsigned long long ) str_cb );

  /* DUF_TRACE_SCAN( pdi->depth, ( void * ) ( unsigned long long ) str_cb ); */
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  {
    if ( sccb && sccb->file_selector )
      r = duf_scan_items_sql( DUF_NODE_LEAF, str_cb, pdi, pdi, sccb, sccb->file_selector, /* ... */ sccb->fieldset, dirid );
    else
    {
      fprintf( stderr, "sccb->file_selector must be set for %s\n", sccb->title );
      exit( 4 );
    }
  }
  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid with corresponding args
 * */
int
duf_scan_files_by_dirid( unsigned long long dirid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  return ( duf_config->cli.act.files ) ? duf_scan_files_by_di( dirid, str_cb, pdi, sccb ) : 0;
}

/*
 * print path by pathid
 * then 
 *    duf_scan_files_by_pathid  with duf_sql_scan_print_file
 * if recursive
 *    call duf_scan_dirs_by_parentid with duf_sql_scan_print_files
 *
 * duf_scan_dirs_by_parentid:
 * call duf_sql_scan_print_files as str_cb + pdi (also) as str_cb_udata 
 *    for each <dir> record by pathid (i.e. children of pathid) with corresponding args
 *
 * */
/* int                                                                                                                                             */
/* duf_print_files_by_pathid( unsigned long long pathid, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, unsigned long long items )              */
/* {                                                                                                                                               */
/*   int r = 0;                                                                                                                                    */
/*                                                                                                                                                 */
/*   if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && ( pdi->depth < pdi->u.maxdepth - 1 ) )                                                 */
/*   {                                                                                                                                             */
/*     {                                                                                                                                           */
/*       (* print directory *)                                                                                                                     */
/*       char *path;                                                                                                                               */
/*                                                                                                                                                 */
/*       path = duf_pathid_to_path( pathid );                                                                                                      */
/*       if ( items )                                                                                                                              */
/*         printf( "   >%2d (%5llu) [%s]\n", pdi ? pdi->depth : 0, items, path );                                                                  */
/*       else                                                                                                                                      */
/*         printf( "   >%2d         [%s]\n", pdi ? pdi->depth : 0, path );                                                                         */
/*       mas_free( path );                                                                                                                         */
/*     }                                                                                                                                           */
/* (*                                                                                                                                              */
/*  * duf_sql_scan_print_file + pdi to be called for each record                                                                                   */
/*  * *)                                                                                                                                           */
/*     r = duf_scan_files_by_pathid( pathid, duf_sql_scan_print_file, pdi, sccb );                                                                 */
/*     if ( pdi && pdi->u.recursive (* && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && ( pdi->depth < pdi->u.maxdepth - 1 ) *)  )             */
/*     {                                                                                                                                           */
/* (* duf_scan_files_by_pi:                                                                                                                          */
/*  * call duf_sql_scan_print_files + pdi (also) as str_cb_udata for each <dir> record by pathid (i.e. children of pathid) with corresponding args */
/*  * *)                                                                                                                                           */
/*       r = duf_scan_dirs_by_parentid( pathid, duf_sql_scan_print_files, pdi, NULL );                                                             */
/*     }                                                                                                                                           */
/*   }                                                                                                                                             */
/*   return r;                                                                                                                                     */
/* }                                                                                                                                               */
