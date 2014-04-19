#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_config.h"

#include "duf_option.h"

/* ###################################################################### */
#include "duf_cli_options.h"
/* ###################################################################### */

const struct option longopts[] = {
  {.name = "help",.has_arg = no_argument,.val = DUF_OPTION_HELP},
  {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_HELP_EXAMPLES},
  {.name = "format",.has_arg = required_argument,.val = DUF_OPTION_FORMAT},
  /* --------------- */
  {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},
  {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},
  {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},
  /* --------------- */
  {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},
  /* --------------- */
  {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_ALL},
  /* --------------- */
  {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},
  {.name = "trace-any",.has_arg = optional_argument,.val = DUF_OPTION_ANY_TRACE},
  {.name = "trace-current",.has_arg = optional_argument,.val = DUF_OPTION_CURRENT_TRACE},
  {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},
  {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},
  {.name = "trace-fill",.has_arg = optional_argument,.val = DUF_OPTION_FILL_TRACE},
  {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},
  {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},
  {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},
  {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},
  {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},
  {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},
  /* --------------- */
  {.name = "verbose",.has_arg = optional_argument,.val = DUF_OPTION_VERBOSE},
  {.name = "debug",.has_arg = no_argument,.val = DUF_OPTION_DEBUG},
  {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},
  {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},
  {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},
  /* --------------- */
  {.name = "totals",.has_arg = no_argument,.val = DUF_OPTION_TOTALS},
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
      r = duf_parse_option( opt, optarg );
      if ( r == DUF_ERROR_OPTION )
      {
        fprintf( stderr, "Invalid option -- '%c' optind=%d/%s opt=%u/%c\n", optopt, optind, argv[optind - 1], opt, opt );
        r = optopt ? optopt : opt;
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
