#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

sqlite3 *db;

int first_row;

int
select_callback( void *p_data, int num_fields, char **p_fields, char **p_col_names )
{

  int i;

  int *nof_records = ( int * ) p_data;

  ( *nof_records )++;

  if ( first_row )
  {
    first_row = 0;

    for ( i = 0; i < num_fields; i++ )
    {
      printf( "%20s", p_col_names[i] );
    }

    printf( "\n" );
    for ( i = 0; i < num_fields * 20; i++ )
    {
      printf( "=" );
    }
    printf( "\n" );
  }

  for ( i = 0; i < num_fields; i++ )
  {
    if ( p_fields[i] )
    {
      printf( "%20s", p_fields[i] );
    }
    else
    {
      printf( "%20s", " " );
    }
  }

  printf( "\n" );
  return 0;
}

void
select_stmt( const char *stmt )
{
  char *errmsg;
  int ret;
  int nrecs = 0;

  first_row = 1;

  ret = sqlite3_exec( db, stmt, select_callback, &nrecs, &errmsg );

  if ( ret != SQLITE_OK )
  {
    printf( "Error in select statement %s [%s].\n", stmt, errmsg );
  }
  else
  {
    printf( "\n   %d records returned.\n", nrecs );
  }
}

void
sql_stmt( const char *stmt )
{
  char *errmsg;
  int ret;

  ret = sqlite3_exec( db, stmt, 0, 0, &errmsg );

  if ( ret != SQLITE_OK )
  {
    printf( "Error in statement: %s [%s].\n", stmt, errmsg );
  }
}

int
main(  )
{
  sqlite3_open( "./bind_insert.db", &db );

  if ( db == 0 )
  {
    printf( "\nCould not open database." );
    return 1;
  }

  sql_stmt( "create table foo (bar, baz)" );

  sqlite3_stmt *stmt;

  if ( sqlite3_prepare( db, "insert into foo values (?,?)", // stmt
                        -1,     // If than zero, then stmt is read up to the first nul terminator
                        &stmt, 0 // Pointer to unused portion of stmt
        ) != SQLITE_OK )
  {
    printf( "\nCould not prepare statement." );
    return 1;
  }

  printf( "\nThe statement has %d wildcards\n", sqlite3_bind_parameter_count( stmt ) );

  if ( sqlite3_bind_double( stmt, 1, // Index of wildcard
                            4.2 ) != SQLITE_OK )
  {
    printf( "\nCould not bind double.\n" );
    return 1;
  }

  if ( sqlite3_bind_int( stmt, 2, // Index of wildcard
                         42 ) != SQLITE_OK )
  {
    printf( "\nCould not bind int.\n" );
    return 1;
  }

  if ( sqlite3_step( stmt ) != SQLITE_DONE )
  {
    printf( "\nCould not step (execute) stmt.\n" );
    return 1;
  }

  sqlite3_reset( stmt );

  if ( sqlite3_bind_null( stmt, 1 // Index of wildcard
        ) != SQLITE_OK )
  {
    printf( "\nCould not bind double.\n" );
    return 1;
  }

  if ( sqlite3_bind_text( stmt, 2, // Index of wildcard
                          "hello", 5, // length of text
                          SQLITE_STATIC ) != SQLITE_OK )
  {
    printf( "\nCould not bind int.\n" );
    return 1;
  }

  if ( sqlite3_step( stmt ) != SQLITE_DONE )
  {
    printf( "\nCould not step (execute) stmt.\n" );
    return 1;
  }

  printf( "\n" );
  select_stmt( "select * from foo" );

  sqlite3_close( db );
  return 0;
}
