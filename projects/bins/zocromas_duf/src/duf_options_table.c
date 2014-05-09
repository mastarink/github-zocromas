#include <sys/types.h>
#include <unistd.h>

#include <getopt.h>
#include "duf_opt_types.h"

/* man getopt_long */
const duf_option_t duf_longopts_table[] = {
/* const struct option longopts[] = { */
  {.name = "help",.has_arg = no_argument,.val = DUF_OPTION_SMART_HELP},
  {.name = "help-short",.has_arg = no_argument,.val = DUF_OPTION_HELP},
  {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_EXAMPLES},
  {.name = "test-option",.has_arg = required_argument /*| optional_argument*/,.val = DUF_OPTION_TEST},
  {.name = "format",.has_arg = required_argument,.val = DUF_OPTION_FORMAT},
  {.name = "output",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT},
  /* --------------- */
  {.name = "disable-insert",.has_arg = no_argument,.val = DUF_OPTION_DISABLE_INSERT},
  {.name = "disable-update",.has_arg = no_argument,.val = DUF_OPTION_DISABLE_UPDATE},
  /* --------------- */
  {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},
  {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},
  {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},
  {.name = "output-file",.has_arg = required_argument,.val = DUF_OPTION_OUTPUT_FILE},
  /* --------------- */
  {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},
  /* --------------- */
  {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_ALL_TRACE},
  /* --------------- */
  {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},
  {.name = "trace-any",.has_arg = optional_argument,.val = DUF_OPTION_ANY_TRACE},
  {.name = "trace-seq",.has_arg = optional_argument,.val = DUF_OPTION_SEQ_TRACE},
  {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},
  {.name = "trace-collect",.has_arg = optional_argument,.val = DUF_OPTION_COLLECT_TRACE},
  {.name = "trace-current",.has_arg = optional_argument,.val = DUF_OPTION_CURRENT_TRACE},
  {.name = "trace-deleted",.has_arg = optional_argument,.val = DUF_OPTION_DELETED_TRACE},
  {.name = "trace-dirent",.has_arg = optional_argument,.val = DUF_OPTION_DIRENT_TRACE},
  {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},
  {.name = "trace-fs",.has_arg = optional_argument,.val = DUF_OPTION_FS_TRACE},
  {.name = "trace-integrity",.has_arg = optional_argument,.val = DUF_OPTION_INTEGRITY_TRACE},
  {.name = "trace-match",.has_arg = optional_argument,.val = DUF_OPTION_MATCH_TRACE},
  {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},
  {.name = "trace-mime",.has_arg = optional_argument,.val = DUF_OPTION_MIME_TRACE},
  {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},
  {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},
  {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},
  {.name = "trace-samupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD_TRACE},
  {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},
  {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},
  {.name = "trace-select",.has_arg = optional_argument,.val = DUF_OPTION_SELECT_TRACE},
  {.name = "trace-insert",.has_arg = optional_argument,.val = DUF_OPTION_INSERT_TRACE},
  {.name = "trace-update",.has_arg = optional_argument,.val = DUF_OPTION_UPDATE_TRACE},
  /* --------------- */
  {.name = "verbose",.has_arg = optional_argument,.val = DUF_OPTION_VERBOSE},
  {.name = "debug",.has_arg = optional_argument,.val = DUF_OPTION_DEBUG},
  {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},
  {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},
  {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},
  /* --------------- */
  {.name = "noopenat",.has_arg = optional_argument,.val = DUF_OPTION_NOOPENAT},
  {.name = "summary",.has_arg = no_argument,.val = DUF_OPTION_SUMMARY},
  /* --------------- */
  {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},
  {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME},
  {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DB},
  {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_DROP_TABLES},
  {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_DATABASE},
  {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_CREATE_TABLES},
  {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},
  {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},

  /* {.name = "zero-duplicates",.has_arg = no_argument,.val = DUF_OPTION_ZERO_DUPLICATES}, */

  {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_ADD_PATH},
  /* {.name = "update-path",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_PATH}, */
  /* {.name = "update-md5",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MD5}, */
  /* {.name = "update-duplicates",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_DUPLICATES}, */
  /* {.name = "update-mdpath",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH}, */
  /* {.name = "update-mdpath-selective",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, */
  {.name = "filedata",.has_arg = no_argument,.val = DUF_OPTION_FILEDATA},
  {.name = "filenames",.has_arg = no_argument,.val = DUF_OPTION_FILENAMES},
  /* {.name = "update-exif",.has_arg = no_argument,.val = DUF_OPTION_UPDATE_EXIF}, */
  {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_RECURSIVE},
  /* --------------- */
  {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_UNI_SCAN},
  {.name = "collect",.has_arg = no_argument,.val = DUF_OPTION_COLLECT},
  {.name = "integrity",.has_arg = no_argument,.val = DUF_OPTION_INTEGRITY},
  {.name = "dirent",.has_arg = no_argument,.val = DUF_OPTION_DIRENT},
  {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_MD5},
  {.name = "mime",.has_arg = no_argument,.val = DUF_OPTION_MIME},
  {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_MDPATH},
  {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},
  {.name = "sampupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD},
  {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_VACUUM},
  /* --------------- */
  {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_PRINT},
  {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_TREE},
  {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FILES},
  {.name = "dirs",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  {.name = "directories",.has_arg = no_argument,.val = DUF_OPTION_DIRS},
  /* --------------- */
  {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},
  {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},
  {.name = "md5id",.has_arg = required_argument,.val = DUF_OPTION_MD5ID},
  {.name = "mimeid",.has_arg = required_argument,.val = DUF_OPTION_MIMEID},
  {.name = "size",.has_arg = required_argument,.val = DUF_OPTION_SIZE},
  {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},
  {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},
  {.name = "same",.has_arg = required_argument,.val = DUF_OPTION_SAME},
  {.name = "min-same",.has_arg = required_argument,.val = DUF_OPTION_MINSAME},
  {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},
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
  /* {.name = "same-files",.has_arg = no_argument,.val = DUF_OPTION_SAME_FILES}, */
  /* {.name = "same-exif",.has_arg = no_argument,.val = DUF_OPTION_SAME_EXIF},   */
  /* {.name = "same-md5",.has_arg = no_argument,.val = DUF_OPTION_SAME_MD5},     */
  {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},
  {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},
  /* {.name = "group",.has_arg = required_argument,.val = DUF_OPTION_GROUP}, */
  /* {.name = "limit",.has_arg = required_argument,.val = DUF_OPTION_LIMIT}, */
  /* {.name = "add-to-group",.has_arg = no_argument,.val = DUF_OPTION_ADD_TO_GROUP}, */
  /* {.name = "remove-from-group",.has_arg = no_argument,.val = DUF_OPTION_REMOVE_FROM_GROUP}, */
  {0},
};

const duf_option_t *duf_longopts = &duf_longopts_table[0];
const int duf_longopts_count = sizeof( duf_longopts_table ) / sizeof( duf_longopts_table[0] );
