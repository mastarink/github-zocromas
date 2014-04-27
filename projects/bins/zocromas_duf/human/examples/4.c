#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "mysqlite.h"

int
main( int argc, char **argv )
{
  sqlite3 *db;
  char *sql;
  sqlite3_stmt *stmt;
  int nrecs;
  char *errmsg;
  int i;
  int row = 0;
  int arg;

  CALL_SQLITE( open( "/home/ben/test.db", &db ) );
  sql = "SELECT * FROM t WHERE xyz = ?";
  CALL_SQLITE( prepare_v2( db, sql, strlen( sql ) + 1, &stmt, NULL ) );

  for ( arg = 1; arg < argc; arg++ )
  {
    CALL_SQLITE( bind_text( stmt, 1, /* The number of the argument. */
                            argv[arg], strlen( argv[arg] ), 0 /* The callback. */
                  ) );
    while ( 1 )
    {
      int s;

      s = sqlite3_step( stmt );
      if ( s == SQLITE_ROW )
      {
        int bytes;
        const unsigned char *text;

        bytes = sqlite3_column_bytes( stmt, 0 );
        text = sqlite3_column_text( stmt, 0 );
        printf( "%d: %s\n", row, text );
        row++;
      }
      else if ( s == SQLITE_DONE )
      {
        break;
      }
      else
      {
        fprintf( stderr, "Failed.\n" );
        exit( 1 );
      }
    }
    CALL_SQLITE( reset( stmt ) );
    CALL_SQLITE( clear_bindings( stmt ) );
  }
  return 0;
}
