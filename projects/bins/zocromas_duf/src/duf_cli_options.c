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

const struct option longopts[] = {
  {.name = "debug",.has_arg = no_argument,.val = DUF_OPTION_DEBUG},
  {.name = "trace-scan",.has_arg = required_argument,.val = DUF_OPTION_SCAN_TRACE},
  {.name = "trace-fill",.has_arg = required_argument,.val = DUF_OPTION_FILL_TRACE},
  {.name = "trace-sql",.has_arg = required_argument,.val = DUF_OPTION_SQL_TRACE},
  {.name = "trace-md5",.has_arg = required_argument,.val = DUF_OPTION_MD5_TRACE},
  {.name = "verbose",.has_arg = required_argument,.val = DUF_OPTION_VERBOSE},

  {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},
  {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},

  {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},
  {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME},
  {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_DROP_TABLES},
  {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_CREATE_TABLES},

  {.name = "zero-duplicates",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DUPLICATES},
  {.name = "zero-filedata",.has_arg = no_argument,.val = DUF_OPTION_ZERO_FILEDATA},

  {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_ADD_PATH},
  /* {.name = "update-path",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_PATH}, */
  /* {.name = "update-md5",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MD5}, */
  {.name = "update-duplicates",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_DUPLICATES},
  {.name = "update-mdpath",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH},
  {.name = "update-mdpath-selective",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE},
  {.name = "update-filedata",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_FILEDATA},
  {.name = "update-exif",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_EXIF},
  {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_RECURSIVE},
  {.name = "sample",.has_arg = no_argument,.val = DUF_OPTION_SAMPLE},
  {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_MD5},
  {.name = "fill",.has_arg = no_argument,.val = DUF_OPTION_FILL},
  {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_PRINT},
  {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_TREE},
  {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FILES},
  {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},
  {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},
  {.name = "max-depth",.has_arg = required_argument,.val = DUF_OPTION_MAXDEPTH},
  {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXSEQ},
  {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_UNI_SCAN},
  /* {.name = "print-paths",.has_arg = no_argument,.val = DUF_OPTION_PRINT_PATHS}, */
  /* {.name = "print-dirs",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DIRS},   */
  /* {.name = "print-files",.has_arg = no_argument,.val = DUF_OPTION_PRINT_FILES}, */
  /* {.name = "print-duplicates",.has_arg = no_argument,.val = DUF_OPTION_PRINT_DUPLICATES}, */
  {.name = "same-files",.has_arg = no_argument,.val = DUF_OPTION_SAME_FILES},
  {.name = "same-exif",.has_arg = no_argument,.val = DUF_OPTION_SAME_EXIF},
  {.name = "same-md5",.has_arg = no_argument,.val = DUF_OPTION_SAME_MD5},
  {.name = "group",.has_arg = required_argument,.val = DUF_OPTION_GROUP},
  {.name = "limit",.has_arg = required_argument,.val = DUF_OPTION_LIMIT},
  {.name = "add-to-group",.has_arg = no_argument,.val = DUF_OPTION_ADD_TO_GROUP},
  {.name = "remove-from-group",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_FROM_GROUP},
  {.name = NULL,.has_arg = no_argument,.val = DUF_OPTION_NONE},
};

int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  if ( duf_config )
  {
    int opt;
    int longindex = 0;

    opterr = 0;
    while ( r == 0 && ( opt = getopt_long( argc, argv, DUF_OPTIONS_SHORT, longopts, &longindex ) ) >= 0 )
    {
      /* fprintf( stderr, "%d OPT:%d; LONGINDEX:%d\n", optind, opt, longindex ); */
      switch ( opt )
      {
      case DUF_OPTION_HELP:
        break;
      case DUF_OPTION_VERBOSE:
        if ( optarg && *optarg )
          duf_config->cli.dbg.verbose = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.dbg.verbose++;
        break;
      case DUF_OPTION_DEBUG:
        duf_config->cli.dbg.debug = 1;
        break;
      case DUF_OPTION_SCAN_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.scan= strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.scan++;
        break;
      case DUF_OPTION_SAMPLE_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.sample= strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.sample++;
        break;
      case DUF_OPTION_MD5_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.md5= strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.md5++;
        break;
      case DUF_OPTION_FILL_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.fill= strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.fill++;
        break;
      case DUF_OPTION_SQL_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.sql= strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.sql++;
        break;
      case DUF_OPTION_MIN_DBGLINE:
        if ( optarg && *optarg )
          duf_config->cli.dbg.min_line = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAX_DBGLINE:
        if ( optarg && *optarg )
          duf_config->cli.dbg.max_line = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_DROP_TABLES:
        duf_config->cli.act.drop_tables = 1;
        break;
      case DUF_OPTION_CREATE_TABLES:
        duf_config->cli.act.create_tables = 1;
        break;
      case DUF_OPTION_ADD_PATH:
        duf_config->cli.act.add_path = 1;
        break;
        /* case DUF_OPTION_UPDATE_PATH:   */
        /*   duf_config->cli.act.update_path = 1; */
        /*   break;                       */
        /* case DUF_OPTION_UPDATE_MD5:   */
        /*   duf_config->cli.act.update_md5 = 1; */
        /*   break;                      */
      case DUF_OPTION_UPDATE_DUPLICATES:
        duf_config->cli.act.update_duplicates = 1;
        break;
      case DUF_OPTION_UPDATE_MDPATH:
        duf_config->cli.act.update_mdpath = 1;
        break;
      case DUF_OPTION_UPDATE_MDPATH_SELECTIVE:
        duf_config->cli.act.update_mdpath_selective = 1;
        break;
      case DUF_OPTION_ZERO_FILEDATA:
        duf_config->cli.act.zero_filedata = 1;
        break;
      case DUF_OPTION_UPDATE_FILEDATA:
        duf_config->cli.act.update_filedata = 1;
        break;
      case DUF_OPTION_UPDATE_EXIF:
        duf_config->cli.act.update_exif = 1;
        break;
      case DUF_OPTION_RECURSIVE:
        duf_config->u.recursive = 1;
        break;
      case DUF_OPTION_SAMPLE:
        duf_config->cli.act.sample = 1;
        break;
      case DUF_OPTION_MD5:
        duf_config->cli.act.md5 = 1;
        break;
      case DUF_OPTION_FILL:
        duf_config->cli.act.fill = 1;
        break;
      case DUF_OPTION_PRINT:
        duf_config->cli.act.print = 1;
        break;
      case DUF_OPTION_TREE:
        duf_config->cli.act.tree = 1;
        break;
      case DUF_OPTION_FILES:
        duf_config->cli.act.files = 1;
        break;
      case DUF_OPTION_MAXSIZE:
        if ( optarg && *optarg )
          duf_config->u.maxsize = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MINSIZE:
        if ( optarg && *optarg )
          duf_config->u.minsize = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXDEPTH:
        if ( optarg && *optarg )
          duf_config->u.maxdepth = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXSEQ:
        if ( optarg && *optarg )
          duf_config->u.maxseq = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_UNI_SCAN:
        duf_config->cli.act.uni_scan = 1;
        break;
        /* case DUF_OPTION_PRINT_PATHS:   */
        /*   duf_config->cli.act.print_paths = 1; */
        /*   break;                       */
        /* case DUF_OPTION_PRINT_DIRS:    */
        /*   duf_config->cli.act.print_dirs = 1;  */
        /*   break;                       */
        /* case DUF_OPTION_PRINT_FILES:   */
        /*   duf_config->cli.act.print_files = 1; */
        /*   break;                       */
        /* case DUF_OPTION_PRINT_DUPLICATES:   */
        /*   duf_config->cli.act.print_duplicates = 1; */
        /*   break;                            */
      case DUF_OPTION_ZERO_DUPLICATES:
        duf_config->cli.act.zero_duplicates = 1;
        break;
      case DUF_OPTION_SAME_FILES:
        duf_config->cli.act.same_files = 1;
        break;
      case DUF_OPTION_SAME_EXIF:
        duf_config->cli.act.same_exif = 1;
        break;
      case DUF_OPTION_SAME_MD5:
        duf_config->cli.act.same_md5 = 1;
        break;
      case DUF_OPTION_ADD_TO_GROUP:
        duf_config->cli.act.to_group = 1;
        break;
      case DUF_OPTION_REMOVE_FROM_GROUP:
        duf_config->cli.act.from_group = 1;
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
        if ( optarg && *optarg )
          duf_config->cli.limit = strtol( optarg, NULL, 10 );
        break;
      case '?':
        printf( "Invalid option -- '%c' optind=%d/%s opt=%u/%c\n", optopt, optind, argv[optind - 1], opt, opt );
        r = optopt ? optopt : opt;
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
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  return r;
}
