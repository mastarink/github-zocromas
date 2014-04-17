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
  {.name = "help",.has_arg = no_argument,.val = DUF_OPTION_HELP},
  {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_HELP_EXAMPLES},
  /* --------------- */
  {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},
  {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},
  {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},
  {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_ALL},
  {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},
  {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},
  {.name = "trace-fill",.has_arg = optional_argument,.val = DUF_OPTION_FILL_TRACE},
  {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},
  {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},
  {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},
  {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},
  {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},
  {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},
  {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},
  {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},
  /* --------------- */
  {.name = "verbose",.has_arg = required_argument,.val = DUF_OPTION_VERBOSE},
  {.name = "debug",.has_arg = no_argument,.val = DUF_OPTION_DEBUG},
  {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},
  {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},
  {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},
  /* --------------- */
  {.name = "totals",.has_arg = optional_argument,.val = DUF_OPTION_TOTALS},
  /* --------------- */
  {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},
  {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME},
  {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DB},
  {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_DROP_TABLES},
  {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_CREATE_TABLES},
  {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},
  {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},

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
  /* --------------- */
  {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_UNI_SCAN},
  {.name = "fill",.has_arg = no_argument,.val = DUF_OPTION_FILL},
  {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_MD5},
  {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_MDPATH},
  {.name = "sample",.has_arg = no_argument,.val = DUF_OPTION_SAMPLE},
  /* --------------- */
  {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_PRINT},
  {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_TREE},
  {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FILES},
  {.name = "dirs",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  {.name = "directories",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  /* --------------- */
  {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},
  {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},
  {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},
  {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},
  {.name = "max-depth",.has_arg = required_argument,.val = DUF_OPTION_MAXDEPTH},
  {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},
  {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},
  {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},
  {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},
  {.name = "max-seq",.has_arg = required_argument,.val = DUF_OPTION_MAXSEQ},
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
        r = opt;
        break;
      case DUF_OPTION_HELP_EXAMPLES:
        r = opt;
        break;
      case DUF_OPTION_VERBOSE:
        if ( optarg && *optarg )
          duf_config->cli.dbg.verbose = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.dbg.verbose++;
        break;
      case DUF_OPTION_MEMUSAGE:
        {
          extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

          if ( &mas_mem_disable_print_usage )
          {
            mas_mem_disable_print_usage = 0;
          }
        }
        break;
      case DUF_OPTION_DEBUG:
        duf_config->cli.dbg.debug = 1;
        break;
      case DUF_OPTION_TRACE_STDERR:
        duf_config->cli.trace.out = stderr;
        break;
      case DUF_OPTION_TRACE_STDOUT:
        duf_config->cli.trace.out = stdout;
        break;
      case DUF_OPTION_TRACE_FILE:
        if ( optarg )
        {
          struct stat st;

          if ( 0 == stat( optarg, &st ) && ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) )
          {
            fprintf( stderr, "Can't open trace file %s - file exists %llu / %llu chr:%d\n", optarg, ( unsigned long long ) st.st_dev,
                     ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
            exit( 4 );
          }
          else
          {
            duf_config->cli.trace.out = fopen( optarg, "w" );
            if ( !duf_config->cli.trace.out )
            {
              fprintf( stderr, "Can't open trace file %s\n", optarg );
              exit( 4 );
            }
          }
        }
        break;
      case DUF_OPTION_TRACE_ALL:
        if ( optarg && *optarg )
          duf_config->cli.trace.sql = duf_config->cli.trace.fill = duf_config->cli.trace.md5 = duf_config->cli.trace.sample =
                duf_config->cli.trace.scan = strtol( optarg, NULL, 10 );
        else
        {
          duf_config->cli.trace.sql++;
          duf_config->cli.trace.fill++;
          duf_config->cli.trace.md5++;
          duf_config->cli.trace.sample++;
          duf_config->cli.trace.scan++;
        }
        break;
      case DUF_OPTION_TRACE_NONEW:
        if ( optarg && *optarg )
          duf_config->cli.trace.nonew = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.nonew++;
        break;
      case DUF_OPTION_CALLS_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.calls = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.calls++;
        break;
      case DUF_OPTION_ACTION_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.action = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.action++;
        break;
      case DUF_OPTION_ERROR_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.error = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.error++;
        break;
      case DUF_OPTION_SCAN_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.scan = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.scan++;
        break;
      case DUF_OPTION_PATH_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.path = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.path++;
        fprintf( stderr, "###################### %d\n", duf_config->cli.trace.path );
        break;
      case DUF_OPTION_SAMPLE_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.sample = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.sample++;
        break;
      case DUF_OPTION_MDPATH_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.mdpath = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.mdpath++;
        break;
      case DUF_OPTION_MD5_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.md5 = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.md5++;
        break;
      case DUF_OPTION_FILL_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.fill = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.trace.fill++;
        break;
      case DUF_OPTION_SQL_TRACE:
        if ( optarg && *optarg )
          duf_config->cli.trace.sql = strtol( optarg, NULL, 10 );
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
      case DUF_OPTION_TOTALS:
        if ( optarg && *optarg )
          duf_config->cli.totals = strtol( optarg, NULL, 10 );
        else
          duf_config->cli.totals++;
        break;
      case DUF_OPTION_TREE_TO_DB:
        /* -ORifd5 
         * i.e.
         *  --create-tables --uni-scan --recursive --fill --files --dirs --md5 */
        duf_config->cli.act.create_tables = duf_config->cli.act.uni_scan = duf_config->u.recursive = duf_config->cli.act.fill =
              duf_config->cli.act.files = duf_config->cli.act.dirs = duf_config->cli.act.md5 = 1;
        break;
      case DUF_OPTION_ZERO_DB:
        /* --drop-tables --create-tables */
        duf_config->cli.act.create_tables = 1;
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
      case DUF_OPTION_MDPATH:
        duf_config->cli.act.mdpath = 1;
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
      case DUF_OPTION_DIRS:
        duf_config->cli.act.dirs = 1;
        break;
      case DUF_OPTION_MAXSIZE:
        if ( optarg && *optarg )
          duf_config->u.maxsize = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MINSIZE:
        if ( optarg && *optarg )
          duf_config->u.minsize = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXDIRFILES:
        if ( optarg && *optarg )
          duf_config->u.maxdirfiles = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MINDIRFILES:
        if ( optarg && *optarg )
          duf_config->u.mindirfiles = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXDEPTH:
        if ( optarg && *optarg )
          duf_config->u.maxdepth = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXITEMS:
        if ( optarg && *optarg )
          duf_config->u.maxitems.total = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXITEMS_FILES:
        if ( optarg && *optarg )
          duf_config->u.maxitems.files = strtol( optarg, NULL, 10 );
        break;
      case DUF_OPTION_MAXITEMS_DIRS:
        if ( optarg && *optarg )
          duf_config->u.maxitems.dirs = strtol( optarg, NULL, 10 );
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
        fprintf( stderr, "Invalid option -- '%c' optind=%d/%s opt=%u/%c\n", optopt, optind, argv[optind - 1], opt, opt );
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
