#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>

/* #include "duf_maintenance.h" */
#  include "duf_tracen_maintenance.h"

#include "duf_error_names.h"

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
  if (  /* rp >= 0 && */ rp < global_error_list_size )
    rev = &global_error_list[rp];
  return rev;
}

duf_error_event_t *
duf_find_error_event_i( duf_error_index_t ri )
{
  duf_error_event_t *rev = NULL;

  if ( ri < 0 && global_error_list_size > 0 )
    for ( int i = global_error_list_size - 1; i >= 0; i-- )
    {
      /* T("@@@@@@i:%d",i); */
      if ( global_error_list[i].index == ri )
      {
        rev = &global_error_list[i];
        break;
      }
    }
  return rev;
}

duf_error_index_t
duf_vregister_error_c( duf_error_code_t code, const char *funcid, int linid, const char *fmt, va_list args )
{
  duf_error_index_t ri = 0;
  char message[4096] = "";

  if ( fmt )
    vsnprintf( message, sizeof( message ), fmt, args );
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
      memset( &global_error_list[global_error_list_size], 0, sizeof( global_error_list[global_error_list_size] ) );

      ri = global_error_list[global_error_list_size].index = -++global_error_index;
      assert( ri < 0 );
      global_error_list[global_error_list_size].code = code;
      global_error_list[global_error_list_size].funcid = funcid;
      global_error_list[global_error_list_size].linid = linid;
      global_error_list[global_error_list_size].message = *message ? mas_strdup( message ) : NULL;
      assert( ri < 0 );
      global_error_list_size++;
    }
    /* fprintf( stderr, "global_error_list_size:%ld (%d) %s\n", global_error_list_size, code, duf_error_name_c( code ) ); */
    DUF_TRACE( errors, 5, "made error code:%d; ri:%d; [%ld:%ld]", code, ri, global_error_list_size, MAX_ERRORS );
    assert( ri < 0 );
  }
  else
    ri = code;
  return ri;
}

duf_error_index_t
duf_register_error_c( duf_error_code_t code, const char *funcid, int linid, const char *fmt, ... )
{
  duf_error_index_t ri;
  va_list args;

  va_start( args, fmt );
  ri = duf_vregister_error_c( code, funcid, linid, fmt, args );
  va_end( args );
  return ri;
}

void
duf_delete_error_rev( duf_error_event_t * rev )
{
  if ( rev && global_error_list_size > 0 )
  {
    for ( int i = global_error_list_size - 1; i >= 0; i-- )
    {
      if ( rev == &global_error_list[i] )
      {
        mas_free( rev->message );
        rev->message = NULL;
        if ( global_error_list_size > 1 )
        {
#if 0
          memcpy( rev, rev + 1, ( global_error_list_size - i ) * sizeof( global_error_list[0] ) );
#else
          unsigned k = 0;

          for ( unsigned j = i; j < global_error_list_size; j++ )
          {
            global_error_list[j] = global_error_list[j + 1];
            k++;
          }
          assert( global_error_list_size - i == k );
#endif
        }
        global_error_list_size--;
        break;
      }
    }
  }
}

void
duf_delete_error_i( duf_error_index_t ri )
{
  duf_delete_error_rev( duf_find_error_event_i( ri ) );
}
