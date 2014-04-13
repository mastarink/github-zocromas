#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

/* #include "duf_sql.h" */

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
/*   if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && pdi->level < pdi->u.maxdepth )                                   */
/*   {                                                                                                                       */
/*     unsigned long long items = 0;                                                                                         */
/*                                                                                                                           */
/*     r = duf_print_files_by_pathid( pathid, pdi, sccb, items );                                                            */
/*   }                                                                                                                       */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                             */
/*   return r;                                                                                                               */
/* }                                                                                                                         */

/* duf_scan_fil_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid (i.e. children of pathid) with corresponding args
 * */
static int
duf_scan_fil_by_pi( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  if ( duf_config->cli.trace.scan )
    fprintf( stderr, "[SCAN ] %20s: L%u >duf_scan_items_sql str_cb=%p\n", __func__, pdi->level, ( void * ) ( unsigned long long ) str_cb );
  if ( duf_config->cli.trace.scan > 1 && sccb->fieldset )
    fprintf( stderr, "FIELDSET %s\n", sccb->fieldset );
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  const char *fieldset;

  fieldset =
        ( sccb->fieldset ? ( sccb->fieldset ) : ( "duf_filenames.pathid, duf_filenames.id as filenameid, duf_filenames.name as filename,"
                                                  " duf_filedatas.id as filedataid, "
                                                  " duf_filedatas.size as filesize, duf_filedatas.mode as filemode, "
                                                  " duf_filedatas.uid as fileuid, duf_filedatas.gid as filegid," " duf_md5.id as md5id, "
                                                  " duf_md5.size as md5size, duf_md5.dupcnt, duf_md5.md5sum1, duf_md5.md5sum2 " ) );
  r = duf_scan_items_sql( str_cb, pdi, pdi, sccb,
                          "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                          " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id)" " WHERE duf_filenames.pathid='%llu'", fieldset, pathid );
  return r;
}

/* duf_scan_files_by_pathid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid with corresponding args
 * */
int
duf_scan_files_by_pathid( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  return duf_scan_fil_by_pi( pathid, str_cb, pdi, sccb );
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
/*   if ( ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && ( pdi->level < pdi->u.maxdepth - 1 ) )                                                 */
/*   {                                                                                                                                             */
/*     {                                                                                                                                           */
/*       (* print directory *)                                                                                                                     */
/*       char *path;                                                                                                                               */
/*                                                                                                                                                 */
/*       path = duf_pathid_to_path( pathid );                                                                                                      */
/*       if ( items )                                                                                                                              */
/*         printf( "   >%2d (%5llu) [%s]\n", pdi ? pdi->level : 0, items, path );                                                                  */
/*       else                                                                                                                                      */
/*         printf( "   >%2d         [%s]\n", pdi ? pdi->level : 0, path );                                                                         */
/*       mas_free( path );                                                                                                                         */
/*     }                                                                                                                                           */
/* (*                                                                                                                                              */
/*  * duf_sql_scan_print_file + pdi to be called for each record                                                                                   */
/*  * *)                                                                                                                                           */
/*     r = duf_scan_files_by_pathid( pathid, duf_sql_scan_print_file, pdi, sccb );                                                                 */
/*     if ( pdi && pdi->u.recursive (* && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) && ( pdi->level < pdi->u.maxdepth - 1 ) *)  )             */
/*     {                                                                                                                                           */
/* (* duf_scan_fil_by_pi:                                                                                                                          */
/*  * call duf_sql_scan_print_files + pdi (also) as str_cb_udata for each <dir> record by pathid (i.e. children of pathid) with corresponding args */
/*  * *)                                                                                                                                           */
/*       r = duf_scan_dirs_by_parentid( pathid, duf_sql_scan_print_files, pdi, NULL );                                                             */
/*     }                                                                                                                                           */
/*   }                                                                                                                                             */
/*   return r;                                                                                                                                     */
/* }                                                                                                                                               */
