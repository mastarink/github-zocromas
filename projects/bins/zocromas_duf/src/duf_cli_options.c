#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_config.h"


/* ###################################################################### */
#include "duf_cli_options.h"
/* ###################################################################### */
typedef enum
{
  DUF_OPTION_NONE,
  DUF_OPTION_HELP = 'h',
  DUF_OPTION_VERBOSE = 'v',
  DUF_OPTION_DB_DIRECTORY = 'D',
  DUF_OPTION_DB_NAME = 'F',
  DUF_OPTION_LONG = 1000,
  DUF_OPTION_CREATE_TABLES,
  DUF_OPTION_DROP_TABLES,
  DUF_OPTION_ADD_PATH,
  DUF_OPTION_UPDATE_PATH,
  DUF_OPTION_UPDATE_MD5,
  DUF_OPTION_ZERO_DUPLICATES,
  DUF_OPTION_UPDATE_DUPLICATES,
  DUF_OPTION_UPDATE_MDPATH,
  DUF_OPTION_UPDATE_MDPATH_SELECTIVE,
  DUF_OPTION_UPDATE_FILEDATA,
  DUF_OPTION_ZERO_FILEDATA,
  DUF_OPTION_UPDATE_EXIF,
  DUF_OPTION_RECURSIVE,
  DUF_OPTION_PRINT_PATHS,
  DUF_OPTION_PRINT_DIRS,
  DUF_OPTION_PRINT_FILES,
  /* DUF_OPTION_PRINT_DUPLICATES, */
  DUF_OPTION_SAME_FILES,
  DUF_OPTION_SAME_EXIF,
  DUF_OPTION_SAME_MD5,
  DUF_OPTION_GROUP,
  DUF_OPTION_LIMIT,
  DUF_OPTION_ADD_TO_GROUP,
  DUF_OPTION_REMOVE_FROM_GROUP,
} duf_option_code_t;

const struct option longopts[] = {
  {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},
  {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME},
  {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_DROP_TABLES},
  {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_CREATE_TABLES},
  {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_ADD_PATH},
  {.name = "update-path",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_PATH},
  {.name = "update-md5",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MD5},
  {.name = "zero-duplicates",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DUPLICATES},
  {.name = "update-duplicates",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_DUPLICATES},
  {.name = "update-mdpath",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH},
  {.name = "update-mdpath-selective",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE},
  {.name = "update-filedata",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_FILEDATA},
  {.name = "zero-filedata",.has_arg = no_argument,.val = DUF_OPTION_ZERO_FILEDATA},
  {.name = "update-exif",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_EXIF},
  {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_RECURSIVE},
  {.name = "print-paths",.has_arg = no_argument,.val = DUF_OPTION_PRINT_PATHS},
  {.name = "print-dirs",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DIRS},
  {.name = "print-files",.has_arg = no_argument,.val = DUF_OPTION_PRINT_FILES},
  /* {.name = "print-duplicates",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DUPLICATES}, */
  {.name = "same-files",.has_arg = no_argument,.val = DUF_OPTION_SAME_FILES},
  {.name = "same-exif",.has_arg = no_argument,.val = DUF_OPTION_SAME_EXIF},
  {.name = "same-md5",.has_arg = no_argument,.val = DUF_OPTION_SAME_MD5},
  {.name = "group",.has_arg = required_argument,.val = DUF_OPTION_GROUP},
  {.name = "verbose",.has_arg = required_argument,.val = DUF_OPTION_VERBOSE},
  {.name = "limit",.has_arg = required_argument,.val = DUF_OPTION_LIMIT},
  {.name = "add-to-group",.has_arg = no_argument,.val = DUF_OPTION_ADD_TO_GROUP},
  {.name = "remove-from-group",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_FROM_GROUP},
  {.name = NULL,.has_arg = no_argument,.val = DUF_OPTION_NONE},
};

int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

  if ( duf_config )
  {
    int opt;
    int longindex = 0;

    opterr = 0;
    while ( r == 0 && ( opt = getopt_long( argc, argv, "hD:F:", longopts, &longindex ) ) >= 0 )
    {
      /* fprintf( stderr, "%d OPT:%d; LONGINDEX:%d\n", optind, opt, longindex ); */
      switch ( opt )
      {
      case DUF_OPTION_HELP:
        break;
      case DUF_OPTION_VERBOSE:
        duf_config->verbose = 1;
        break;
      case DUF_OPTION_DROP_TABLES:
        duf_config->drop_tables = 1;
        break;
      case DUF_OPTION_CREATE_TABLES:
        duf_config->create_tables = 1;
        break;
      case DUF_OPTION_ADD_PATH:
        duf_config->add_path = 1;
        break;
      case DUF_OPTION_UPDATE_PATH:
        duf_config->update_path = 1;
        break;
      case DUF_OPTION_UPDATE_MD5:
        duf_config->update_md5 = 1;
        break;
      case DUF_OPTION_UPDATE_DUPLICATES:
        duf_config->update_duplicates = 1;
        break;
      case DUF_OPTION_UPDATE_MDPATH:
        duf_config->update_mdpath = 1;
        break;
      case DUF_OPTION_UPDATE_MDPATH_SELECTIVE:
        duf_config->update_mdpath_selective = 1;
        break;
      case DUF_OPTION_ZERO_FILEDATA:
        duf_config->zero_filedata = 1;
        break;
      case DUF_OPTION_UPDATE_FILEDATA:
        duf_config->update_filedata = 1;
        break;
      case DUF_OPTION_UPDATE_EXIF:
        duf_config->update_exif = 1;
        break;
      case DUF_OPTION_RECURSIVE:
        duf_config->recursive = 1;
        break;
      case DUF_OPTION_PRINT_PATHS:
        duf_config->print_paths = 1;
        break;
      case DUF_OPTION_PRINT_DIRS:
        duf_config->print_dirs = 1;
        break;
      case DUF_OPTION_PRINT_FILES:
        duf_config->print_files = 1;
        break;
        /* case DUF_OPTION_PRINT_DUPLICATES:   */
        /*   duf_config->print_duplicates = 1; */
        /*   break;                            */
      case DUF_OPTION_ZERO_DUPLICATES:
        duf_config->zero_duplicates = 1;
        break;
      case DUF_OPTION_SAME_FILES:
        duf_config->same_files = 1;
        break;
      case DUF_OPTION_SAME_EXIF:
        duf_config->same_exif = 1;
        break;
      case DUF_OPTION_SAME_MD5:
        duf_config->same_md5 = 1;
        break;
      case DUF_OPTION_ADD_TO_GROUP:
        duf_config->to_group = 1;
        break;
      case DUF_OPTION_REMOVE_FROM_GROUP:
        duf_config->from_group = 1;
        break;
      case DUF_OPTION_GROUP:
        if ( optarg )
        {
          mas_free( duf_config->group );
          duf_config->group = mas_strdup( optarg );
        }
        break;
      case DUF_OPTION_DB_DIRECTORY:
        if ( optarg )
        {
          mas_free( duf_config->db_dir );
          duf_config->db_dir = mas_strdup( optarg );
        }
        break;
      case DUF_OPTION_DB_NAME:
        if ( optarg )
        {
          mas_free( duf_config->db_name );
          duf_config->db_name = mas_strdup( optarg );
        }
        break;
      case DUF_OPTION_LIMIT:
        {
          long limit = 0;

          if ( sscanf( optarg, "%ld", &limit ) > 0 )
            duf_config->limit = limit;
        }
        break;
      case '?':
        printf( "Invalid option -- '%c'\n", optopt );
        r = optopt;
        break;
      }
    }
    if ( optind < argc )
    {
      mas_del_argv( duf_config->targc, duf_config->targv, 0 );
      duf_config->targc = 0;
      duf_config->targv = NULL;

      duf_config->targc = mas_add_argv_argv( duf_config->targc, &duf_config->targv, argc, argv, optind );
      /* fprintf( stderr, ">>>>>>>> targc: %d; argc: %d; optind: %d\n", duf_config->targc, argc, optind ); */
      /* for ( int ia = 0; ia < duf_config->targc; ia++ )                                                  */
      /* {                                                                                                 */
      /*   fprintf( stderr, ">>>>>>>> targv[%d]: %s\n", ia, duf_config->targv[ia] );                       */
      /* }                                                                                                 */
    }
  }
  return r;
}
