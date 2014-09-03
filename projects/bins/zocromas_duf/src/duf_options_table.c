#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

#include "duf_config_types.h"
#include "duf_option_types.h"
#include "duf_options_def.h"
#include "duf_cli_types.h"

#include "duf_options.h"

/* man getopt_long */

static const duf_longval_extended_t _lo_extended[] = {
#include "duf_optable.cdef"
  {0}
};

const duf_longval_extended_t *lo_extended = _lo_extended;
unsigned lo_extended_count = sizeof( _lo_extended ) / sizeof( _lo_extended[0] );

/* static const duf_option_t duf_longopts_table[] = {                                                                                             */
/*   {.name = "add-path",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ADD_PATH},                                                                 */
/* (*{.name = "add-to-group",                      .has_arg = no_argument,                         .val = DUF_OPTION_ADD_TO_GROUP}, *)            */
/*   {.name = "collect",.has_arg = no_argument,.val = DUF_OPTION_FLAG_COLLECT},                                                                   */
/*   {.name = "crc32",.has_arg = no_argument,.val = DUF_OPTION_FLAG_CRC32},                                                                       */
/*   {.name = "crc32id",.has_arg = required_argument,.val = DUF_OPTION_CRC32ID},                                                                  */
/*   {.name = "create-tables",.has_arg = no_argument,.val = DUF_OPTION_FLAG_CREATE_TABLES},                                                       */
/*   {.name = "db-directory",.has_arg = required_argument,.val = DUF_OPTION_DB_DIRECTORY},                                                        */
/*   {.name = "db-name-adm",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME_ADM},                                                          */
/*   {.name = "db-name",.has_arg = required_argument,.val = DUF_OPTION_DB_NAME_MAIN},                                                             */
/*   {.name = "debug",.has_arg = optional_argument,.val = DUF_OPTION_DEBUG},                                                                      */
/*   {.name = "directories",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRS},                                                                  */
/*   {.name = "dirent",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRENT},                                                                     */
/*   {.name = "dirs",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DIRS},                                                                         */
/*   {.name = "disable-calculate",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_CALCULATE},                                               */
/*   {.name = "disable-insert",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_INSERT},                                                     */
/*   {.name = "disable-update",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DISABLE_UPDATE},                                                     */
/*   {.name = "drop-tables",.has_arg = no_argument,.val = DUF_OPTION_FLAG_DROP_TABLES},                                                           */
/*   {.name = "dry-run",.has_arg = no_argument,.val = DUF_OPTION_DRY_RUN_TRACE},                                                                  */
/*   {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},                                                       */
/*   {.name = "exif",.has_arg = no_argument,.val = DUF_OPTION_FLAG_EXIF},                                                                         */
/*   {.name = "exifid",.has_arg = required_argument,.val = DUF_OPTION_EXIFID},                                                                    */
/*   {.name = "explain",.has_arg = optional_argument,.val = DUF_OPTION_EXPLAIN_TRACE},                                                            */
/*   {.name = "filedata",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILEDATA},                                                                 */
/*   {.name = "filenames",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILENAMES},                                                               */
/*   {.name = "files",.has_arg = no_argument,.val = DUF_OPTION_FLAG_FILES},                                                                       */
/*   {.name = "format",.has_arg = required_argument,.val = DUF_OPTION_FORMAT},                                                                    */
/* (*{.name = "group",                             .has_arg = required_argument,                           .val = DUF_OPTION_GROUP}, *)           */
/*   {.name = "help-all",.has_arg = no_argument,.val = DUF_OPTION_HELP_ALL},                                                                      */
/*   {.name = "help-collect",.has_arg = no_argument,.val = DUF_OPTION_HELP_COLLECT},                                                              */
/*   {.name = "help-control",.has_arg = no_argument,.val = DUF_OPTION_HELP_CONTROL},                                                              */
/*   {.name = "help-debug",.has_arg = no_argument,.val = DUF_OPTION_HELP_DEBUG},                                                                  */
/*   {.name = "help-examples",.has_arg = no_argument,.val = DUF_OPTION_EXAMPLES},                                                                 */
/*   {.name = "help",.has_arg = optional_argument,.val = DUF_OPTION_SMART_HELP},                                                                  */
/*   {.name = "help-help",.has_arg = no_argument,.val = DUF_OPTION_HELP_HELP},                                                                    */
/*   {.name = "help-nodesc",.has_arg = no_argument,.val = DUF_OPTION_HELP_NODESC},                                                                */
/*   {.name = "help-print",.has_arg = no_argument,.val = DUF_OPTION_HELP_PRINT},                                                                  */
/*   {.name = "help-reference",.has_arg = no_argument,.val = DUF_OPTION_HELP_REFERENCE},                                                          */
/*   {.name = "help-request",.has_arg = no_argument,.val = DUF_OPTION_HELP_REQUEST},                                                              */
/*   {.name = "help-scan",.has_arg = no_argument,.val = DUF_OPTION_HELP_SCAN},                                                                    */
/*   {.name = "help-short",.has_arg = no_argument,.val = DUF_OPTION_HELP},                                                                        */
/*   {.name = "help-system",.has_arg = no_argument,.val = DUF_OPTION_HELP_SYSTEM},                                                                */
/*   {.name = "help-trace",.has_arg = no_argument,.val = DUF_OPTION_HELP_TRACE},                                                                  */
/*   {.name = "help-update",.has_arg = no_argument,.val = DUF_OPTION_HELP_UPDATE},                                                                */
/*   {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},                                                       */
/*   {.name = "info",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INFO},                                                                         */
/*   {.name = "information",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INFO},                                                                  */
/*   {.name = "integrity",.has_arg = no_argument,.val = DUF_OPTION_FLAG_INTEGRITY},                                                               */
/* (*{.name = "limit",                             .has_arg = required_argument,                           .val = DUF_OPTION_LIMIT}, *)           */
/*   {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},                                                        */
/*   {.name = "max-depth",.has_arg = required_argument,.val = DUF_OPTION_MAXDEPTH},                                                               */
/*   {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},                                                         */
/*   {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},                                                           */
/*   {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},                                                         */
/*   {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},                                                               */
/*   {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},                                                                 */
/*   {.name = "max-seq",.has_arg = required_argument,.val = DUF_OPTION_MAXSEQ},                                                                   */
/*   {.name = "max-size",.has_arg = required_argument,.val = DUF_OPTION_MAXSIZE},                                                                 */
/*   {.name = "md5",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MD5},                                                                           */
/*   {.name = "md5id",.has_arg = required_argument,.val = DUF_OPTION_MD5ID},                                                                      */
/*   {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MDPATH},                                                                     */
/*   {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},                                                                      */
/*   {.name = "mime",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MIME},                                                                         */
/*   {.name = "mimeid",.has_arg = required_argument,.val = DUF_OPTION_MIMEID},                                                                    */
/*   {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},                                                        */
/*   {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},                                                         */
/*   {.name = "min-same",.has_arg = required_argument,.val = DUF_OPTION_MINSAME},                                                                 */
/*   {.name = "min-size",.has_arg = required_argument,.val = DUF_OPTION_MINSIZE},                                                                 */
/*   {.name = "output-file",.has_arg = required_argument,.val = DUF_OPTION_OUTPUT_FILE},                                                          */
/*   {.name = "output-level",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT_LEVEL},                                                        */
/* (*{.name = "print-dirs",                        .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DIRS},   *)            */
/* (*{.name = "print-duplicates",                  .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DUPLICATES}, *)        */
/* (*{.name = "print-files",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_FILES}, *)             */
/*   {.name = "print",.has_arg = no_argument,.val = DUF_OPTION_FLAG_PRINT},                                                                       */
/* (*{.name = "print-paths",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_PATHS}, *)             */
/*   {.name = "progress",.has_arg = no_argument,.val = DUF_OPTION_FLAG_PROGRESS},                                                                 */
/*   {.name = "recursive",.has_arg = no_argument,.val = DUF_OPTION_FLAG_RECURSIVE},                                                               */
/*   {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_FLAG_REMOVE_DATABASE},                                                   */
/* (*{.name = "remove-from-group",                 .has_arg = no_argument,                         .val = DUF_OPTION_REMOVE_FROM_GROUP}, *)       */
/* (*{.name = "same-exif",                         .has_arg = no_argument,                         .val = DUF_OPTION_SAME_EXIF},   *)             */
/* (*{.name = "same-files",                        .has_arg = no_argument,                         .val = DUF_OPTION_SAME_FILES}, *)              */
/*   {.name = "same",.has_arg = required_argument,.val = DUF_OPTION_SAME},                                                                        */
/* (*{.name = "same-md5",                          .has_arg = no_argument,                         .val = DUF_OPTION_SAME_MD5},     *)            */
/*   {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},                                                                    */
/*   {.name = "sampupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD},                                                                  */
/*   {.name = "sd5",.has_arg = no_argument,.val = DUF_OPTION_FLAG_SD5},                                                                           */
/*   {.name = "sd5id",.has_arg = required_argument,.val = DUF_OPTION_SD5ID},                                                                      */
/*   {.name = "size",.has_arg = required_argument,.val = DUF_OPTION_SIZE},                                                                        */
/*   {.name = "summary",.has_arg = no_argument,.val = DUF_OPTION_FLAG_SUMMARY},                                                                   */
/*   {.name = "test-option",.has_arg = required_argument,.val = DUF_OPTION_TEST},                                                                 */
/*   {.name = "trace-action",.has_arg = optional_argument,.val = DUF_OPTION_ACTION_TRACE},                                                        */
/*   {.name = "trace-all",.has_arg = optional_argument,.val = DUF_OPTION_ALL_TRACE},                                                              */
/*   {.name = "trace-any",.has_arg = optional_argument,.val = DUF_OPTION_ANY_TRACE},                                                              */
/*   {.name = "trace-calls",.has_arg = optional_argument,.val = DUF_OPTION_CALLS_TRACE},                                                          */
/*   {.name = "trace-collect",.has_arg = optional_argument,.val = DUF_OPTION_COLLECT_TRACE},                                                      */
/*   {.name = "trace-crc32",.has_arg = optional_argument,.val = DUF_OPTION_CRC32_TRACE},                                                          */
/*   {.name = "trace-current",.has_arg = optional_argument,.val = DUF_OPTION_CURRENT_TRACE},                                                      */
/*   {.name = "trace-deleted",.has_arg = optional_argument,.val = DUF_OPTION_DELETED_TRACE},                                                      */
/*   {.name = "trace-dirent",.has_arg = optional_argument,.val = DUF_OPTION_DIRENT_TRACE},                                                        */
/*   {.name = "trace-error",.has_arg = optional_argument,.val = DUF_OPTION_ERROR_TRACE},                                                          */
/*   {.name = "trace-exif",.has_arg = optional_argument,.val = DUF_OPTION_EXIF_TRACE},                                                            */
/*   {.name = "trace-explain",.has_arg = optional_argument,.val = DUF_OPTION_EXPLAIN_TRACE},                                                      */
/*   {.name = "trace-file",.has_arg = required_argument,.val = DUF_OPTION_TRACE_FILE},                                                            */
/*   {.name = "trace-fs",.has_arg = optional_argument,.val = DUF_OPTION_FS_TRACE},                                                                */
/*   {.name = "trace-insert",.has_arg = optional_argument,.val = DUF_OPTION_INSERT_TRACE},                                                        */
/*   {.name = "trace-integrity",.has_arg = optional_argument,.val = DUF_OPTION_INTEGRITY_TRACE},                                                  */
/*   {.name = "trace-match",.has_arg = optional_argument,.val = DUF_OPTION_MATCH_TRACE},                                                          */
/*   {.name = "trace-md5",.has_arg = optional_argument,.val = DUF_OPTION_MD5_TRACE},                                                              */
/*   {.name = "trace-mdpath",.has_arg = optional_argument,.val = DUF_OPTION_MDPATH_TRACE},                                                        */
/*   {.name = "trace-mime",.has_arg = optional_argument,.val = DUF_OPTION_MIME_TRACE},                                                            */
/*   {.name = "trace-nonew",.has_arg = optional_argument,.val = DUF_OPTION_TRACE_NONEW},                                                          */
/*   {.name = "trace-options",.has_arg = optional_argument,.val = DUF_OPTION_OPTIONS_TRACE},                                                      */
/*   {.name = "trace-path",.has_arg = optional_argument,.val = DUF_OPTION_PATH_TRACE},                                                            */
/*   {.name = "trace-sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE_TRACE},                                                        */
/*   {.name = "trace-samupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD_TRACE},                                                       */
/*   {.name = "trace-scandir",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_DE_DIR_TRACE},                                                  */
/*   {.name = "trace-scan",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_TRACE},                                                            */
/*   {.name = "trace-scanreg",.has_arg = optional_argument,.val = DUF_OPTION_SCAN_DE_REG_TRACE},                                                  */
/*   {.name = "trace-sd5",.has_arg = optional_argument,.val = DUF_OPTION_SD5_TRACE},                                                              */
/*   {.name = "trace-select",.has_arg = optional_argument,.val = DUF_OPTION_SELECT_TRACE},                                                        */
/*   {.name = "trace-seq",.has_arg = optional_argument,.val = DUF_OPTION_SEQ_TRACE},                                                              */
/*   {.name = "trace-sql",.has_arg = optional_argument,.val = DUF_OPTION_SQL_TRACE},                                                              */
/*   {.name = "trace-stderr",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDERR},                                                              */
/*   {.name = "trace-stdout",.has_arg = no_argument,.val = DUF_OPTION_TRACE_STDOUT},                                                              */
/*   {.name = "trace-temp",.has_arg = optional_argument,.val = DUF_OPTION_TEMP_TRACE},                                                            */
/*   {.name = "trace-update",.has_arg = optional_argument,.val = DUF_OPTION_UPDATE_TRACE},                                                        */
/*   {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                     */
/*   {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_FLAG_TREE},                                                                         */
/*   {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                  */
/*   {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_FLAG_UNI_SCAN},                                                                 */
/* (*{.name = "update-exif",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_EXIF}, *)             */
/* (*{.name = "update-mdpath",                     .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH}, *)           */
/* (*{.name = "update-mdpath-selective",           .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, *) */
/* (*{.name = "update-path",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_PATH}, *)             */
/*   {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_FLAG_VACUUM},                                                                     */
/*   {.name = "verbose",.has_arg = optional_argument,.val = DUF_OPTION_VERBOSE},                                                                  */
/*   {.name = "version",.has_arg = no_argument,.val = DUF_OPTION_VERSION},                                                                        */
/*   {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ZERO_DB},                                                                   */
/* (*{.name = "zero-duplicates",                   .has_arg = no_argument,                         .val = DUF_OPTION_ZERO_DUPLICATES}, *)         */
/*   {0},                                                                                                                                         */
/* };                                                                                                                                             */
/*                                                                                                                                                */
/* const duf_option_t *duf_longopts = &duf_longopts_table[0];                                                                                     */
/* unsigned duf_longopts_count = sizeof( duf_longopts_table ) / sizeof( duf_longopts_table[0] );                                                  */
