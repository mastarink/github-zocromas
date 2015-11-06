#include <string.h>

#include "duf_maintenance.h"

#include "duf_sql_positional.h"
#include "duf_sql_prepared.h"


/* ###################################################################### */
#include "duf_sql_field.h"
/* ###################################################################### */

int
__duf_sql_pos_by_name2( duf_stmnt_t * pstmt, const char *name )
{
  int pos = -1;

  for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
  {
    /* T( "@field %s ? %s (%d)", name, duf_sql_column_name( pstmt, icol ), pos ); */
    if ( 0 == strcmp( duf_sql_column_name( pstmt, icol ), name ) )
    {
      pos = icol;
      /* T( "@field %s  found (%d)", name, pos ); */
      break;
    }
  }
  /* T( "@field %s  pos:%d", name, pos ); */
  if ( pos < 0 )
  {
    T( "@field %s not found:", name );
    for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
    {
      T( "@@@- %s", duf_sql_column_name( pstmt, icol ) );
    }
    assert( 0 );
  }
  return pos;
}

const char *
__duf_sql_str_by_name2( duf_stmnt_t * pstmt, const char *name, int opt )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    ptr = duf_sql_column_string( pstmt, pos );

  return ptr;
}

unsigned long long
__duf_sql_ull_by_name2( duf_stmnt_t * pstmt, const char *name, int opt )
{
  unsigned long long val = 0;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  assert( opt || pos >= 0 );

  if ( pos >= 0 )
    val = duf_sql_column_long_long( pstmt, pos );

  return val;
}
