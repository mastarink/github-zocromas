#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H

#  define DUF_OPTIONS_SHORT "hD:N:RvSFQf"
typedef enum
{
  DUF_OPTION_NONE,
  DUF_OPTION_HELP = 'h',
  DUF_OPTION_DB_DIRECTORY = 'D',
  DUF_OPTION_DB_NAME = 'N',
  DUF_OPTION_RECURSIVE = 'R',
  DUF_OPTION_VERBOSE = 'v',
  DUF_OPTION_SCAN_TRACE = 'S',
  DUF_OPTION_FILL_TRACE = 'F',
  DUF_OPTION_SQL_TRACE = 'Q',
  DUF_OPTION_FILES = 'f',

  DUF_OPTION_LONG = 1000,

  DUF_OPTION_SAMPLE_TRACE,
  DUF_OPTION_MD5_TRACE,
  DUF_OPTION_DEBUG,

  DUF_OPTION_MIN_DBGLINE,
  DUF_OPTION_MAX_DBGLINE,
  DUF_OPTION_CREATE_TABLES,
  DUF_OPTION_DROP_TABLES,
  DUF_OPTION_ADD_PATH,
  /* DUF_OPTION_UPDATE_PATH, */
  /* DUF_OPTION_UPDATE_MD5, */
  DUF_OPTION_ZERO_DUPLICATES,
  DUF_OPTION_UPDATE_DUPLICATES,
  DUF_OPTION_UPDATE_MDPATH,
  DUF_OPTION_UPDATE_MDPATH_SELECTIVE,
  DUF_OPTION_UPDATE_FILEDATA,
  DUF_OPTION_ZERO_FILEDATA,
  DUF_OPTION_UPDATE_EXIF,
  DUF_OPTION_MINSIZE,
  DUF_OPTION_MAXSIZE,
  DUF_OPTION_MAXDEPTH,
  DUF_OPTION_MAXSEQ,
  DUF_OPTION_SAMPLE,
  DUF_OPTION_MD5,
  DUF_OPTION_FILL,
  DUF_OPTION_PRINT,
  DUF_OPTION_TREE,
  DUF_OPTION_UNI_SCAN,
  /* DUF_OPTION_PRINT_PATHS, */
  /* DUF_OPTION_PRINT_DIRS, */
  /* DUF_OPTION_PRINT_FILES, */
  /* DUF_OPTION_PRINT_DUPLICATES, */
  DUF_OPTION_SAME_FILES,
  DUF_OPTION_SAME_EXIF,
  DUF_OPTION_SAME_MD5,
  DUF_OPTION_GROUP,
  DUF_OPTION_LIMIT,
  DUF_OPTION_ADD_TO_GROUP,
  DUF_OPTION_REMOVE_FROM_GROUP,
} duf_option_code_t;

typedef struct
{
  unsigned drop_tables:1;
  unsigned create_tables:1;
  unsigned add_path:1;
  /* unsigned update_path:1; */
  /* unsigned update_md5:1; */
  unsigned update_duplicates:1;
  unsigned update_mdpath:1;
  unsigned update_mdpath_selective:1;
  unsigned update_filedata:1;
  unsigned zero_filedata:1;
  unsigned update_exif:1;
  unsigned sample:1;
  unsigned md5:1;
  unsigned fill:1;
  unsigned print:1;
  unsigned tree:1;
  unsigned files:1;
  unsigned uni_scan:1;
  /* unsigned print_paths:1; */
  /* unsigned print_dirs:1; */
  /* unsigned print_files:1; */
  unsigned print_duplicates:1;
  unsigned zero_duplicates:1;
  unsigned same_files:1;
  unsigned same_md5:1;
  unsigned same_exif:1;
  unsigned to_group:1;
  unsigned from_group:1;
} duf_config_cli_actions_t;

typedef struct
{
  unsigned debug:1;
  unsigned verbose;
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_cli_debug_t;

typedef struct
{
  unsigned scan;
  unsigned sample;
  unsigned md5;
  unsigned fill;
  unsigned sql;
} duf_config_cli_trace_t;

typedef struct
{
  duf_config_cli_actions_t act;
  duf_config_cli_debug_t dbg;
  duf_config_cli_trace_t trace;

  long limit;
} duf_config_cli_t;

#endif
