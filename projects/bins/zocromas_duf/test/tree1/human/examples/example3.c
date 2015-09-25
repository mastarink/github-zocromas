// sqltest.c : Simple SQLite3 program in C by D. Bolton (C) 2013 http://cplus.about.com

#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include <windows.h>

char *dbname = "C:\\devstuff\\devstuff\\cplus\\tutorials\\c\\sqltest\\about.db";
char *insert_sql = "insert into events (idevent, date,idvenue, ideventtype,description) values (?,?,?,?,?)";

sqlite3 *db;
sqlite3_stmt *stmt;
char message[255];

int date;
char *description = "My House";
char *venue;

/* standalone function to return highest idevent + 1 */
int
getmaxidevent(  )
{
  char *sql = "select max(idevent)+1 from events";
  sqlite3_stmt *lstmt;
  int result = sqlite3_prepare_v2( db, sql, strlen( sql ) + 1, &lstmt, NULL );

  if ( result == SQLITE_OK )
  {
    result = sqlite3_step( lstmt );
    if ( result == SQLITE_ROW )
    {                           /* can read data */
      return sqlite3_column_int( lstmt, 0 );
    }
  }
  return -1;                    /* oh oh */
}

int
main( int argc, char *argv[] )
{
  /* open the database */
  int result = sqlite3_open( dbname, &db );
  int maxid;

  if ( result != SQLITE_OK )
  {
    printf( "Failed to open database %s\n\r", sqlite3_errstr( result ) );
    sqlite3_close( db );
    return 1;
  }
  printf( "Opened db %s OK\n\r", dbname );
  maxid = getmaxidevent(  );

  /* prepare the insert sql, leave stmt ready for loop */
  result = sqlite3_prepare_v2( db, insert_sql, strlen( insert_sql ) + 1, &stmt, NULL );
  if ( result != SQLITE_OK )
  {
    printf( "Failed to prepare database %s\n\r", sqlite3_errstr( result ) );
    sqlite3_close( db );
    return 2;
  }

  printf( "SQL prepared ok\n\r" );
  /* bind parameters */
  sqlite3_bind_int( stmt, 1, maxid ); /* best done by autoincreement */
  sqlite3_bind_int( stmt, 2, 41456 ); /* date */
  sqlite3_bind_int( stmt, 3, 4 ); /* idvenue 4 = Echo */
  sqlite3_bind_int( stmt, 4, 1 ); /* ideventtype - disco */
  sqlite3_bind_text( stmt, 5, description, strlen( description ), SQLITE_STATIC );

  printf( "Before - maxid = %d\n\r", maxid );
  /* insert row by running one stmt */
  result = sqlite3_step( stmt );
  maxid = getmaxidevent(  );
  printf( "After - maxid = %d\n\r", maxid );

  /* finish off */
  sqlite3_close( db );

  return 0;
}
