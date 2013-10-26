#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sqlite3.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_def.h"

#include "duf_utils.h"


char *
join_path( const char *path, const char *fname )
{
  char *fpath = NULL;

  if ( fname )
  {
    if ( path )
      fpath = strdup( path );
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

char *
path_id_to_path( sqlite3_int64 pathid )
{
  char *path = NULL;
  char *name = NULL;
  sqlite3_int64 parentid = -1;

  /* fprintf( stderr, "((( %llu )))\n", pathid ); */
  {
    int r;
    char *sql;
    int row, column;
    char **presult = NULL;

    sql = sqlite3_mprintf( "SELECT parentid, dirname FROM paths WHERE id=%llu", pathid );
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row == 1 )
      {
        name = mas_strdup( presult[3] );
        parentid = strtol( presult[2], NULL, 10 );
        /* fprintf( stderr, "%llu - %s\n", parentid, name ); */
      }
    }
    else
      SQL_ERR( r );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
  }
  if ( name && parentid >= 0 )
  {
    char *parent = NULL;

    parent = path_id_to_path( parentid );
    path = join_path( parent, name );
    if ( parent )
      mas_free( parent );
  }
  if ( name )
    mas_free( name );
  return path;
}

sqlite3_int64
path_to_path_id( const char *path )
{
  sqlite3_int64 pathid = 0;
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

      ed = bd;
      while ( ed && *ed && *ed != '/' )
        ed++;
      if ( ed && *ed )
        *ed++ = 0;
      {
        int r;
        char *sql;
        int row, column;
        char **presult = NULL;
        char *qname;

        qname = single_quotes_2( bd );

        sql = sqlite3_mprintf( "SELECT id FROM paths WHERE parentid='%llu' and dirname='%s'", pathid, qname ? qname : bd );
        /* fprintf( stderr, ">>>>>> [%s] ed:%s [%02x] {%s}<<<<<<<<\n", bd, ed, ed ? *ed : 0xff, sql ); */
        r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
        if ( r == SQLITE_OK )
        {
          if ( row )
            for ( int ir = column; ir <= column * row; ir += column )
              pathid = strtoll( presult[ir + 0], NULL, 10 );
        }
        free( qname );
        sqlite3_free_table( presult );
        sqlite3_free( sql );
      }
      if ( !*ed )
        break;
      bd = ed;
    }

    /* fprintf( stderr, "{%d:%s} %s\n", __LINE__, __FILE__, path ); */
    free( cpath );
  }
  /* fprintf( stderr, ">>>>>>>[PARENT ID]>>>>>> %llu\n", pathid ); */
  return pathid;
}

sqlite3_int64
file_to_path_id( const char *path, const char *name, sqlite3_int64 * pmd5id )
{
  sqlite3_int64 pathid;
  sqlite3_int64 filenameid;

  pathid = path_to_path_id( path );
  {
    int r;
    char *sql;
    int row, column;
    char **presult = NULL;
    char *qname;

    qname = single_quotes_2( name );

    sql = sqlite3_mprintf( "SELECT filenames.id, md5.id FROM filenames "
                           " LEFT JOIN filedatas on (filenames.dataid=filedatas.id) "
                           " LEFT JOIN md5 on (md5.id=filedatas.md5id) " 
			   " WHERE pathid='%llu' and name='%s'", pathid,
                           qname ? qname : name );
    /* fprintf( stderr, ">>>>>{%s}<<<<<<<<\n", sql ); */
    r = sqlite3_get_table( pDb, sql, &presult, &row, &column, &errmsg );
    if ( r == SQLITE_OK )
    {
      if ( row )
        for ( int ir = column; ir <= column * row; ir += column )
        {
          filenameid = strtoll( presult[ir + 0], NULL, 10 );
          if ( pmd5id )
            *pmd5id = strtoll( presult[ir + 1], NULL, 10 );
        }
    }
    else
      SQL_ERRQ( r, sql );
    free( qname );
    sqlite3_free_table( presult );
    sqlite3_free( sql );
  }
  return filenameid;
}
sqlite3_int64
md5id_of_file( const char *path, const char *name )
{
  sqlite3_int64 md5id = 0;

  /* sqlite3_int64 pathid = 0;  */
  /* sqlite3_int64 pathid2 = 0; */
  sqlite3_int64 filenameid = 0;

  filenameid = file_to_path_id( path, name, &md5id );
  /* pathid = path_to_path_id( path ); */
  /* SAME:  pathid = update_path( path, -1, 0, 0 ); */
  /* fprintf( stderr, "#%llu#%llu# : %s %s\n", pathid, pathid2, path, name ); */

  /* fprintf( stderr, "#%llu#%llu# : %s %s\n", filenameid, md5id, path, name ); */
  return md5id;
}


