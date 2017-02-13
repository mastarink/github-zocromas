#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"

#define MAST_TRACE(...) if (do_fprintf) fprintf(stderr, "%s\n", __VA_ARGS__)

#define MULCONFNT_ALLOCATE__SOURCE_LIST

int
mulconfnt_parse( int argc, const char *argv[], const config_option_table_list_t * tablist )
{
#ifdef MULCONFNT_ALLOCATE__SOURCE_LIST
  config_source_list_t *plist = mulconfnt_source_list_create(  );
#else
  config_source_list_t list_ = { 0 };
  config_source_list_t *plist = &list_;
#endif

  config_source_desc_t *osrc = NULL;

  mulconfnt_source_list_init( plist );
  osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_STRING, 0, "first=45 another=37:second=49;third=23", ":", "=", NULL );
  osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ENV, 0, "MAS_TEST_ENV", ":", "=", NULL );

  const config_prefix_encoder_t _uUu_ pref_ids[MULCONF_VARIANTS] = {
    {"-", MULCONF_VARIANT_SHORT}
    , {"--", MULCONF_VARIANT_LONG}
    , {NULL, MULCONF_VARIANT_NONOPT}
  };
  osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ARGV, argc, argv, NULL, "=", NULL /* pref_ids */  );
  mulconfnt_source_lookup( osrc, tablist );

  if ( do_fprintf )
    fprintf( stderr, "STRING: '%s'\n", mulconfnt_source_load_string( osrc, 0 ) );
  mulconfnt_source_list_integrate( plist, 0 );
  {
    if ( plist->targ.argc )
    {
      for ( int i = 0; i < plist->targ.argc; i++ )
      {
        if ( do_fprintf )
          fprintf( stderr, "[%d] list.targ >>>> %d : %s;\n", __LINE__, i, plist->targ.argv[i] );
      }
    }
  }
#ifdef MULCONFNT_ALLOCATE__SOURCE_LIST
  mulconfnt_source_list_delete( plist );
#else
  mulconfnt_source_list_close( plist );
#endif

#if 0
  {
    char *envname = "ENVVAR";

    osrc = mulconfnt_open( MULCONF_SOURCE_DIRECT, 0, "first=45:next=49", ":", '=', 0, 0 );
    mulconfnt_open( MULCONF_SOURCE_ENV, 0, envname, ":" );
    mulconfnt_open( MULCONF_SOURCE_ARGV, argc, argv, 0 );
    mulconfnt_open( MULCONF_SOURCE_CONFIG, 0, NULL, ";\n" );
    mulconfnt_open( MULCONF_SOURCE_FILE, 0, "mas.cfg", ";\n" );
    mulconfnt_open( MULCONF_SOURCE_STREAM, 0, stdin, ";\n" );
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
        MAST_TRACE( "block device" );
        break;
      case S_IFCHR:
        MAST_TRACE( "character device" );                            /* Y */
        break;
      case S_IFDIR:
        MAST_TRACE( "directory" );
        break;
      case S_IFIFO:
        MAST_TRACE( "FIFO/pipe" );                                   /* cmd | this */
        break;
      case S_IFLNK:
        MAST_TRACE( "symlink" );
        break;
      case S_IFREG:
        MAST_TRACE( "regular file" );                                /* this < file */
        break;
      case S_IFSOCK:
        MAST_TRACE( "socket" );
        break;
      default:
        MAST_TRACE( "unknown?" );
        break;
      }
    }
  }
  return 0;
}
