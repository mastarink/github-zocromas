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

#include "duf_error_regfind.h"
/* ###################################################################### */
#include "duf_errors.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */

/* get err number by error code */
int
duf_errnumber_c( duf_error_code_t rc )
{
  DUF_TRACE( error, 5, "calc error number from error code %d => %d (%s)", rc, rc - DUF_ERROR_ERROR_BASE, duf_error_name_c( rc ) );
  return ( rc - DUF_ERROR_ERROR_BASE );
}

/* get err number by error unique id ~ index */
int
duf_errnumber_i( duf_error_index_t ri )
{
  return duf_errnumber_c( duf_error_code_i( ri ) );
}

/* get error code by error list pointer */
duf_error_code_t
duf_error_code_rev( const duf_error_event_t * rev )
{
  duf_error_code_t rc = DUF_ERROR_UNDEFINED;

  if ( rev )
    rc = rev->code;
  return rc;
}

duf_error_code_t
duf_error_code_p( size_t rp )
{
  return duf_error_code_rev( duf_find_error_event_p( rp ) );
}

/* get error code by error unique id ~ index */
duf_error_code_t
duf_error_code_i( duf_error_index_t ri )
{
  return duf_error_code_rev( duf_find_error_event_i( ri ) );
}

/* get err name by error code */
const char *
duf_error_name_c( duf_error_code_t c )
{
  const char *pserr = NULL;

  /* static char buf[2048]; */
  int found = 0;

  for ( int i = 0; i < duf_error_descriptions_table_size; i++ )
  {
    DUF_TRACE( error, 10, "c:%d t[%d]:%d", c, i, duf_error_descriptions_table[i].code );
    if ( c == duf_error_descriptions_table[i].code )
    {
      DUF_TRACE( error, 7, "c:%d t[%d]:%d => %s", c, i, duf_error_descriptions_table[i].code, duf_error_descriptions_table[i].name );
#if 0
      snprintf( buf, sizeof( buf ), "%s", duf_error_descriptions_table[i].name + 4 );
      pserr = buf;
#else
      pserr = duf_error_descriptions_table[i].name + 4;
#endif
      found = 1;
      break;
    }
  }
  if ( !found )
    pserr = "Unknown errcode";
  /* snprintf( buf, sizeof( buf ), "Unknown errcode %d", c ); */
  return pserr;
}

/* get err name by error list pointer */
const char *
duf_error_name_rev( const duf_error_event_t * rev )
{
  const char *name = NULL;

  if ( rev )
    name = duf_error_name_c( duf_error_code_rev( rev ) );
  return name;
}

/* get err name by error list position */
const char *
duf_error_name_p( size_t rp )
{
  return duf_error_name_rev( duf_find_error_event_p( rp ) );
}

/* get err name by error unique id ~ index */
const char *
duf_error_name_i( duf_error_index_t ri )
{
  return duf_error_name_rev( duf_find_error_event_i( ri ) );
}

/* get err func by error list pointer */
const char *
duf_error_func_rev( const duf_error_event_t * rev )
{
  const char *funcid;

  if ( rev )
    funcid = rev->funcid;
  return funcid;
}

/* get err func by error list position */
const char *
duf_error_func_p( size_t rp )
{
  return duf_error_func_rev( duf_find_error_event_p( rp ) );
}

/* get err func by error unique id ~ index */
const char *
duf_error_func_i( duf_error_index_t ri )
{
  return duf_error_func_rev( duf_find_error_event_i( ri ) );
}

/* get err line by error list pointer */
int
duf_error_line_rev( const duf_error_event_t * rev )
{
  unsigned line = 0;

  if ( rev )
    line = rev->linid;
  return line;
}

/* get err line by error list position */
int
duf_error_line_p( size_t rp )
{
  return duf_error_line_rev( duf_find_error_event_p( rp ) );
}

/* get err line by error unique id ~ index */
int
duf_error_line_i( duf_error_index_t ri )
{
  return duf_error_line_rev( duf_find_error_event_i( ri ) );
}

/* get err message by error list pointer */
const char *
duf_error_message_rev( const duf_error_event_t * rev )
{
  const char *msg = NULL;

  if ( rev )
    msg = rev->message;
  return msg ? msg : "";
}

/* get err message by error list position */
const char *
duf_error_message_p( size_t rp )
{
  return duf_error_message_rev( duf_find_error_event_p( rp ) );
}

/* get err message by error unique id ~ index */
const char *
duf_error_message_i( duf_error_index_t ri )
{
  return duf_error_message_rev( duf_find_error_event_i( ri ) );
}

/* get err reported counter by error list pointer */
int
duf_icount_reported_rev( const duf_error_event_t * rev )
{
  int cnt = 0;

  if ( rev )
    cnt = rev->count_reported;
  /* msg = global_error_list[-ri - 1].message; */
  return cnt;
}

/* get err reported counter by error unique id ~ index */
int
duf_icount_reported_i( duf_error_index_t ri )
{
  return duf_icount_reported_rev( duf_find_error_event_i( ri ) );
}
