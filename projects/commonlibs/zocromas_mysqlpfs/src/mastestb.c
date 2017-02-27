#include <my_global.h>
#include <mysql.h>

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  MYSQL mysql;

  mysql_init( &mysql );
/* MYSQL *mysql_real_connect(MYSQL *mysql, 
 	const char *host,
	const char *user,
	const char *passwd,
	const char *db,
	unsigned int port,
	const char *unix_socket,
	unsigned long client_flag)
 */
  /* mysql_create_db( &mysql, "masdufntdb" ); XXX DEPRECATED XXX */

  if ( mysql_real_connect( &mysql, "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306, NULL, 0 ) == NULL )
  {
    fprintf( stderr, "%s\n", mysql_error( &mysql ) );
    mysql_close( &mysql );
    exit( 1 );
  }

  if ( mysql_query( &mysql, "SHOW STATUS" ) )
  {
    fprintf( stderr, "%s\n", mysql_error( &mysql ) );
    mysql_close( &mysql );
    exit( 1 );
  }

  {
    MYSQL_RES *result = NULL;

/* XXX ?
    store_result() will fetch the whole resultset from the MySQL server
    use_result() will fetch the rows one by one.
*/
#if 0
    result = mysql_store_result( &mysql );
#else
    result = mysql_use_result( &mysql );
#endif
    if ( !result )
    {
      fprintf( stderr, "%s\n", mysql_error( &mysql ) );
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
  mysql_close( &mysql );
  exit( 0 );
}
