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

#include "duf_file_scan.h"

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *         int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
 * */
int
duf_sql_scan_files( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  if ( str_cb )
  {
    unsigned long long pathid;
    unsigned long long filenameid;
    char *path;

    pathid = strtoll( presult[0], NULL, 10 );
    filenameid = strtoll( presult[1], NULL, 10 );
    path = duf_pathid_to_path( pathid );
    ( *str_cb ) ( pathid, path, filenameid, presult[2], str_cb_udata /* str_cb_udata */  );
    mas_free( path );
  }
  return 0;
}

int
duf_scan_vfiles_sql( duf_str_cb_t str_cb, void *str_cb_udata, const char *sql, va_list args )
{
  return duf_sql_vselect( duf_sql_scan_files /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, 0, sql, args );
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt) ?
 * */
int
duf_scan_files_sql( duf_str_cb_t str_cb, void *str_cb_udata, const char *sql, ... )
{
  int r = 0;
  va_list args;

  va_start( args, sql );
  r = duf_scan_vfiles_sql( str_cb, str_cb_udata, sql, args );
  va_end( args );
  return r;
}

int
duf_scan_vfiles_sql_trace( duf_str_cb_t str_cb, void *str_cb_udata, int trace, const char *sql, va_list args )
{
  return duf_sql_vselect( duf_sql_scan_files /* sel_cb */ , SEL_CB_UDATA_DEF, str_cb, str_cb_udata, trace, sql, args );
}

int
duf_scan_files_sql_trace( duf_str_cb_t str_cb, void *str_cb_udata, int trace, const char *sql, ... )
{
  int r = 0;
  va_list args;

  va_start( args, sql );
  r = duf_scan_vfiles_sql_trace( str_cb, str_cb_udata, trace, sql, args );
  va_end( args );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_str_cb_t: 
 *              int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
 * */
int
duf_sql_scan_print_file( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
{
  /* printf( "%7llu: %s/%s\n", pathid, path, name ); */
  printf( "%7llu: %-20s @ %7llu: %s/%s\n", filenameid, name, pathid, path, name );
  return 0;
}
