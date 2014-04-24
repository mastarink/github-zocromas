#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_path.h"
#include "duf_file.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_md5id.h"
/* ###################################################################### */




/* get md5id for file by pathid and filenameid */
static unsigned long long
duf_filenameid_md5id( unsigned long long filenameid )
{
  int r;
  unsigned long long md5id = 0;
  duf_scan_callbacks_t sccb = {.title = __FILE__,.fieldset = "md5id" };

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  fprintf( stderr, "BY FILENAMEID %llu\n", filenameid );
  r = duf_sql_select( duf_sel_cb_field_by_sccb, &md5id, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL, &sccb,
                      ( const duf_dirhandle_t * ) NULL,
                      "SELECT md5id FROM duf_filenames " " LEFT JOIN duf_filedatas ON (duf_filenames.dataid=duf_filedatas.id) "
                      " WHERE duf_filenames.id='%llu'", filenameid );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r >= 0 ? md5id : r;
}

/* get md5id for file by full path */
unsigned long long
duf_filepath_md5id( const char *path, int *pr )
{
  int r = 0;
  unsigned long long prev_pathid = 0;
  char *notfound = NULL;
  unsigned long long pathid = 0;
  unsigned long long md5id = 0;
  unsigned long long filenameid = 0;

  DEBUG_START(  );
  pathid = duf_path_to_pathid_x( path, &prev_pathid, &notfound, ( duf_depthinfo_t * ) NULL, &r );
  fprintf( stderr, "PATH %s => %llu / %llu\n", path, pathid, prev_pathid );
  if ( !pathid && prev_pathid && notfound )
  {
    filenameid = file_at_pathid_to_filenameid( prev_pathid, notfound, &r );
    fprintf( stderr, "FILENAMEID %llu (%s) => %llu\n", prev_pathid, notfound, filenameid );
/* get md5id for file by pathid and filenameid */
    md5id = duf_filenameid_md5id( filenameid );
  }
  if ( pr )
    *pr = r;
  mas_free( notfound );
  DEBUG_ENDULL( md5id );
  return md5id;
}

/* duf_scan_files_by_md5id:
 *
 * call str_cb for each file record got by the sql  with given md5id
 * */
/* static int                                                                                                                                 */
/* duf_scan_files_by_md5id( unsigned long long md5id, duf_scan_callback_file_t str_cb, void *str_cb_udata )                                   */
/* {                                                                                                                                          */
/*   int r = 0;                                                                                                                               */
/*                                                                                                                                            */
/* (*                                                                                                                                         */
/*  * str_cb + str_cb_udata to be called for each record with given md5id                                                                     */
/*  * *)                                                                                                                                      */
/*   r = duf_scan_items_sql( str_cb, str_cb_udata, ( duf_depthinfo_t * ) NULL, ( duf_scan_callbacks_t * ) NULL (*  sccb *) ,                    */
/*                           "SELECT duf_filenames.pathid, duf_filenames.id, name, duf_paths.mdpathid, duf_paths.md5dir1, duf_paths.md5dir2 " */
/*                           " FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "                    */
/*                           " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)"                                                    */
/*                           " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_md5.id='%llu'", md5id );                   */
/*   return r;                                                                                                                                */
/* }                                                                                                                                          */

/* duf_print_files_by_md5id:
 * duf_str_cb_scan_print_file to be called for each record with given md5id
 * */
/* int                                                                                   */
/* duf_print_files_by_md5id( unsigned long long md5id )                                  */
/* {                                                                                     */
/*   int r = 0;                                                                          */
/*                                                                                       */
/* (*                                                                                    */
/*  * duf_str_cb_scan_print_file to be called for each record with given md5id           */
/*  * *)                                                                                 */
/*   fprintf( stderr, "%s:PRINT_FILES_BY_MD5ID md5id:%llu\n", __func__, md5id );         */
/*   r = duf_scan_files_by_md5id( md5id, duf_str_cb_scan_print_file, STR_CB_UDATA_DEF ); */
/*   return r;                                                                           */
/* }                                                                                     */

/* int                                                                       */
/* duf_print_files_same( const char *path )                                  */
/* {                                                                         */
/*   unsigned long long md5id;                                               */
/*                                                                           */
/*   md5id = duf_filepath_md5id( path );                                     */
/*   fprintf( stderr, "%s:PATH %s => md5id:%llu\n", __func__, path, md5id ); */
/*   if ( md5id )                                                            */
/*     duf_print_files_by_md5id( md5id );                                    */
/*   return 0;                                                               */
/* }                                                                         */
