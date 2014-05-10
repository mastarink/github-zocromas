#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"


#include "duf_config.h"

#include "duf_sql_def.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_sql_field.h"
/* ###################################################################### */





int
__duf_sql_pos_by_name2( duf_sqlite_stmt_t * pstmt, const char *name )
{
  int pos = -1;

  for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ )
  {
    if ( 0 == strcmp( duf_sql_column_name( pstmt, icol ), name ) )
    {
      pos = icol;
      break;
    }
  }
  return pos;
}

const char *
__duf_sql_str_by_name2( duf_sqlite_stmt_t * pstmt, const char *name )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  if ( pos >= 0 )
    ptr = duf_sql_column_string( pstmt, pos );

  return ptr;
}

unsigned long long
__duf_sql_ull_by_name2( duf_sqlite_stmt_t * pstmt, const char *name )
{
  unsigned long long val = 0;
  int pos = __duf_sql_pos_by_name2( pstmt, name );

  if ( pos >= 0 )
    val = duf_sql_column_long_long( pstmt, pos );
  return val;
}
