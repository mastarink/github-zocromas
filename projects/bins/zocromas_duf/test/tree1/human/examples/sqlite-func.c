/*
 * gcc  -shared -fPIC -Isqlite3 -lssl  -o sqlite_digest.ext sqlite_digest.c
 *
 *
sqlite3 [optional database name]
sqlite>SELECT load_extension('./sqlite_digest.ext');
sqlite>SELECT md2('test');
dd34716876364a02d0195e2fb9ae2d1b
sqlite>SELECT md5('test');
098f6bcd4621d373cade4e832627b4f6
sqlite>SELECT sha('test');
f8d3b312442a67706057aeb45b983221afb4f035
sqlite>SELECT sha1('test');
a94a8fe5ccb19ba61c4c0873d391e987982fbbd3
sqlite>SELECT ripemd160('test');
5e52fee47e6b070565f74372468cdc699de89107
* 
 *
 * */


  int
sqlite3_extension_init( sqlite3 * db, char **pzErrMsg, const sqlite3_api_routines * pApi )
{
  SQLITE_EXTENSION_INIT2( pApi ) sqlite3_create_function( db, "md2", 1, SQLITE_ANY, 0, md2, 0, 0 );
  sqlite3_create_function( db, "md5", 1, SQLITE_ANY, 0, md5, 0, 0 );
  sqlite3_create_function( db, "sha", 1, SQLITE_ANY, 0, sha, 0, 0 );
  sqlite3_create_function( db, "sha1", 1, SQLITE_ANY, 0, sha1, 0, 0 );
// ** Not Used as it didn't seem to be available in my OpenSSL
//  sqlite3_create_function(db, "mdc2", 1, SQLITE_ANY, 0, mdc2, 0, 0);
  sqlite3_create_function( db, "ripemd160", 1, SQLITE_ANY, 0, ripemd160, 0, 0 );
  return 0;
}

static void[function_name] ( sqlite3_context * context, int argc, sqlite3_value ** argv )
{
  unsigned char md_value[EVP_MAX_MD_SIZE];
  int md_len, i;

  if ( argc != 1 )
  {
    sqlite3_result_null( context );
    return;
  }
  EVP_MD_CTX ctx;

  EVP_MD_CTX_init( &amp;
                   ctx );
  if ( EVP_DigestInit_ex( &amp; ctx, EVP_[function_name] (  ), NULL ) )
  {
    char *input = 0;
    char *output = 0;

    input = ( char * ) sqlite3_value_text( argv[0] );
    EVP_DigestUpdate( &amp;
                      ctx, input, strlen( input ) );
    EVP_DigestFinal_ex( &amp;
                        ctx, md_value, &amp;
                        md_len );
    output = ( char * ) malloc( md_len * 2 + 1 );
    if ( !output )
    {
      sqlite3_result_null( context );
      EVP_MD_CTX_cleanup( &amp;
                          ctx );
      return;
    }
    for ( i = 0; i & lt; md_len; i++ )
    {
      sprintf( &amp;
               output[i * 2], "%02x", ( unsigned char ) md_value[i] );
    }
    sqlite3_result_text( context, output, strlen( output ), ( void * ) -1 );
    free( output );
    output = 0;
  }
  else
  {
    sqlite3_result_null( context );
  }
  EVP_MD_CTX_cleanup( &amp;
                      ctx );
}
