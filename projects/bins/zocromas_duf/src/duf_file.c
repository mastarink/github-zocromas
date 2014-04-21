#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

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
} md5id_fnid_udata_t;

/* 
 * duf_sql_select_cb_t: 
 * */
static int
duf_sel_cb_md5id_fnid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                       duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  md5id_fnid_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( md5id_fnid_udata_t * ) sel_cb_udata;

  DUF_UFIELD( filenameid );
  pud->filenameid = filenameid;
  /* pud->filenameid = duf_sql_ull_by_name( "filenameid", precord, 0 ); */

  DUF_UFIELD( md5id );
  pud->md5id = md5id;
  /* pud->md5id = duf_sql_ull_by_name( "md5id", precord, 0 ); */

  /* if ( precord->presult[0] )                                    */
  /*   pud->filenameid = strtoll( precord->presult[0], NULL, 10 ); */
  /* if ( precord->presult[1] )                                    */
  /*   pud->md5id = strtoll( precord->presult[1], NULL, 10 );      */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

unsigned long long
file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id )
{
  int r = 0;

  /* unsigned long long filenameid; */
  md5id_fnid_udata_t ud;
  char *qname;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  qname = duf_single_quotes_2( name );
  ud.md5id = 0;
  ud.filenameid = 0;
  fprintf( stderr, "%s:PATHID_TO_FILENAMEID %llu : %s\n", __func__, pathid, qname ? qname : name );
  r = duf_sql_select( duf_sel_cb_md5id_fnid, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                      ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                      "SELECT duf_filenames.id as filenameid, duf_md5.id as md5id " " FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                      " WHERE duf_filenames.pathid='%llu' and duf_filenames.name='%s'", pathid, qname ? qname : name );
  if ( r >= 0 && pmd5id )
  {
    *pmd5id = ud.md5id;
  }
  mas_free( qname );
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
  return file_at_pathid_to_filenameid_x( duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL /* pdi */  ), name, pmd5id );
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
/* int                                                                                                               */
/* duf_print_files( const char *path, duf_ufilter_t u )                                                              */
/* {                                                                                                                 */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                   */
/*   duf_depthinfo_t di = {.depth = 0,                                                                                 */
/*     .seq = 0,                                                                                                     */
/*     .levinfo = NULL,                                                                                              */
/*     .u = u                                                                                                        */
/*   };                                                                                                              */
/*   return duf_print_files_by_pathid( duf_path_to_pathid( path ), &di, ( duf_scan_callbacks_t * ) NULL (* sccb *) , */
/*                                     0 (* items unknown ? *)  );                                                   */
/* }                                                                                                                 */





typedef struct
{
  unsigned long long pathid;
  char *filename;
} pi_fname_udata_t;

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sel_cb_pi_fname( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                     duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  pi_fname_udata_t *pud;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  pud = ( pi_fname_udata_t * ) sel_cb_udata;
  pud->pathid = 0;
  pud->filename = NULL;

  DUF_UFIELD( dirid );
  DUF_SFIELD( filename );

  /* if ( precord->presult[0] )                                */
  /*   pud->pathid = strtoll( precord->presult[0], NULL, 10 ); */
  pud->pathid = dirid;          /* needless?? */
  if ( filename )
    pud->filename = mas_strdup( filename );
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
    pi_fname_udata_t pifn = {.pathid = 0,.filename = NULL };

    r = duf_sql_select( duf_sel_cb_pi_fname, &pifn, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filenames.pathid as dirid, duf_filenames.id as filenameid, duf_filenames.name as filename "
                        " FROM duf_filenames " " WHERE duf_filenames.id='%llu'", filenameid );
    /* fprintf( stderr, "%s:FILENAMEID_TO_FILEPATH %llu => %llu:%s\n", __func__, filenameid, pifn.pathid, pifn.filename ); */
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( r >= 0 )
    {
      /* fprintf( stderr, "pathid: %lld; filename:%s\n", pifn.pathid, pifn.filename ); */
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
      filepath = duf_pathid_to_path_s( pifn.pathid );
      if ( filepath )
      {
        filepath = mas_strcat_x( filepath, "/" );
        filepath = mas_strcat_x( filepath, pifn.filename );
        duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
      }
      duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, ( unsigned ) pifn.pathid, filenameid, filepath );
      mas_free( pifn.filename );
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    }
  }
  duf_dbgfunc( DBG_ENDS, __func__, __LINE__, filepath );
  return filepath;
}
