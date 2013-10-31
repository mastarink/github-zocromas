#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_path.h"
#include "duf_file.h"
#include "duf_utils.h"


char *
duf_single_quotes_2( const char *s )
{
  char *r = NULL;

  if ( strchr( s, '\'' ) )
  {
    size_t len;
    const char *ip;
    char *op;

    len = strlen( s );
    r = mas_malloc( 2 * ( len + 1 ) );
    /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
    ip = s;
    op = r;
    while ( ip && *ip && op )
    {
      if ( *ip == '\'' )
        *op++ = '\'';
      *op++ = *ip;
      ip++;
    }
    *op++ = 0;
  }
  return r;
}
