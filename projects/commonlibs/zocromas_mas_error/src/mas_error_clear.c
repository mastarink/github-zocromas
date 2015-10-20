#include <mastar/wrap/mas_memory.h>

#include "mas_error_base.h"
#include "mas_error_regfind.h"
/* ###################################################################### */
#include "mas_error_clear.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */



mas_error_code_t
mas_vclear_error_c( mas_error_code_t re, va_list args )
{
  mas_error_code_t c = 0;

  do
  {
    c = va_arg( args, int );

    {
      char *en1 = mas_strdup( mas_error_name_c( re ) );
      char *en2 = mas_strdup( mas_error_name_c( c ) );

/////      DUF_TRACE( handle_error, 2, "@@@clear (%d) %s ? (%d) %s (2nd from list)", re, en1, c, en2 );
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

mas_error_code_t
mas_clear_error_c( mas_error_code_t re, ... )
{
  va_list args;

  va_start( args, re );
  re = mas_vclear_error_c( re, args );
  va_end( args );
  return re;
}

mas_error_index_t
mas_clear_error_i( mas_error_index_t ri, ... )
{
  va_list args;
  mas_error_code_t c = 0;

  va_start( args, ri );
  if ( ri < 0 )
  {
    c = mas_error_code_i( ri );
/////    DUF_TRACE( handle_error, 2, "c:%d <= ri:%d", c, ri );
    if ( c < 0 )
    {
      c = mas_vclear_error_c( c, args );
/////      DUF_TRACE( handle_error, 2, "@(%d) %s", c, mas_error_name_c( c ) );
      if ( c == 0 )
      {
        mas_delete_error_i( ri );
        ri = 0;
      }
      /* if ( DUF_IS_ERROR( c ) )  */
      /*   DUF_MAKE_ERROR( ri, c ); */
    }
  }
  va_end( args );
/////  DUF_TRACE( handle_error, 2, "@(%d) %s %s", c, mas_error_name_c( c ), mas_error_name_i( ri ) );

  return ri;
}
