#include <assert.h>
/* #include <unistd.h> */
#include <mastar/wrap/mas_memory.h>

/* #  include "mas_maintenance.h" */
/* #  include "mas_tracen_maintenance.h" */

#include "mas_error_names.h"

#include "mas_error_regfind.h"
/* ###################################################################### */
#include "mas_error_base.h"
/* ###################################################################### */

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 5001 ) ) );
static void
constructor_main( int argc MAS_UNUSED, char **argv MAS_UNUSED, char **envp MAS_UNUSED )
{
  fprintf( stderr, "%s : %d\n", __FILE__, argc );
}

/* #define DUF_NOTIMING */

/* get err number by error code */
int
mas_errnumber_c( mas_error_code_t rc )
{
/////  DUF_TRACE( errors, 5, "calc error number from error code %d => %d (%s)", rc, rc - DUF_ERROR_ERROR_BASE, mas_error_name_c( rc ) );
  return ( rc - DUF_ERROR_ERROR_BASE );
}

/* get err number by error unique id ~ index */
int
mas_errnumber_i( mas_error_index_t ri )
{
  return mas_errnumber_c( mas_error_code_i( ri ) );
}

/* get error code by error list pointer */
mas_error_code_t
mas_error_code_rev( const mas_error_event_t * rev )
{
  mas_error_code_t rc = DUF_ERROR_UNDEFINED;

  if ( rev )
    rc = rev->code;
  return rc;
}

mas_error_code_t
mas_error_code_p( size_t rp )
{
  return mas_error_code_rev( mas_find_error_event_p( rp ) );
}

/* get error code by error unique id ~ index */
mas_error_code_t
mas_error_code_i( mas_error_index_t ri )
{
  return mas_error_code_rev( mas_find_error_event_i( ri ) );
}

/* get err name by error code */
const char *
mas_error_name_c( mas_error_code_t c )
{
  const char *pserr = NULL;

/* static char buf[2048]; */
  int found = 0;

  for ( int i = 0; i < mas_error_descriptions_table_size; i++ )
  {
/////    DUF_TRACE( errors, 10, "c:%d t[%d]:%d", c, i, mas_error_descriptions_table[i].code );
    if ( c == mas_error_descriptions_table[i].code )
    {
/////      DUF_TRACE( errors, 3, "c:%d t[%d]:%d => %s", c, i, mas_error_descriptions_table[i].code, mas_error_descriptions_table[i].name );
#if 0
      snprintf( buf, sizeof( buf ), "%s", mas_error_descriptions_table[i].name + 4 );
      pserr = buf;
#else
      pserr = mas_error_descriptions_table[i].name + 4;
#endif
      found = 1;
      break;
    }
  }
  if ( !found )
    pserr = "Unknown errcode";
/* snprintf( buf, sizeof( buf ), "Unknown errcode %d", c ); */
  assert( pserr );
  return pserr;
}

/* get err name by error list pointer */
const char *
mas_error_name_rev( const mas_error_event_t * rev )
{
  const char *name = NULL;

  if ( rev )
    name = mas_error_name_c( mas_error_code_rev( rev ) );
  return name ? name : "-";
}

/* get err name by error list position */
const char *
mas_error_name_p( size_t rp )
{
  return mas_error_name_rev( mas_find_error_event_p( rp ) );
}

/* get err name by error unique id ~ index */
const char *
mas_error_name_i( mas_error_index_t ri )
{
  return mas_error_name_rev( mas_find_error_event_i( ri ) );
}

/* get err func by error list pointer */
const char *
mas_error_func_rev( const mas_error_event_t * rev )
{
  const char *funcid;

  if ( rev )
    funcid = rev->funcid;
  return funcid;
}

/* get err func by error list position */
const char *
mas_error_func_p( size_t rp )
{
  return mas_error_func_rev( mas_find_error_event_p( rp ) );
}

/* get err func by error unique id ~ index */
const char *
mas_error_func_i( mas_error_index_t ri )
{
  return mas_error_func_rev( mas_find_error_event_i( ri ) );
}

/* get err line by error list pointer */
int
mas_error_line_rev( const mas_error_event_t * rev )
{
  unsigned line = 0;

  if ( rev )
    line = rev->linid;
  return line;
}

/* get err line by error list position */
int
mas_error_line_p( size_t rp )
{
  return mas_error_line_rev( mas_find_error_event_p( rp ) );
}

/* get err line by error unique id ~ index */
int
mas_error_line_i( mas_error_index_t ri )
{
  return mas_error_line_rev( mas_find_error_event_i( ri ) );
}

/* get err message by error list pointer */
const char *
mas_error_message_rev( const mas_error_event_t * rev )
{
  const char *msg = NULL;

  if ( rev )
    msg = rev->message;
  return msg ? msg : "";
}

/* get err message by error list position */
const char *
mas_error_message_p( size_t rp )
{
  return mas_error_message_rev( mas_find_error_event_p( rp ) );
}

/* get err message by error unique id ~ index */
const char *
mas_error_message_i( mas_error_index_t ri )
{
  return mas_error_message_rev( mas_find_error_event_i( ri ) );
}

/* set err message by error list pointer */
void
mas_vset_error_message_rev( mas_error_event_t * rev, const char *fmt, va_list args )
{
  if ( rev )
  {
    char message[4096] = "";

    mas_free( rev->message );
    rev->message = NULL;
    if ( fmt )
    {
      vsnprintf( message, sizeof( message ), fmt, args );
      rev->message = mas_strdup( message );
    }
  }
}

/* set err message by error list position */
void
mas_vset_error_message_p( size_t rp, const char *fmt, va_list args )
{
  mas_vset_error_message_rev( mas_find_error_event_p( rp ), fmt, args );
}

/* set err message by error unique id ~ index */
void
mas_vset_error_message_i( mas_error_index_t ri, const char *fmt, va_list args )
{
  mas_vset_error_message_rev( mas_find_error_event_i( ri ), fmt, args );
}

/* set err message by error list pointer */
void
mas_set_error_message_rev( mas_error_event_t * rev, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mas_vset_error_message_rev( rev, fmt, args );
  va_end( args );
}

/* set err message by error list position */
void
mas_set_error_message_p( size_t rp, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mas_vset_error_message_p( rp, fmt, args );
  va_end( args );
}

/* set err message by error unique id ~ index */
void
mas_set_error_message_i( mas_error_index_t ri, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mas_vset_error_message_i( ri, fmt, args );
  va_end( args );
}

/* get err reported counter by error list pointer */
int
mas_icount_reported_rev( const mas_error_event_t * rev )
{
  int cnt = 0;

  if ( rev )
    cnt = rev->count_reported;
/* msg = global_error_list[-ri - 1].message; */
  return cnt;
}

/* get err reported counter by error unique id ~ index */
int
mas_icount_reported_i( mas_error_index_t ri )
{
  return mas_icount_reported_rev( mas_find_error_event_i( ri ) );
}

__attribute__ ( ( destructor( 50000 ) ) )
     static void destructor_errorlist( void )
{

#define DUF_MEM_ROW " ▤▤▤▤▤▤▤▤▤▤▤▤▤ "

#if 0
# ifdef MAS_TRACEMEM
  print_memlist_msg( FL, stdout,                                     /* */
                     "\n\x1b[0;1;7;44;35m" DUF_MEM_ROW "<  1.",      /* */
                     "\n\x1b[0;1;7;46;37m" DUF_MEM_ROW "<  2.",      /* */
                     "\x1b[0;1;7;32m    MEMORY FREE    ",            /* */
                     "\x1b[0;1;7;44;35m  >" DUF_MEM_ROW "\x1b[0m", "\n\n\x1b[0;1;33;41m                              MEMORY TABLE                              \x1b[0m" /* */
           );
# endif
#endif
}
