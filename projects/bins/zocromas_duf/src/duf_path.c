#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_utils.h"

#include "duf_path.h"

char *
duf_join_path( const char *path, const char *fname )
{
  char *fpath = NULL;

  if ( fname )
  {
    if ( path )
      fpath = mas_strdup( path );
    if ( *fname != '/' && !( fpath && *fpath == '/' && fpath[1] == 0 ) )
      fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, fname );
  }
  return fpath;
}

typedef struct
{
  char *name;
  unsigned long long parentid;
} pathid_to_path_udata_t;
/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *             int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_pathid_to_path( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  pathid_to_path_udata_t *pud;

  pud = ( pathid_to_path_udata_t * ) sel_cb_udata;
  if ( nrow == 0 )
  {
    pud->parentid = strtoll( presult[0], NULL, 10 );
    pud->name = mas_strdup( presult[1] );
  }
  /* fprintf( stderr, "pud a:%d nrow:%d\n", presult ? 1 : 0, nrow ); */
  /* fprintf( stderr, "pud b: %s :: %s\n", presult[0], presult[1] ); */
  return 0;
}

char *
duf_pathid_to_path( unsigned long long pathid )
{
  int r;
  char *path = NULL;
  pathid_to_path_udata_t pathdef = {.name = NULL,.parentid = 0 };

  r = duf_sql_select( duf_sql_pathid_to_path, &pathdef, NULL, 0, "SELECT parentid, dirname FROM duf_paths WHERE id=%llu", pathid );
  if ( r >= 0 && pathdef.name && pathdef.parentid >= 0 )
  {
    char *parent = NULL;

    /* fprintf( stderr, "parentid:%lld - %s\n", pathdef.parentid, pathdef.name ); */
    parent = duf_pathid_to_path( pathdef.parentid );
    path = duf_join_path( parent, pathdef.name );
    if ( parent )
      mas_free( parent );
  }
  if ( pathdef.name )
    mas_free( pathdef.name );
  return path;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *             int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
static int
duf_sql_path_to_pathid( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long *ppathid;

  ppathid = ( unsigned long long * ) sel_cb_udata;
  ( *ppathid ) = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_path_to_pathid( const char *path, unsigned long long *pprevpathid, char **notfound )
{
  int r = 0;
  unsigned long long pathid_new = 0;
  unsigned long long prevpathid = 0;

  if ( path )
  {
    char *bd = NULL;
    unsigned long long pathid = 0;
    char *qname = NULL;
    char *cpath = NULL;

    bd = cpath = mas_strdup( path );

    while ( bd && *bd && *bd == '/' )
      bd++;
    while ( bd && *bd )
    {
      char *ed;

      prevpathid = pathid;
      qname = duf_single_quotes_2( bd );
      ed = bd;
      /* find next '/' */
      while ( ed && *ed && *ed != '/' )
        ed++;
      /* find next name */
      while ( ed && *ed && *ed == '/' )
        *ed++ = 0;
      pathid_new = 0;
      r = duf_sql_select( duf_sql_path_to_pathid, &pathid_new, NULL, 0, "SELECT id FROM duf_paths WHERE parentid='%llu' and dirname='%s'",
                          pathid, qname ? qname : bd );
      if ( r < 0 )
        break;
      if ( !pathid_new )
      {
        if ( notfound && !*notfound )
          *notfound = mas_strdup( bd );
        break;
      }
      if ( 0 )
      {
        char *path = duf_pathid_to_path( pathid_new );

        fprintf( stderr, "%lld => %lld : %s : ed='%s'\n", pathid, pathid_new, path, ed );
        mas_free( path );
      }
      pathid = pathid_new;
      bd = ed;
      mas_free( qname );
      qname = NULL;
    }
    mas_free( cpath );
    cpath = NULL;
    mas_free( qname );
    qname = NULL;
  }
  if ( pprevpathid )
    *pprevpathid = prevpathid;
  return pathid_new;
}

/* 
 * duf_str_cb_t:
 *              int fun( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata ); 
 * */
static int
duf_sql_scan_print_path( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, void *str_cb_udata )
{
  printf( "%7llu: %s\n", pathid, path );
  return 0;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *         int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
 * */
int
duf_sql_scan_paths( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t fuscan )
{
  unsigned long long pathid;
  char *path;

  pathid = strtoll( presult[0], NULL, 10 );
  path = duf_pathid_to_path( pathid );
  if ( fuscan )
  {
/* 
 * duf_str_cb_t:
 *       int fun( pathid, path, filenameid,          name,             str_cb_udata ); 
 * */
    ( *fuscan ) ( pathid, path, 0 /* filenameid */ , NULL /* name */ , NULL /* str_cb_udata */  );
  }
  mas_free( path );

  return 0;
}

int
duf_scan_paths( duf_str_cb_t fun )
{
  int r = 0;

  r = duf_sql_select( duf_sql_scan_paths, NULL, fun, 0, "SELECT id FROM duf_paths WHERE added=%u ORDER BY dirname", 0 );
  return r;
}

int
duf_print_paths( void )
{
  int r = 0;

  duf_scan_paths( duf_sql_scan_print_path );
  return r;
}
