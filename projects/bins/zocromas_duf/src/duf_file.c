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
duf_sql_file_to_filenameid( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                            void *str_cb_udata )
{
  file_to_filenameid_udata_t *pud;

  pud = ( file_to_filenameid_udata_t * ) sel_cb_udata;
  pud->filenameid = 0;
  pud->md5id = 0;
  if ( presult[0] )
    pud->filenameid = strtoll( presult[0], NULL, 10 );
  if ( presult[1] )
    pud->md5id = strtoll( presult[1], NULL, 10 );
  return 0;
}

unsigned long long
file_at_pathid_to_filenameid_x( unsigned long long pathid, const char *name, unsigned long long *pmd5id )
{
  int r = 0;

  /* unsigned long long filenameid; */
  file_to_filenameid_udata_t ud;
  char *qname;

  qname = duf_single_quotes_2( name );
  ud.md5id = 0;
  ud.filenameid = 0;
  r = duf_sql_select( duf_sql_file_to_filenameid, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                      "SELECT duf_filenames.id, duf_md5.id FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                      " WHERE duf_filenames.pathid='%llu' and duf_filenames.name='%s'", pathid, qname ? qname : name );
  if ( r >= 0 && pmd5id )
  {
    *pmd5id = ud.md5id;
  }
  free( qname );
  return ud.filenameid;
}

unsigned long long
file_at_pathid_to_filenameid( unsigned long long pathid, const char *name )
{
  return file_at_pathid_to_filenameid_x( pathid, name, NULL );
}

unsigned long long
file_at_path_to_filenameid_x( const char *path, const char *name, unsigned long long *pmd5id )
{
  return file_at_pathid_to_filenameid_x( duf_path_to_pathid( path ), name, pmd5id );
}

unsigned long long
file_at_path_to_filenameid( const char *path, const char *name )
{
  return file_at_path_to_filenameid_x( path, name, NULL );
}

int
duf_print_files( const char *path, int recursive )
{
  return duf_print_files_by_pathid( duf_path_to_pathid( path ), recursive );
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
duf_sql_filenameid_to_filepath( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                                void *str_cb_udata )
{
  filenameid_to_filepath_udata_t *pud;

  pud = ( filenameid_to_filepath_udata_t * ) sel_cb_udata;
  pud->pathid = 0;
  pud->filename = NULL;
  if ( presult[0] )
    pud->pathid = strtoll( presult[0], NULL, 10 );
  if ( presult[2] )
    pud->filename = mas_strdup( presult[2] );
  /* fprintf( stderr, "presult[0]=%s; presult[2]=[%s]\n", presult[0], presult[2] ); */
  return 0;
}

char *
filenameid_to_filepath( unsigned long long filenameid )
{
  int r = 0;
  char *filepath = NULL;
  filenameid_to_filepath_udata_t ud;

  r = duf_sql_select( duf_sql_filenameid_to_filepath, &ud, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                      "SELECT duf_filenames.pathid, duf_filenames.id, duf_filenames.name " " FROM duf_filenames "
                      " WHERE duf_filenames.id='%llu'", filenameid );
  if ( r >= 0 )
  {
    /* fprintf( stderr, "pathid: %lld; filename:%s\n", ud.pathid, ud.filename ); */
    filepath = duf_pathid_to_path( ud.pathid );
    if ( filepath )
    {
      filepath = mas_strcat_x( filepath, "/" );
      filepath = mas_strcat_x( filepath, ud.filename );
    }
    mas_free( ud.filename );
  }
  return filepath;
}
