#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_utils.h"
#include "duf_path.h"
#include "duf_file_pathid.h"
/* #include "duf_file_scan.h" */

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file.h"
/* ###################################################################### */



typedef struct
{
  unsigned long long md5id;
  unsigned long long filenameid;
} file_to_filenameid_udata_t;

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_file_to_filenameid(  duf_record_t * precord, va_list args, void *sel_cb_udata,
                            duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  file_to_filenameid_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( file_to_filenameid_udata_t * ) sel_cb_udata;
  pud->filenameid = 0;
  pud->md5id = 0;
  if ( precord->presult[0] )
    pud->filenameid = strtoll( precord->presult[0], NULL, 10 );
  if ( precord->presult[1] )
    pud->md5id = strtoll( precord->presult[1], NULL, 10 );
  return 0;
}

unsigned long long
file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id )
{
  int r = 0;

  /* unsigned long long filenameid; */
  file_to_filenameid_udata_t ud;
  char *qname;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  qname = duf_single_quotes_2( name );
  ud.md5id = 0;
  ud.filenameid = 0;
  r = duf_sql_select( duf_sql_file_to_filenameid, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                      ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                      "SELECT duf_filenames.id as filenameid, duf_md5.id as md5id " " FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                      " WHERE duf_filenames.pathid='%llu' and duf_filenames.name='%s'", pathid, qname ? qname : name );
  if ( r >= 0 && pmd5id )
  {
    *pmd5id = ud.md5id;
  }
  free( qname );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, ud.filenameid );
  return ud.filenameid;
}

unsigned long long
file_at_pathid_to_filenameid( unsigned long long pathid, const char *name )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  return file_at_pathid_to_filenameid_x( pathid, name, NULL );
}

unsigned long long
file_at_path_to_filenameid_x( const char *path, const char *name, unsigned long long *pmd5id )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  return file_at_pathid_to_filenameid_x( duf_path_to_pathid( path ), name, pmd5id );
}

unsigned long long
file_at_path_to_filenameid( const char *path, const char *name )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  return file_at_path_to_filenameid_x( path, name, NULL );
}

/*
 * call duf_print_files_by_pathid for pathid correspond tp path
 * */
int
duf_print_files( const char *path, duf_ufilter_t u )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  duf_dirinfo_t di = {.level = 0,
    .seq = 0,
    .levinfo = NULL,
    .u = u
  };
  return duf_print_files_by_pathid( duf_path_to_pathid( path ), &di, ( duf_scan_callbacks_t * ) NULL /* sccb */ ,
                                    0 /* items unknown ? */  );
}





typedef struct
{
  unsigned long long pathid;
  char *filename;
} filenameid_to_filepath_udata_t;

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_filenameid_to_filepath(  duf_record_t * precord, va_list args, void *sel_cb_udata,
                                duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  filenameid_to_filepath_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( filenameid_to_filepath_udata_t * ) sel_cb_udata;
  pud->pathid = 0;
  pud->filename = NULL;
  if ( precord->presult[0] )
    pud->pathid = strtoll( precord->presult[0], NULL, 10 );
  if ( precord->presult[2] )
    pud->filename = mas_strdup( precord->presult[2] );
  /* fprintf( stderr, "presult[0]=%s; precord->presult[2]=[%s]\n", precord->presult[0], precord->presult[2] ); */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

char *
filenameid_to_filepath( unsigned long long filenameid )
{
  char *filepath = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( filenameid )
  {
    int r = 0;
    filenameid_to_filepath_udata_t ud = {.pathid = 0,.filename = NULL };

    r = duf_sql_select( duf_sql_filenameid_to_filepath, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                        "SELECT duf_filenames.pathid, duf_filenames.id, duf_filenames.name " " FROM duf_filenames "
                        " WHERE duf_filenames.id='%llu'", filenameid );
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( r >= 0 )
    {
      /* fprintf( stderr, "pathid: %lld; filename:%s\n", ud.pathid, ud.filename ); */
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
      filepath = duf_pathid_to_path( ud.pathid );
      if ( filepath )
      {
        filepath = mas_strcat_x( filepath, "/" );
        filepath = mas_strcat_x( filepath, ud.filename );
        duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
      }
      duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, ( unsigned ) ud.pathid, filenameid, filepath );
      mas_free( ud.filename );
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    }
  }
  duf_dbgfunc( DBG_ENDS, __func__, __LINE__, filepath );
  return filepath;
}
