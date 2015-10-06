#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_maintenance.h"

#include "duf_error_names.h"

/* #include "duf_sql_const.h" */

/* ###################################################################### */
#include "duf_error_regfind.h"
/* ###################################################################### */

#define MAX_ERRORS 100000l

static size_t global_error_list_size = 0;
static duf_error_index_t global_error_index = 0;
static duf_error_event_t *global_error_list = NULL;

__attribute__ ( ( destructor( 65535 ) ) )
     static void destructor_errorlist( void )
{

  for ( int i = global_error_list_size - 1; i >= 0; i-- )
  {
    mas_free( global_error_list[i].message );
    global_error_list[i].message = NULL;
  }
  mas_free( global_error_list );
  global_error_list = NULL;
  global_error_list_size = 0;
}

size_t
duf_error_list_size( void )
{
  return global_error_list_size;
}

duf_error_event_t *
duf_find_error_event_p( size_t rp )
{
  duf_error_event_t *rev = NULL;

  /* assert( rp >= 0 ); */
  if ( /* rp >= 0 && */ rp < global_error_list_size )
    rev = &global_error_list[rp];
  return rev;
}

duf_error_event_t *
duf_find_error_event_i( duf_error_index_t ri )
{
  duf_error_event_t *rev = NULL;

  assert( ri < 0 );
  if ( global_error_list_size > 0 )
    for ( int i = global_error_list_size - 1; i >= 0; i-- )
    {
      /* T("@@@@@@i:%d",i); */
      if ( global_error_list[i].index == ri )
      {
        rev = &global_error_list[i];
        break;
      }
    }
  assert(rev);
  return rev;
}

duf_error_index_t
duf_register_error_c( duf_error_code_t code, const char *funcid, int linid, const char *message )
{
  duf_error_index_t ri = 0;

  if ( code < 0 )
  {
    if ( !global_error_list )
    {
      global_error_list = mas_malloc( sizeof( duf_error_event_t ) * MAX_ERRORS );
      memset( global_error_list, 0, sizeof( duf_error_event_t ) * MAX_ERRORS );
    }
    assert( global_error_list_size < MAX_ERRORS );
    if ( global_error_list_size < MAX_ERRORS )
    {
      ri = global_error_list[global_error_list_size].index = -++global_error_index;
      assert( ri < 0 );
      global_error_list[global_error_list_size].code = code;
      global_error_list[global_error_list_size].funcid = funcid;
      global_error_list[global_error_list_size].linid = linid;
      global_error_list[global_error_list_size].message = message ? mas_strdup( message ) : NULL;
      assert( ri < 0 );
      global_error_list_size++;
    }
    /* fprintf( stderr, "global_error_list_size:%ld (%d) %s\n", global_error_list_size, code, duf_error_name_c( code ) ); */
    DUF_TRACE( error, 5, "made error code:%d; ri:%d; [%ld:%ld]", code, ri, global_error_list_size, MAX_ERRORS );
    assert( ri < 0 );
  }
  else
    ri = code;
  return ri;
}

void
duf_delete_error_rev( duf_error_event_t * rev )
{
  if ( rev && global_error_list_size > 0 )
    for ( int i = global_error_list_size - 1; i >= 0; i-- )
    {
      if ( rev == &global_error_list[i] )
      {
        mas_free( rev->message );
        if ( global_error_list_size > 1 )
          memcpy( rev, rev + 1, ( global_error_list_size - 1 - i ) * sizeof( global_error_list[0] ) );
        global_error_list_size--;
        break;
      }
    }
}

void
duf_delete_error_i( duf_error_index_t ri )
{
  duf_delete_error_rev( duf_find_error_event_i( ri ) );
}
