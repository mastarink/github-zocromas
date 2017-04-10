#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/exam/masexam.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
/* #include "mulconfnt_error.h" */

#include "mastest.h"

int
test_3a( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int variant _uUu_ )
{
  const char *arg;
  char *v_string0 = NULL;
  char *v_string1 = NULL;

  mucs_option_t options[] = {
    {.s = {"string0", 0, MUCS_RTYP_STRING, &v_string0,.flags = MUCS_FLAG_NEED_EQ | MUCS_FLAG_AUTOFREE}}
    , {.s = {"string1", 0, MUCS_RTYP_STRING, &v_string1,.flags = MUCS_FLAG_NEED_EQ | MUCS_FLAG_AUTOFREE}}

    , {.s = {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL}} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.coptions = options, /* */
  };

  const char *xargv[] = {
    argv[0],
    "something",
    "--string0=lorem-ipsum",
    "wow",
    "--string1", "lorem ipsum",                                      /* for string1: MUCS_FLAG_NEED_EQ flag is set, so this should be an error! */
    "abrakadabra",
  };
#define NUM_OPTS 2
/* #define NUM_NOPTS ( sizeof( xargv ) / sizeof( xargv[0] ) - NUM_OPTS ) */
#define NUM_NOPTS 3

  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  {
    FILE *f;
    char fname[128];

    snprintf( fname, sizeof( fname ), "test/mastest_%d%s.commands", nseries, series_suffix );
    f = fopen( fname, "w" );

    if ( f )
    {
      for ( int i = 1; i < xargc; i++ )
      {
        fprintf( f, "%s\n", xargv[i] );
      }
      fclose( f );
    }
  }
  {
    mucs_source_list_t *plist = mucs_source_list_create(  );
    mucs_source_t *osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    masexam_next_group(  );
    EXAMX( plist ? 1 : 0, "plist: %p", plist );
    EXAMX( osrc ? 1 : 0, "osrc: %p", osrc );

    if ( osrc && osrc->oldtarg.argc )
    {
      FILE *f;
      char fname[128];

      snprintf( fname, sizeof( fname ), "test/mastest_%d%s.args", nseries, series_suffix );
      f = fopen( fname, "w" );
      if ( f )
      {
        for ( int i = 1; i < osrc->oldtarg.argc; i++ )
        {
          fprintf( f, "%s\n", osrc->oldtarg.argv[i] );
        }
        fclose( f );
      }
    }
#if 1
    mucs_source_list_lookup_all( plist, &test_tablist, NULL );
#else
    mucs_source_lookup_all( osrc, &test_tablist, NULL );
#endif

    masexam_next_group(  );
  /* EXAMX( mucs_error_source( osrc ),  "mulconfnt_error: %d (last error: \"%s\")", mucs_error_source( osrc ), */
  /*               mucs_error_source_msg( osrc ) );                                                                                           */
    EXAMX( masregerrs_count_all_default( NULL, TRUE ), "mulconfnt_error: %d (last error: \"%s\")",
           masregerrs_count_all_default( NULL, TRUE ), masregerr_last_msg_default( NULL ) );
    masexam_next_group(  );

    EXAMX( v_string0 && 0 == mas_strcmp( v_string0, "lorem-ipsum" ), "string0=%s ? %s", v_string0 ? v_string0 : "<NULL>", "lorem-ipsum" );
#if 0
  /* Don't: Auto-free */
    if ( v_string0 )
      mas_free( v_string0 );
    v_string0 = NULL;
#endif

    EXAMX( !v_string1, "string1=%s ? %s", v_string1 ? v_string1 : "<NULL>", "lorem ipsum" );
#if 0
  /* Don't: Auto-free */
    if ( v_string1 )
      mas_free( v_string1 );
    v_string1 = NULL;
#endif
    masexam_next_group(  );
    EXAMX( mucs_source_argc_no( osrc ) == NUM_NOPTS, "%d ? %d", mucs_source_argc_no( osrc ), NUM_NOPTS );
    arg = mucs_source_arg_no( osrc, 1 );
    EXAMX( arg && 0 == mas_strcmp( "something", arg ), "'%s' ? '%s'", "something", arg );
    arg = mucs_source_arg_no( osrc, 2 );
    EXAMX( arg && 0 == mas_strcmp( "wow", arg ), "'%s' ? '%s'", "wow", arg );
  /* arg = mucs_source_arg_no( osrc, 3 );                                                                     */
  /* EXAMX( arg && 0 == mas_strcmp( "abrakadabra", arg ),  "'%s' ? '%s'", "abrakadabra", arg ); */

    char **argvno = mucs_source_argv_no( osrc );
    int argcount = mucs_source_argc_no( osrc );

  /* WARN( "argcount:%d", argcount ); */
    EXAMX( argcount > 1 && argvno && argvno[1]
           && 0 == mas_strcmp( "something", argvno[1] ), "'%s' ? '%s'", "something", argvno && argcount > 1 ? argvno[1] : "?" );
    EXAMX( argcount > 2 && argvno && argvno[2]
           && 0 == mas_strcmp( "wow", argvno[2] ), "'%s' ? '%s'", "wow", argvno && argcount > 2 ? argvno[2] : "?" );
  /* EXAMX( argcount > 3 && argvno && argvno[3]                                                                                                       */
  /*        && 0 == mas_strcmp( "abrakadabra", argvno[3] ),  "'%s' ? '%s'", "abrakadabra", argvno && argcount > 3 ? argvno[3] : NULL ); */

    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_reset( &test_tablist );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
