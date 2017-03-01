void
example( void )
{
  MYSQL_STMT *stmt = mysql_stmt_init( conn );
  char *query = "INSERT INTO im(userid,nickname,type) values(?,?,?)"; /* <--- don't forget the * */

  int userid = 17;                                                   /* <--- no '' around integer literals, no * either */
  char *nickname = "pisoga";                                         /* <--- use "" for string literals */
  char *type = "yahoo";                                              /* <--- idem */
  unsigned long nickname_len = strlen( nickname );                   /* <--- added */
  unsigned long type_len = strlen( type );                           /* <--- added */

  mysql_stmt_prepare( stmt, query, strlen( query ) );
  printf( "prepare done\n" );

  memset( bind, 0, sizeof( bind ) );                                 /* <---- Add this */

  bind[0].buffer_type = MYSQL_TYPE_LONG;
  bind[0].buffer = ( char * ) &userid;
  bind[0].is_null = 0;
  bind[0].length = 0;

/* STRING PARAM */
  bind[1].buffer_type = MYSQL_TYPE_STRING;
  bind[1].buffer = ( char * ) nickname;                              /* <--- no need to take the address of a string */
  bind[1].buffer_length = STRING_SIZE;
  bind[1].is_null = 0;
  bind[1].length = &nickname_len;                                    /* <--- pass the correct length */

/* SMALLINT PARAM */
  bind[2].buffer_type = MYSQL_TYPE_STRING;
  bind[2].buffer = ( char * ) type;                                  /* <--- no need to take the address of a string */
  bind[2].is_null = 0;                                               /* <--- no need to send a null */
  bind[2].length = &type_len;                                        /* <--- pass the correct length */
//mysql_stmt_prepare(stmt, query, strlen(query));

  if ( mysql_stmt_bind_param( stmt, bind ) )
  {
    fprintf( stderr, " mysql_stmt_bind_param() failed\n" );
    fprintf( stderr, " %s\n", mysql_stmt_error( stmt ) );
    exit( 0 );
  }
  printf( "Bind param done\n" );

  if ( mysql_stmt_execute( stmt ) )
  {
    fprintf( stderr, "%s\n", mysql_error( conn ) );
    exit( 0 );
  }
}
