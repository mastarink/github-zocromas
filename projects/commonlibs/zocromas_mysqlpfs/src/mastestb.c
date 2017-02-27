#include <my_global.h>
#include <mysql.h>

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  MYSQL *con = mysql_init( NULL );

  if ( con == NULL )
  {
    fprintf( stderr, "%s\n", mysql_error( con ) );
    exit( 1 );
  }

  if ( mysql_real_connect( con, "localhost", "masdufnt", "i2xV9KrTA54HRpj4e", NULL, 0, NULL, 0 ) == NULL )
  {
    fprintf( stderr, "%s\n", mysql_error( con ) );
    mysql_close( con );
    exit( 1 );
  }

  if ( mysql_query( con, "SHOW STATUS" ) )
  {
    fprintf( stderr, "%s\n", mysql_error( con ) );
    mysql_close( con );
    exit( 1 );
  }

  {
    MYSQL_RES *result = mysql_store_result( con );

    if ( !result )
    {
      fprintf( stderr, "%s\n", mysql_error( con ) );
    }
    else
    {
#if 1
#else
      int num_fields = mysql_num_fields( result );
#endif
      MYSQL_ROW row;
      int num = 0;

      while ( ( row = mysql_fetch_row( result ) ) )
      {
#if 1
        printf( "%d #b# %s: %s\n", num, row[0], row[1] );
#else
        printf( "%d ", num );
        for ( int i = 0; i < num_fields; i++ )
        {
          printf( "%s ", row[i] ? row[i] : "NULL" );
        }
        printf( "\n" );
#endif
        num++;
      }

      mysql_free_result( result );
    }
  }
  mysql_close( con );
  exit( 0 );
}
