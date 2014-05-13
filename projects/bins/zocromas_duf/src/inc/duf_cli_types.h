#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H

typedef struct
{
  int val;
  const char *help;
} duf_longval_extended_t;

typedef struct
{
  unsigned dataid:1;
  unsigned dirid:1;
  unsigned filename:1;
  unsigned filesize:1;
  unsigned gid:1;
  unsigned human:1;
  unsigned inode:1;
  unsigned sd5:1;
  unsigned sd5id:1;
  unsigned md5:1;
  unsigned md5id:1;
  unsigned crc32:1;
  unsigned crc32id:1;
  unsigned mimeid:1;
  unsigned exifid:1;
  unsigned mode:1;
  unsigned mtime:1;
  unsigned nlink:1;
  unsigned prefix:1;
  unsigned realpath:1;
  unsigned seq:1;
  unsigned truepath:1;
  unsigned uid:1;
  unsigned short_filename:1;
  unsigned nsame;
  unsigned offset;
} duf_format_t;


typedef enum
{
  DUF_FORMAT_DATAID,
  DUF_FORMAT_DIRID,
  DUF_FORMAT_FILENAME,
  DUF_FORMAT_FILESIZE,
  DUF_FORMAT_GID,
  DUF_FORMAT_HUMAN,
  DUF_FORMAT_INODE,
  DUF_FORMAT_SD5,
  DUF_FORMAT_SD5ID,
  DUF_FORMAT_MD5,
  DUF_FORMAT_MD5ID,
  DUF_FORMAT_CRC32,
  DUF_FORMAT_CRC32ID,
  DUF_FORMAT_MIMEID,
  DUF_FORMAT_EXIFID,
  DUF_FORMAT_MODE,
  DUF_FORMAT_MTIME,
  DUF_FORMAT_NLINK,
  DUF_FORMAT_NSAME,
  DUF_FORMAT_OFFSET,
  DUF_FORMAT_PREFIX,
  DUF_FORMAT_REALPATH,
  DUF_FORMAT_SEQ,
  DUF_FORMAT_TRUEPATH,
  DUF_FORMAT_UID,
  DUF_FORMAT_MAX,
} duf_format_index_t;


typedef enum
{
  DUF_TRACE_MODE_NONE,
  DUF_TRACE_MODE_seq,
  DUF_TRACE_MODE_action,
  DUF_TRACE_MODE_any,
  DUF_TRACE_MODE_collect,
  DUF_TRACE_MODE_current,
  DUF_TRACE_MODE_deleted,
  DUF_TRACE_MODE_dirent,
  DUF_TRACE_MODE_error,
  DUF_TRACE_MODE_errorr,
  DUF_TRACE_MODE_filedata,
  DUF_TRACE_MODE_filenames,
  /* DUF_TRACE_MODE_fill, */
  DUF_TRACE_MODE_fs,
  DUF_TRACE_MODE_match,
  DUF_TRACE_MODE_MAX,
  DUF_TRACE_MODE_sd5,
  DUF_TRACE_MODE_md5,
  DUF_TRACE_MODE_crc32,
  DUF_TRACE_MODE_mdpath,
  DUF_TRACE_MODE_mime,
  DUF_TRACE_MODE_exif,
  DUF_TRACE_MODE_path,
  DUF_TRACE_MODE_sample,
  DUF_TRACE_MODE_sampupd,
  DUF_TRACE_MODE_scan,
  DUF_TRACE_MODE_sql,
  DUF_TRACE_MODE_select,
  DUF_TRACE_MODE_insert,
  DUF_TRACE_MODE_update,
  DUF_TRACE_MODE_statistics,
  DUF_TRACE_MODE_verbose,
} duf_trace_mode_t;


typedef struct
{
  unsigned remove_database:1;
  unsigned drop_tables:1;
  unsigned create_tables:1;
  unsigned add_path:1;
  /* unsigned update_path:1; */
  /* unsigned update_md5:1; */
  unsigned update_duplicates:1;
  /* unsigned update_mdpath:1; */
  /* unsigned update_mdpath_selective:1; */
  unsigned mdpath:1;
  unsigned vacuum:1;

  unsigned dirs:1;
  unsigned files:1;

  unsigned dirent:1;
  unsigned filedata:1;
  unsigned filenames:1;
  unsigned sd5:1;
  unsigned md5:1;
  unsigned crc32:1;
  unsigned mime:1;
  unsigned exif:1;

  unsigned integrity:1;
  unsigned collect:1;

  unsigned print:1;
  unsigned tree:1;

  unsigned uni_scan:1;
  /* unsigned print_paths:1; */
  /* unsigned print_dirs:1; */
  /* unsigned print_files:1; */
  /* unsigned print_duplicates:1; */
  /* (* unsigned zero_duplicates:1; *) */
  /* unsigned same_files:1; */
  /* unsigned same_md5:1; */
  /* unsigned same_exif:1; */
  /* unsigned to_group:1; */
  /* unsigned from_group:1; */
  unsigned progress:1;
  unsigned summary:1;
  unsigned sample;
  unsigned sampupd;
} duf_config_cli_actions_t;

typedef struct
{
  unsigned debug;
  unsigned nosqlite;
  unsigned verbose;
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_cli_debug_t;

typedef struct
{
  unsigned action;
  unsigned any;
  unsigned calls;
  unsigned collect;
  unsigned current;
  unsigned deleted;
  unsigned dirent;
  unsigned error;
  unsigned errorr;
  unsigned dirs;
  unsigned filedata;
  unsigned filenames;
  /* unsigned fill; */
  unsigned fs;
  unsigned integrity;
  unsigned match;
  unsigned sd5;
  unsigned md5;
  unsigned crc32;
  unsigned mdpath;
  unsigned mime;
  unsigned exif;
  unsigned nonew;
  unsigned path;
  unsigned sample;
  unsigned sampupd;
  unsigned scan;
  unsigned seq;
  unsigned sql;
  unsigned select;
  unsigned insert;
  unsigned update;
  unsigned statistics;
  char *file;
  FILE *out;
} duf_config_cli_trace_t;
typedef struct
{
  unsigned level;
  char *file;
  FILE *out;
} duf_config_cli_output_t;
typedef struct
{
  unsigned calculate:1;
  unsigned insert:1;
  unsigned update:1;
} duf_config_cli_disable_t;

typedef struct
{
  duf_config_cli_actions_t act;
  duf_config_cli_disable_t disable;
  duf_config_cli_debug_t dbg;
  duf_config_cli_trace_t trace;
  duf_config_cli_output_t output;
  duf_format_t format;
  /* long limit; */
  unsigned noopenat:1;
  unsigned dry_run:1;
} duf_config_cli_t;



#endif
