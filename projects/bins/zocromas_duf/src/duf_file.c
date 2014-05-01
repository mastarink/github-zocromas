#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"
#include "duf_levinfo.h"

#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_path.h"

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
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
static int
duf_sel_cb_md5id_fnid( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata_unused, duf_depthinfo_t * pdi_unused,
                       duf_scan_callbacks_t * sccb )
{
  int r = 0;
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
  return r;
}

static unsigned long long
file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id, int *pr )
{
  int r = 0;

  md5id_fnid_udata_t ud = {.md5id = 0,.filenameid = 0 };
  char *qname;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  qname = duf_single_quotes_2( name );
  DUF_ERROR( "%s:PATHID_TO_FILENAMEID %llu : %s\n", __func__, pathid, qname ? qname : name );
  {
    r = duf_sql_select( duf_sel_cb_md5id_fnid, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb *//* , ( const duf_dirhandle_t * ) NULL off */ ,
                        "SELECT duf_filenames.id AS filenameid, duf_md5.id AS md5id " " FROM duf_filenames "
                        " JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                        " WHERE duf_filenames.pathid='%llu' AND duf_filenames.name='%s'", pathid, qname ? qname : name );
    if ( r >= 0 && pmd5id )
    {
      *pmd5id = ud.md5id;
    }
  }
  mas_free( qname );
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, ud.filenameid );
  return ud.filenameid;
}

unsigned long long
file_at_pathid_to_filenameid( unsigned long long pathid, const char *name, int *pr )
{
  unsigned long long fnid = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  fnid = file_at_pathid_to_filenameid_x( pathid, name, NULL, pr );
  duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, fnid );
  return fnid;
}

/* unsigned long long                                                                                      */
/* file_at_path_to_filenameid_x( const char *path, const char *name, unsigned long long *pmd5id, int *pr ) */
/* {                                                                                                       */
/*   int r = 0;                                                                                            */
/*   unsigned long long filenameid = 0;                                                                    */
/*   unsigned long long dirid = 0;                                                                         */
/*                                                                                                         */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                         */
/*   dirid = duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL (* pdi *) , &r );                        */
/*   if ( r >= 0 )                                                                                         */
/*     filenameid = file_at_pathid_to_filenameid_x( dirid, name, pmd5id, &r );                             */
/*   if ( pr )                                                                                             */
/*     *pr = r;                                                                                            */
/*   return filenameid;                                                                                    */
/* }                                                                                                       */
/*                                                                                                         */
/* unsigned long long                                                                                      */
/* file_at_path_to_filenameid( const char *path, const char *name, int *pr )                               */
/* {                                                                                                       */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                         */
/*   return file_at_path_to_filenameid_x( path, name, NULL, pr );                                          */
/* }                                                                                                       */

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




#ifdef DUF_COMPILE_EXPIRED

typedef struct
{
  unsigned long long pathid;
  char *filename;
} pi_fname_udata_t;

/* 
 * this is callback of type:duf_sel_cb_t (second range; ; sel_cb)
 * */
static int
duf_sel_cb_pi_fname( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb_unused, void *str_cb_udata,
                     duf_depthinfo_t * pdi_unused, duf_scan_callbacks_t * sccb /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  int r = 0;
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
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

char *
filenameid_to_filepath( unsigned long long filenameid, duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  char *filepath = NULL;
  char *filepath_old = NULL;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( filenameid )
  {
    int r = 0;
    pi_fname_udata_t pifn = {.pathid = 0,.filename = NULL };

    r = duf_sql_select( duf_sel_cb_pi_fname, &pifn, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filenames.pathid AS dirid, duf_filenames.id AS filenameid, duf_filenames.name AS filename "
                        " FROM duf_filenames " " WHERE duf_filenames.id='%llu'", filenameid );
    duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( r >= 0 )
    {
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );


/* #ifdef DUF_COMPILE_EXPIRED */
      filepath_old = duf_pathid_to_path_s( pifn.pathid, pdi, &r );
      if ( r >= 0 && filepath_old )
      {
        filepath_old = mas_strcat_x( filepath_old, "/" );
        filepath_old = mas_strcat_x( filepath_old, pifn.filename );
      }
/* #else */
      filepath = mas_strdup( duf_levinfo_path( pdi ) );
      filepath = mas_strcat_x( filepath, "/" );
      filepath = mas_strcat_x( filepath, pifn.filename );

/* #endif */
      DUF_ERROR( "OLD: %s", filepath_old );
      DUF_ERROR( "LEVI: %s", filepath );
      duf_dbgfunc( DBG_STEPI2S, __func__, __LINE__, ( unsigned ) pifn.pathid, filenameid, filepath );
      mas_free( pifn.filename );
      duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    }
  }
  if ( pr )
    *pr = r;
  duf_dbgfunc( DBG_ENDS, __func__, __LINE__, filepath );
  return filepath;
}
#endif
