#include "duf_maintenance.h"

#include "duf_error_regfind.h"
/* ###################################################################### */
#include "duf_errors_clear.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */



duf_error_code_t
duf_vclear_error_c( duf_error_code_t re, va_list args )
{
  duf_error_code_t c = 0;

  do
  {
    c = va_arg( args, int );

    {
      char *en1 = mas_strdup( duf_error_name_c( re ) );
      char *en2 = mas_strdup( duf_error_name_c( c ) );

      DUF_TRACE( handle_error, 2, "@@@clear (%d) %s ? (%d) %s (2nd from list)", re, en1, c, en2 );
      mas_free( en1 );
      mas_free( en2 );
    }
    if ( re == c )
    {
      re = 0;
      break;
    }
  }
  while ( c );
  return re;
}

duf_error_code_t
duf_clear_error_c( duf_error_code_t re, ... )
{
  va_list args;

  va_start( args, re );
  re = duf_vclear_error_c( re, args );
  va_end( args );
  return re;
}

duf_error_index_t
duf_clear_error_i( duf_error_index_t ri, ... )
{
  va_list args;
  duf_error_code_t c = 0;

  va_start( args, ri );
  if ( DUF_IS_ERROR( ri ) )
  {
    c = duf_error_code_i( ri );
    DUF_TRACE( handle_error, 2, "c:%d <= ri:%d", c, ri );
    if ( DUF_IS_ERROR( c ) )
    {
      c = duf_vclear_error_c( c, args );
      DUF_TRACE( handle_error, 2, "@(%d) %s", c, duf_error_name_c( c ) );
      if ( c == 0 )
      {
        duf_delete_error_i( ri );
        ri = 0;
      }
      /* if ( DUF_IS_ERROR( c ) )  */
      /*   DUF_MAKE_ERROR( ri, c ); */
    }
  }
  va_end( args );
  DUF_TRACE( handle_error, 2, "@(%d) %s %s", c, duf_error_name_c( c ), duf_error_name_i( ri ) );

  return ri;
}
