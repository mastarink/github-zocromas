#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

#include "duf_sql.h"
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"

#include "duf_dbg.h"
#include "duf_sql.h"

#include "duf_file_pathid.h"

/* ###################################################################### */
#include "duf_dir_scan.h"
/* ###################################################################### */

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
 *    1. for <current> dir call sccb->directory_scan
 *    2. for each dir in <current> dir call str_cb + str_cb_udata
 *    3. for each file in <current> dir call sccb->file_scan
 * */
static int
duf_scan_dir_by_pi( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( 0 )
    fprintf( stderr, "+L%u\n", pdi->level );
  pdi->level++;
  if ( sccb && sccb->directory_scan )
  {
    /* actions on current dir */
    sccb->directory_scan( pathid, pdi->name, 0 /* items */ , pdi, sccb );
  }

  if ( duf_config->scan_trace )
  {
    char *path = duf_pathid_to_path( pathid );

    fprintf( stderr, "[SCAN ] %20s: %llu:%s\n", __func__, pathid, path );
    mas_free( path );
  }

  {
    if ( duf_config->scan_trace )
      fprintf( stderr, "[SCAN ] %20s: L%u >duf_scan_items_sql by pathid=%llu; str_cb=%p\n", __func__, pdi->level, pathid,
               ( void * ) ( unsigned long long ) str_cb );
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
    r = duf_scan_items_sql( str_cb, pdi, pdi, sccb,
                            "SELECT duf_paths.id as pathid, duf_paths.dirname, items " " FROM duf_paths "
                            " WHERE duf_paths.parentid='%llu'", pathid );
    if ( duf_config->scan_trace )
      fprintf( stderr, "[SCAN ] %20s: L%u <duf_scan_items_sql by pathid=%llu\n", __func__, pdi->level, pathid );
  }

  if ( sccb && !r )
  {
    pdi->level++;
    if ( duf_config->scan_trace )
      fprintf( stderr, "[SCAN ] %20s: L%u >duf_scan_files_by_pathid by pathid=%llu; sccb->file_scan as str_cb=%p\n", __func__, pdi->level,
               pathid, ( void * ) ( unsigned long long ) sccb->file_scan );
/* duf_scan_files_by_pathid:
 * call sccb->file_scan + pdi (also) as str_cb_udata for each <file> record by pathid with corresponding args
 * */
    r = duf_scan_files_by_pathid( pathid, sccb->file_scan /* str_cb */ , pdi, sccb );
    if ( duf_config->scan_trace )
      fprintf( stderr, "[SCAN ] %20s: L%u <duf_scan_files_by_pathid by pathid=%llu; r=%d; sccb:%d\n", __func__, pdi->level, pathid, r,
               sccb ? 1 : 0 );
    pdi->level--;
  }

  pdi->level--;
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return r;
}
/* duf_scan_dirs_by_parentid
 * 1. for <current> dir call sccb->directory_scan
 * 2. for each dir in <current> dir call str_cb + str_cb_udata
 * 3. for each file in <current> dir call sccb->file_scan
 * see duf_scan_dir_by_pi
 * */
int
duf_scan_dirs_by_parentid( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  return duf_scan_dir_by_pi( pathid, str_cb, pdi, sccb );
}

/* static int duf_print_dirs_by_parentid( unsigned long long parentid, duf_dirinfo_t * pdi, int tree ); */

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt) - see duf_scan_dirs_by_parentid
 * this is callback of type: duf_scan_callback_file_t (second range):
 * */
/* static int                                                                                                              */
/* duf_sql_scan_print_dir( unsigned long long pathid, unsigned long long filenameid, const char *name, void *str_cb_udata, */
/*                         duf_dirinfo_t * pdi1, duf_scan_callbacks_t * sccb, duf_record_t * precord )                     */
/* {                                                                                                                       */
/*   duf_dirinfo_t *pdi;                                                                                                   */
/*                                                                                                                         */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                         */
/*   pdi = ( duf_dirinfo_t * ) str_cb_udata;                                                                               */
/*   (* printf( stderr, "%7llu: %s/%s\n", pathid, path, name ); *)                                                         */
/*   if ( pdi )                                                                                                            */
/*   {                                                                                                                     */
/*     char format[256];                                                                                                   */
/*     char *path = duf_pathid_to_path( pathid );                                                                          */
/*                                                                                                                         */
/*                                                                                                                         */
/*     if ( pdi->levinfo && !precord->nrow )                                                                               */
/*       pdi->levinfo[pdi->level] = precord->nrows;                                                                        */
/*                                                                                                                         */
/*     snprintf( format, sizeof( format ), "%%4llu: (%%7s) %%-%ds %%s : %%s\n", pdi->level * 2 + 1 );                      */
/*     (* printf( format, pathid, precord->presult[2], pdi->level ? "" : "-", precord->presult[2], path ); *)              */
/*     printf( format, pathid, "0", pdi->level ? "" : "-", duf_sql_str_by_name( "items", precord, 0 ), path );             */
/*     (* printf( format, pathid,  "0", pdi->level ? "" : "-", name, path ); *)                                            */
/*                                                                                                                         */
/*     pdi->levinfo[pdi->level]--;                                                                                         */
/*     if ( pdi->u.recursive && ( !pdi->u.maxdepth || pdi->level < pdi->u.maxdepth ) )                                     */
/*     {                                                                                                                   */
/*       duf_print_dirs_by_parentid( pathid, pdi, 0 );                                                                     */
/*     }                                                                                                                   */
/*     mas_free( path );                                                                                                   */
/*   }                                                                                                                     */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                           */
/*   return 0;                                                                                                             */
/* }                                                                                                                       */

/*
 * this is callback of type: duf_scan_callback_file_t (second range):
 * */
/* static int                                                                                                                                */
/* duf_sql_scan_print_tree( unsigned long long pathid, unsigned long long filenameid,                                                        */
/*                          const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord ) */
/* {                                                                                                                                         */
/*   (* duf_dirinfo_t *pdi1; *)                                                                                                              */
/*   unsigned long long dbitems = 0;                                                                                                         */
/*                                                                                                                                           */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                                           */
/*   (* pdi1 = ( duf_dirinfo_t * ) str_cb_udata; *)                                                                                          */
/*   (* printf( "%7llu: %s/%s\n", pathid, path, name ); *)                                                                                   */
/*                                                                                                                                           */
/*   dbitems = duf_sql_ull_by_name( "items", precord, 0 );                                                                                   */
/*   (* if ( precord->presult[2] )                            *)                                                                             */
/*   (*   dbitems = strtoll( precord->presult[2], NULL, 10 ); *)                                                                             */
/*                                                                                                                                           */
/*   if ( pdi && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) )                                                                            */
/*   {                                                                                                                                       */
/*     if ( pdi->levinfo && !precord->nrow )                                                                                                 */
/*       pdi->levinfo[pdi->level] = precord->nrows;                                                                                          */
/*     pdi->seq++;                                                                                                                           */
/*     (* printf( "%4llu: (%7s) :", pathid, precord->presult[2] ? precord->presult[2] : "-" ); *)                                            */
/*     for ( int i = 0; i <= pdi->level; i++ )                                                                                               */
/*     {                                                                                                                                     */
/*       if ( pdi->levinfo )                                                                                                                 */
/*       {                                                                                                                                   */
/*         if ( i == 0 )                                                                                                                     */
/*           printf( "%3d ", pdi->seq );                                                                                                     */
/*         if ( pdi->levinfo[i] == 1 && i == pdi->level )                                                                                    */
/*           printf( "└── " );                                                                                                         */
/*         else if ( pdi->levinfo[i] && i == pdi->level )                                                                                    */
/*           printf( "├── " );                                                                                                         */
/*         else if ( pdi->levinfo[i] )                                                                                                       */
/*           printf( "│   " );                                                                                                             */
/*         else                                                                                                                              */
/*           printf( "    " );                                                                                                               */
/*       }                                                                                                                                   */
/*       else                                                                                                                                */
/*       {                                                                                                                                   */
/*       }                                                                                                                                   */
/*     }                                                                                                                                     */
/*     (* printf( "@@ %s :%s\n", precord->presult[1], path ); *)                                                                             */
/*     printf( "%s [items=%llu]\n", precord->presult[2], dbitems );                                                                          */
/*     pdi->levinfo[pdi->level]--;                                                                                                           */
/*     if ( pdi->u.recursive && pdi->level < pdi->u.maxdepth )                                                                               */
/*     {                                                                                                                                     */
/*       duf_print_dirs_by_parentid( pathid, pdi, 1 );                                                                                       */
/*     }                                                                                                                                     */
/*   }                                                                                                                                       */
/*   duf_dbgfunc( DBG_END, __func__, __LINE__ );                                                                                             */
/*   return 0;                                                                                                                               */
/* }                                                                                                                                         */

/* static int                                                                                                       */
/* duf_print_dirs_by_parentid( unsigned long long parentid, duf_dirinfo_t * pdi, int tree )                         */
/* {                                                                                                                */
/*   int r = 0;                                                                                                     */
/*                                                                                                                  */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                  */
/* (* duf_scan_dirs_by_parentid                                                                                     */
/*  * 1. for each dir in <current> dir call str_cb + str_cb_udata                                                   */
/*  * 2. for each file in <current> dir call sccb->file_scan                                                        */
/*  * *)                                                                                                            */
/*   r = duf_scan_dirs_by_parentid( parentid, tree ? duf_sql_scan_print_tree : duf_sql_scan_print_dir, pdi, NULL ); */
/*   duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );                                                                */
/*   return r;                                                                                                      */
/* }                                                                                                                */

/* int                                                                 */
/* duf_print_dirs( const char *path, duf_ufilter_t u, int tree )       */
/* {                                                                   */
/*   int r = 0;                                                        */
/*   unsigned long long pathid;                                        */
/*                                                                     */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                     */
/*   duf_dirinfo_t di = {.level = 0,                                   */
/*     .seq = 0,                                                       */
/*     .levinfo = NULL,                                                */
/*     .u = u                                                          */
/*   };                                                                */
/*   pathid = duf_path_to_pathid( path );                              */
/*   if ( pathid || !path )                                            */
/*   {                                                                 */
/*     if ( di.u.maxdepth && !di.levinfo )                             */
/*     {                                                               */
/*       size_t lsz = sizeof( di.levinfo[0] ) * ( di.u.maxdepth + 1 ); */
/*                                                                     */
/*       di.levinfo = mas_malloc( lsz );                               */
/*                                                                     */
/*       memset( di.levinfo, 0, lsz );                                 */
/*     }                                                               */
/*     {                                                               */
/*       char *top;                                                    */
/*                                                                     */
/*       top = duf_pathid_to_path( pathid );                           */
/*       (* printf( "%s -- %llu\n", top?top:"/", pathid ); *)          */
/*       printf( "[%s]\n", top ? top : "/" );                          */
/*       mas_free( top );                                              */
/*     }                                                               */
/*     r = duf_print_dirs_by_parentid( pathid, &di, tree );            */
/*     mas_free( di.levinfo );                                         */
/*   }                                                                 */
/*   else                                                              */
/*   {                                                                 */
/*     fprintf( stderr, "Path '%s' not in db\n", path );               */
/*     r = -1;                                                         */
/*   }                                                                 */
/*   duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );                   */
/*   return r;                                                         */
/* }                                                                   */
