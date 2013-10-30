#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* #include <sqlite3.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

/* #include "duf_def.h" */
#include "duf_sql.h"

#include "duf_utils.h"


char *
join_path( const char *path, const char *fname )
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

char *
single_quotes_2( const char *s )
{
  char *r = NULL;

  if ( strchr( s, '\'' ) )
  {
    size_t len;
    const char *ip;
    char *op;

    len = strlen( s );
    r = mas_malloc( 2 * ( len + 1 ) );
    /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
    ip = s;
    op = r;
    while ( ip && *ip && op )
    {
      if ( *ip == '\'' )
        *op++ = '\'';
      *op++ = *ip;
      ip++;
    }
    *op++ = 0;
  }
  return r;
}

static int
duf_sql_scan_path_test( unsigned long long pathid, const char *path )
{
  printf( "%llu: %s\n", pathid, path );
  return 0;
}

int
duf_sql_print_paths( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  unsigned long long pathid;
  char *path;

  pathid = strtoll( presult[0], NULL, 10 );
  path = path_id_to_path( pathid );
  if ( fuscan )
  {
    ( *fuscan ) ( pathid, path );
  }
  mas_free( path );

  return 0;
}

int
print_paths( void )
{
  int r = 0;

  r = duf_sql_select( duf_sql_print_paths, NULL, duf_sql_scan_path_test, 0, "SELECT id FROM duf_paths WHERE added=%u ORDER BY dirname", 0 );
  return r;
}

typedef struct
{
  char *name;
  unsigned long long parentid;
} path_id_to_path_udata_t;
static int
duf_sql_path_id_to_path( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  path_id_to_path_udata_t *pud;

  pud = ( path_id_to_path_udata_t * ) udata;
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
path_id_to_path( unsigned long long pathid )
{
  int r;
  char *path = NULL;
  path_id_to_path_udata_t pathdef = {.name = NULL,.parentid = 0 };

  /* char *name = NULL; */
  /* unsigned long long parentid = -1; */
  /* fprintf( stderr, "pathid:%llu\n", pathid ); */
  r = duf_sql_select( duf_sql_path_id_to_path, &pathdef, NULL, 0, "SELECT parentid, dirname FROM duf_paths WHERE id=%llu", pathid );
  /* fprintf( stderr, "((( %llu )))\n", pathid ); */
  /* {                                                                                           */
  /*   int r;                                                                                    */
  /*   char *sql;                                                                                */
  /*   int row, column;                                                                          */
  /*   char **presult = NULL;                                                                    */
  /*                                                                                             */
  /*   sql = sqlite3_mprintf( "SELECT parentid, dirname FROM duf_paths WHERE id=%llu", pathid ); */
  /*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                      */
  /*   if ( r == SQLITE_OK )                                                                     */
  /*   {                                                                                         */
  /*     if ( row == 1 )                                                                         */
  /*     {                                                                                       */
  /*       name = mas_strdup( presult[3] );                                                      */
  /*       parentid = strtol( presult[2], NULL, 10 );                                            */
  /*       (* fprintf( stderr, "%llu - %s\n", parentid, name ); *)                               */
  /*     }                                                                                       */
  /*   }                                                                                         */
  /*   else                                                                                      */
  /*     SQL_ERR( r );                                                                           */
  /*   sqlite3_free_table( presult );                                                            */
  /*   sqlite3_free( sql );                                                                      */
  /* }                                                                                           */
  /* fprintf( stderr, "pathid:%llu - %s\n", pathdef.parentid, pathdef.name ); */
  if ( r >= 0 && pathdef.name && pathdef.parentid >= 0 )
  {
    char *parent = NULL;

    /* fprintf( stderr, "parentid:%lld - %s\n", pathdef.parentid, pathdef.name ); */
    parent = path_id_to_path( pathdef.parentid );
    path = join_path( parent, pathdef.name );
    if ( parent )
      mas_free( parent );
  }
  if ( pathdef.name )
    mas_free( pathdef.name );
  return path;
}

static int
duf_sql_path_to_path_id( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  unsigned long long *ppathid;

  ppathid = ( unsigned long long * ) udata;
  ( *ppathid ) = strtoll( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
path_to_path_id( const char *path )
{
  int r = 0;
  unsigned long long pathid = 0;
  unsigned long long pathid_new = 0;
  char *cpath = NULL;

  if ( path )
  {
    char *bd = cpath;

    bd = mas_strdup( path );
    while ( bd && *bd && *bd == '/' )
      bd++;
    while ( bd && *bd )
    {
      char *ed;
      char *qname;

      qname = single_quotes_2( bd );
      ed = bd;
      while ( ed && *ed && *ed != '/' )
        ed++;
      if ( ed && *ed )
        *ed++ = 0;
      r = duf_sql_select( duf_sql_path_to_path_id, &pathid_new, NULL, 0, "SELECT id FROM duf_paths WHERE parentid='%llu' and dirname='%s'",
                          pathid, qname ? qname : bd );
      /* {                                                                                                                         */
      /*   int r;                                                                                                                  */
      /*   char *sql;                                                                                                              */
      /*   int row, column;                                                                                                        */
      /*   char **presult = NULL;                                                                                                  */
      /*                                                                                                                           */
      /*   sql = sqlite3_mprintf( "SELECT id FROM duf_paths WHERE parentid='%llu' and dirname='%s'", pathid, qname ? qname : bd ); */
      /*   (* fprintf( stderr, ">>>>>> [%s] ed:%s [%02x] {%s}<<<<<<<<\n", bd, ed, ed ? *ed : 0xff, sql ); *)                       */
      /*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                                                    */
      /*   if ( r == SQLITE_OK )                                                                                                   */
      /*   {                                                                                                                       */
      /*     if ( row )                                                                                                            */
      /*       for ( int ir = column; ir <= column * row; ir += column )                                                           */
      /*         pathid = strtoll( presult[ir + 0], NULL, 10 );                                                                    */
      /*   }                                                                                                                       */
      /*   sqlite3_free_table( presult );                                                                                          */
      /*   sqlite3_free( sql );                                                                                                    */
      /* }                                                                                                                         */
      free( qname );
      if ( !*ed || r < 0 )
        break;
      bd = ed;
    }

    /* fprintf( stderr, "{%d:%s} %s\n", __LINE__, __FILE__, path ); */
    free( cpath );
  }
  /* fprintf( stderr, ">>>>>>>[PARENT ID]>>>>>> %llu\n", pathid ); */
  return pathid;
}

typedef struct
{
  unsigned long long md5id;
  unsigned long long filenameid;
} file_to_path_id_udata_t;
static int
duf_sql_file_to_path_id( int nrow, int nrows, char *presult[], va_list args, void *udata, duf_str_callback_t fuscan )
{
  file_to_path_id_udata_t *pud;

  pud = ( file_to_path_id_udata_t * ) udata;
  pud->filenameid = strtoll( presult[0], NULL, 10 );
  pud->md5id = strtoll( presult[1], NULL, 10 );
  return 0;
}

unsigned long long
file_to_path_id( const char *path, const char *name, unsigned long long *pmd5id )
{
  int r = 0;
  unsigned long long pathid;

  /* unsigned long long filenameid; */
  file_to_path_id_udata_t ud;
  char *qname;

  qname = single_quotes_2( name );
  pathid = path_to_path_id( path );
  ud.md5id = 0;
  ud.filenameid = 0;
  r = duf_sql_select( duf_sql_file_to_path_id, &ud, NULL, 0, "SELECT duf_filenames.id, duf_md5.id FROM duf_filenames "
                      " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
                      " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "
                      " WHERE pathid='%llu' and name='%s'", pathid, qname ? qname : name );
  if ( r >= 0 && pmd5id )
  {
    *pmd5id = ud.md5id;
  }
  /* {                                                                                               */
  /*   int r;                                                                                        */
  /*   char *sql;                                                                                    */
  /*   int row, column;                                                                              */
  /*   char **presult = NULL;                                                                        */
  /*                                                                                                 */
  /*   sql = sqlite3_mprintf( "SELECT duf_filenames.id, duf_md5.id FROM duf_filenames "              */
  /*                          " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) " */
  /*                          " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) "              */
  /*                          " WHERE pathid='%llu' and name='%s'", pathid, qname ? qname : name );  */
  /*   (* fprintf( stderr, ">>>>>{%s}<<<<<<<<\n", sql ); *)                                          */
  /*   r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );                          */
  /*   if ( r == SQLITE_OK )                                                                         */
  /*   {                                                                                             */
  /*     if ( row )                                                                                  */
  /*       for ( int ir = column; ir <= column * row; ir += column )                                 */
  /*       {                                                                                         */
  /*         filenameid = strtoll( presult[ir + 0], NULL, 10 );                                      */
  /*         if ( pmd5id )                                                                           */
  /*           *pmd5id = strtoll( presult[ir + 1], NULL, 10 );                                       */
  /*       }                                                                                         */
  /*   }                                                                                             */
  /*   else                                                                                          */
  /*     SQL_ERRQ( r, sql );                                                                         */
  /*   sqlite3_free_table( presult );                                                                */
  /*   sqlite3_free( sql );                                                                          */
  /* }                                                                                               */
  free( qname );
  return ud.filenameid;
}

unsigned long long
md5id_of_file( const char *path, const char *name )
{
  unsigned long long md5id = 0;

  /* unsigned long long pathid = 0;  */
  /* unsigned long long pathid2 = 0; */
  /* unsigned long long filenameid = 0; */

  /* filenameid = */ ( void ) file_to_path_id( path, name, &md5id );
  /* pathid = path_to_path_id( path ); */
  /* SAME:  pathid = update_path( path, -1, 0, 0 ); */
  /* fprintf( stderr, "#%llu#%llu# : %s %s\n", pathid, pathid2, path, name ); */

  /* fprintf( stderr, "#%llu#%llu# : %s %s\n", filenameid, md5id, path, name ); */
  return md5id;
}
