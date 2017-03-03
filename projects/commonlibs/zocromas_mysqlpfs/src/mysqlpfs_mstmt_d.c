#define RGEMSG mysql_stmt_error(mstmt->stmt)
/* #define RGEMSG mysql_error(&pfs->mysql) */
#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>


#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>

#include "mysqlpfs_structs.h"
/* #include "mysqlpfs.h" */

#include "mysqlpfs_mstmt_d.h"

int
mas_mysqlpfs_mstmt_set_direct_param_string( mysqlpfs_mstmt_t * mstmt, int pos, const char *str )
{
  int r = -1;

  QRGP( mstmt );
  if ( mstmt )
  {
    QRGP( mstmt->binds.param.bind );
    QRGP( mstmt->binds.param.length );
    if ( mstmt->binds.param.bind && mstmt->binds.param.length && pos < mstmt->binds.param.nbind )
    {
      void **albs = mstmt->binds.param.allocated_buffers;

      INFO("(%d) BIND '%s'", pos, str );
      mstmt->binds.param.bind[pos].buffer_type = MYSQL_TYPE_STRING;
      QRGP( albs );
      if ( albs )
      {
        QRGP( albs[pos] );
        if ( albs[pos] )
        {
          mas_free( albs[pos] );
          albs[pos] = NULL;
          mstmt->binds.param.bind[pos].buffer_length = 0;
        }
      }
      {
        mysqlpfs_s_length_t *plength = mstmt->binds.param.length + pos;

        *plength = strlen( str );
        mstmt->binds.param.bind[pos].buffer = ( char * ) str;
      /* mstmt->binds.param.bind[pos].buffer_length = 255; */
/* mstmt->binds.param.bind[pos].is_unsigned = 0; */
        mstmt->binds.param.bind[pos].is_null = 0;
        mstmt->binds.param.bind[pos].length = plength;
      }
      r = 0;
    }
  }
  return r;
}
