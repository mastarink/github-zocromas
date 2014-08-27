#ifndef MAS_DUF_OPTIONS_DEF_H
#  define MAS_DUF_OPTIONS_DEF_H

#  define DUF_CONFIG_PATH_FROM_ENV "MSH_CONF_DIR"
#  define DUF_CONFIG_FILE_NAME "zocromas_duf.conf"

typedef enum
{
  DUF_OPTION_NONE,

  DUF_OPTION_SMART_HELP = 'h',
  DUF_OPTION_HELP = 'H',
  DUF_OPTION_EXAMPLES = 'x',
  DUF_OPTION_TEST = 't',
  DUF_OPTION_VERBOSE = 'v',
  DUF_OPTION_DEBUG = 'g',
  DUF_OPTION_VERSION = 'V',
  DUF_OPTION_MEMUSAGE = 'M',
/* db general */
  DUF_OPTION_DB_DIRECTORY = 'Y',
  DUF_OPTION_FLAG_ZERO_DB = 'Z',
  DUF_OPTION_FLAG_CREATE_TABLES = 'O',
  DUF_OPTION_FLAG_INTEGRITY = 'G',

  /* trace */
  DUF_OPTION_ACTION_TRACE = 'A',
  DUF_OPTION_SCAN_TRACE = 'S',
  DUF_OPTION_TEMP_TRACE = '$',
  DUF_OPTION_COLLECT_TRACE = 'I',
  DUF_OPTION_ANY_TRACE = 'y',
  DUF_OPTION_MATCH_TRACE = 'm',
  /* /trace */

  DUF_OPTION_FLAG_RECURSIVE = 'R',
  DUF_OPTION_TREE_TO_DB = 'B',
  DUF_OPTION_FLAG_ADD_PATH = 'P',

  /* actions */
  DUF_OPTION_FLAG_TREE = 'T',
  DUF_OPTION_FLAG_PRINT = 'p',
  DUF_OPTION_FLAG_SUMMARY = 's',

  /* actions */
  DUF_OPTION_FLAG_COLLECT = 'i',
  DUF_OPTION_FLAG_FILES = 'f',
  DUF_OPTION_FLAG_DIRS = 'd',
  DUF_OPTION_FLAG_DIRENT = 'E',

  DUF_OPTION_FLAG_FILEDATA = 'D',
  DUF_OPTION_FLAG_FILENAMES = 'n',
  DUF_OPTION_FLAG_SD5 = '2',
  DUF_OPTION_FLAG_MD5 = '5',
  DUF_OPTION_FLAG_MIME = 'e',
  DUF_OPTION_FLAG_EXIF = 'X',


/*==================================================*/
  DUF_OPTION_LONG = 1000,
/*==================================================*/
  DUF_OPTION_HELP_NONE,
  DUF_OPTION_HELP_ALL,

  DUF_OPTION_HELP_HELP,
  DUF_OPTION_HELP_SYSTEM,
  DUF_OPTION_HELP_CONTROL,
  DUF_OPTION_HELP_REFERENCE,
  DUF_OPTION_HELP_COLLECT,
  DUF_OPTION_HELP_SCAN,
  DUF_OPTION_HELP_UPDATE,
  DUF_OPTION_HELP_REQUEST,
  DUF_OPTION_HELP_PRINT,

  DUF_OPTION_HELP_TRACE,
  DUF_OPTION_HELP_DEBUG,
  DUF_OPTION_HELP_OBSOLETE,
  DUF_OPTION_HELP_NODESC,
  DUF_OPTION_HELP_OTHER,

  DUF_OPTION_SCAN_DE_DIR_TRACE,
  DUF_OPTION_SCAN_DE_REG_TRACE,

  DUF_OPTION_FLAG_CRC32,

/* db general */
  DUF_OPTION_DB_NAME_MAIN,
  DUF_OPTION_DB_NAME_ADM,
  DUF_OPTION_FLAG_REMOVE_DATABASE,
  DUF_OPTION_FLAG_DROP_TABLES,
  DUF_OPTION_FLAG_VACUUM,

  DUF_OPTION_FLAG_PROGRESS,

  /* i/o */
  DUF_OPTION_OUTPUT_LEVEL,
  DUF_OPTION_OUTPUT_FILE,
  DUF_OPTION_FORMAT,

  /* execution control */
  DUF_OPTION_FLAG_DISABLE_CALCULATE,
  DUF_OPTION_FLAG_DISABLE_INSERT,
  DUF_OPTION_FLAG_DISABLE_UPDATE,

  /* trace */
  DUF_OPTION_DRY_RUN_TRACE,
  DUF_OPTION_EXPLAIN_TRACE,

  DUF_OPTION_ERROR_TRACE,
  DUF_OPTION_CURRENT_TRACE,
  DUF_OPTION_SEQ_TRACE,
  DUF_OPTION_OPTIONS_TRACE,
  DUF_OPTION_CALLS_TRACE,
  DUF_OPTION_DELETED_TRACE,
  DUF_OPTION_DIRENT_TRACE,
  DUF_OPTION_FS_TRACE,
  DUF_OPTION_INTEGRITY_TRACE,
  DUF_OPTION_SD5_TRACE,
  DUF_OPTION_MD5_TRACE,
  DUF_OPTION_CRC32_TRACE,
  DUF_OPTION_MIME_TRACE,
  DUF_OPTION_EXIF_TRACE,
  DUF_OPTION_MDPATH_TRACE,
  DUF_OPTION_PATH_TRACE,
  DUF_OPTION_SAMPLE_TRACE,
  DUF_OPTION_SAMPUPD_TRACE,
  DUF_OPTION_SQL_TRACE,
  DUF_OPTION_SELECT_TRACE,
  DUF_OPTION_INSERT_TRACE,
  DUF_OPTION_UPDATE_TRACE,
  DUF_OPTION_ALL_TRACE,

  DUF_OPTION_TRACE_NONEW,

  /* trace i/o */
  DUF_OPTION_TRACE_FILE,
  DUF_OPTION_TRACE_STDERR,
  DUF_OPTION_TRACE_STDOUT,

  /* debug */
  DUF_OPTION_MIN_DBGLINE,
  DUF_OPTION_MAX_DBGLINE,

  DUF_OPTION_FLAG_DRY_RUN,
  DUF_OPTION_FLAG_EXPLAIN,

  /* actions */
  DUF_OPTION_SAMPLE,
  DUF_OPTION_SAMPUPD,
  DUF_OPTION_FLAG_MDPATH,
/* /actions */

  /* DUF_OPTION_UPDATE_PATH, */
  /* DUF_OPTION_UPDATE_MD5, */
  /* DUF_OPTION_ZERO_DUPLICATES, */
  /* DUF_OPTION_UPDATE_DUPLICATES, */
  /* DUF_OPTION_UPDATE_MDPATH, */
  /* DUF_OPTION_UPDATE_MDPATH_SELECTIVE, */
  /* DUF_OPTION_UPDATE_FILEDATA, */
  /* DUF_OPTION_ZERO_FILEDATA, */
  /* DUF_OPTION_UPDATE_EXIF, */

  /* filters */
  DUF_OPTION_MINDIRFILES,
  DUF_OPTION_MAXDIRFILES,
  DUF_OPTION_GLOB_INCLUDE_FILES,
  DUF_OPTION_GLOB_EXCLUDE_FILES,
  DUF_OPTION_SD5ID,
  DUF_OPTION_MD5ID,
  DUF_OPTION_CRC32ID,
  DUF_OPTION_MIMEID,
  DUF_OPTION_EXIFID,
  DUF_OPTION_SIZE,
  DUF_OPTION_MINSIZE,
  DUF_OPTION_MAXSIZE,
  DUF_OPTION_SAME,
  DUF_OPTION_MINSAME,
  DUF_OPTION_MAXSAME,
  DUF_OPTION_MAXDEPTH,
  DUF_OPTION_MAXSEQ,
  DUF_OPTION_MAXITEMS,
  DUF_OPTION_MAXITEMS_FILES,
  DUF_OPTION_MAXITEMS_DIRS,
  /* /filters */

  DUF_OPTION_FLAG_UNI_SCAN,
  DUF_OPTION_FLAG_INFO,

  /* DUF_OPTION_PRINT_PATHS, */
  /* DUF_OPTION_PRINT_DIRS, */
  /* DUF_OPTION_PRINT_FILES, */
  /* DUF_OPTION_PRINT_DUPLICATES, */
  /* DUF_OPTION_SAME_FILES, */
  /* DUF_OPTION_SAME_EXIF, */
  /* DUF_OPTION_SAME_MD5, */
  /* DUF_OPTION_GROUP, */
  /* DUF_OPTION_LIMIT, */
  /* DUF_OPTION_ADD_TO_GROUP, */
  /* DUF_OPTION_REMOVE_FROM_GROUP, */
  DUF_OPTION_MAX_LONG,
} duf_option_code_t;



#endif
