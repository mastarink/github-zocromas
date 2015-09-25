#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#pragma lib "SQLite3.lib"

static int
callback( void *NotUsed, int argc, char **argv, char **azColName )
{
  NotUsed = 0;
  int i;

  for ( i = 0; i < argc; i++ )
  {
    printf( "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL" );
  }
  printf( "\n" );
  return 0;
}

int
main( int argc, char **argv )
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open( "test.db3", &db );
  if ( rc == SQLITE_OK )
  {
    rc = sqlite3_exec( db, "CREATE TABLE test (id INTEGER NOT NULL, text VARCHAR(100))", callback, 0, &zErrMsg );
    if ( rc != SQLITE_OK )
      fprintf( stderr, "SQL error: %s\n", zErrMsg );
    rc = sqlite3_exec( db, "INSERT INTO test VALUES (1, 'text1')", callback, 0, &zErrMsg );
    if ( rc != SQLITE_OK )
      fprintf( stderr, "SQL error: %s\n", zErrMsg );
    rc = sqlite3_exec( db, "SELECT * FROM test", callback, 0, &zErrMsg );
    if ( rc != SQLITE_OK )
      fprintf( stderr, "SQL error: %s\n", zErrMsg );
    sqlite3_close( db );
  }
  else
  {
    fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg( db ) );
    sqlite3_close( db );
    return 1;
  }
  return 0;
}
