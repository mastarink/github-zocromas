#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"

#define MULCONFNT_ALLOCATE__SOURCE_LIST

int
mucs_parse( int argc _uUu_, const char *argv[]_uUu_, const mucs_option_table_list_t * tablist _uUu_ )
{
#ifdef MULCONFNT_ALLOCATE__SOURCE_LIST
  mucs_source_list_t *plist = mucs_source_list_create(  );
#else
  mucs_source_list_t list_ = { 0 };
  mucs_source_list_t *plist = &list_;
#endif

  mucs_source_t *osrc = NULL;

  mucs_source_list_init( plist );
  osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_ENV, 0, "MAS_TEST_ENV", 0, ":", "=", NULL );
  osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_STRING, 0, "first=45 another=37:second=49;third=23", 0, ":", "=", NULL );

  const mucs_prefix_encoder_t _uUu_ pref_ids[MUCS_VARIANTS] = {
    {"-", MUCS_VARIANT_SHORT}
    , {"--", MUCS_VARIANT_LONG}
    , {NULL, MUCS_VARIANT_NONOPT}
  };
  osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, argc, argv, 0, NULL, "=", NULL /* pref_ids */  );
  mucs_source_list_lookup_all( plist, tablist, NULL );

  INFO( "STRING: '%s'\n", mucs_source_load_string( osrc ) );
  mucs_source_list_integrate( plist );
  {
    if ( plist->targ.argc )
    {
      for ( int i = 0; i < plist->targ.argc; i++ )
      {
        WARN( "[%d] list.targ >>>> %d : %s;", __LINE__, i, plist->targ.argv[i] );
      }
    }
  }
#ifdef MULCONFNT_ALLOCATE__SOURCE_LIST
  mucs_source_list_delete( plist );
#else
  mucs_source_list_reset( plist );
#endif

#if 0
  {
    char *envname = "ENVVAR";

    osrc = mucs_open( MUCS_SOURCE_DIRECT, 0, "first=45:next=49", ":", '=', 0, 0 );
    mucs_open( MUCS_SOURCE_ENV, 0, envname, ":" );
    mucs_open( MUCS_SOURCE_ARGV, argc, argv, 0 );
    mucs_open( MUCS_SOURCE_CONFIG, 0, NULL, ";\n" );
    mucs_open( MUCS_SOURCE_FILE, 0, "mas.cfg", ";\n" );
    mucs_open( MUCS_SOURCE_STREAM, 0, stdin, ";\n" );
  }
#endif
/*
 * sources:
 *   1. argv nextarg: iarg++; getarg: argv[iarg]; delim:0
 *   2a. hard-coded config file: open, 
 *   2b. config file: nextarg: fgets; getarg:...; delim: '\n'
 *   3. * FILE
 *   4. env
 *   5. stdin
 *   6. interactive / readline
 * */
  {
    struct stat st_stdin;

    if ( fstat( STDIN_FILENO, &st_stdin ) >= 0 )
    {
      switch ( st_stdin.st_mode & S_IFMT )
      {
      case S_IFBLK:
        WARN( "block device" );
        break;
      case S_IFCHR:
        WARN( "character device" );                                  /* Y */
        break;
      case S_IFDIR:
        WARN( "directory" );
        break;
      case S_IFIFO:
        WARN( "FIFO/pipe" );                                         /* cmd | this */
        break;
      case S_IFLNK:
        WARN( "symlink" );
        break;
      case S_IFREG:
        WARN( "regular file" );                                      /* this < file */
        break;
      case S_IFSOCK:
        WARN( "socket" );
        break;
      default:
        WARN( "unknown?" );
        break;
      }
    }
  }
  return 0;
}
