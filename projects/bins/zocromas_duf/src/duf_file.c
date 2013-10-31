#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_utils.h"

#include "duf_path.h"
#include "duf_file_pathid.h"
#include "duf_file.h"



typedef struct
{
  unsigned long long md5id;
  unsigned long long filenameid;
} file_to_filenameid_udata_t;
static int
duf_sql_file_to_filenameid( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  file_to_filenameid_udata_t *pud;

  pud = ( file_to_filenameid_udata_t * ) udata;
  pud->filenameid = strtoll( presult[0], NULL, 10 );
  pud->md5id = strtoll( presult[1], NULL, 10 );
  return 0;
}

unsigned long long
file_at_pathid_to_filenameid( unsigned long long pathid, const char *name, unsigned long long *pmd5id )
{
  int r = 0;

  /* unsigned long long filenameid; */
  file_to_filenameid_udata_t ud;
  char *qname;

  qname = duf_single_quotes_2( name );
  ud.md5id = 0;
  ud.filenameid = 0;
  r = duf_sql_select( duf_sql_file_to_filenameid, &ud, NULL, 0, "SELECT duf_filenames.id, duf_md5.id FROM duf_filenames "
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
file_at_path_to_filenameid( const char *path, const char *name, unsigned long long *pmd5id )
{
  return file_at_pathid_to_filenameid( duf_path_to_pathid( path, NULL, NULL ), name, pmd5id );
}

/* 
 * type: duf_sql_select_callback_t 
 * sql must select pathid, filenameid, filename ... (, md5id)
 * */
int
duf_sql_scan_files( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  if ( fuscan )
  {
    unsigned long long pathid;
    unsigned long long filenameid;
    char *path;

    pathid = strtoll( presult[0], NULL, 10 );
    filenameid = strtoll( presult[1], NULL, 10 );
    path = duf_pathid_to_path( pathid );
    ( *fuscan ) ( pathid, path, filenameid, presult[2], NULL );
    mas_free( path );
  }
  return 0;
}

int
duf_scan_vfiles_sql( duf_str_callback_t fun, const char *sql, va_list args )
{
  return duf_sql_vselect( duf_sql_scan_files, NULL /* udata */ , fun, 0, sql, args );
}

/* 
 * sql must select pathid, filenameid, filename
 * */
int
duf_scan_files_sql( duf_str_callback_t fun, const char *sql, ... )
{
  int r = 0;
  va_list args;

  va_start( args, sql );
  r = duf_scan_vfiles_sql( fun, sql, args );
  va_end( args );
  return r;
}

/* type: duf_str_callback_t */
int
duf_sql_scan_print_file( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *udata )
{
  /* printf( "%7llu: %s/%s\n", pathid, path, name ); */
  printf( "%7llu: %-20s @ %s/%s\n", pathid, name, path, name );
  /* printf( "%s ", name ); */
  return 0;
}

int
duf_print_files( const char *path )
{
  return duf_print_files_by_pathid( duf_path_to_pathid( path, NULL, NULL ) );
}
