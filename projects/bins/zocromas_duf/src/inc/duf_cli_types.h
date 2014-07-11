#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H

/* duf_format_t */
#  include "duf_format_types.h"


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
} duf_config_act_flags_t;

typedef struct
{
  union
  {
    duf_config_act_flags_t flag;
    unsigned bit;
  } v;
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
  unsigned dry_run;
  unsigned explain;

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
  unsigned dry_run:1;
  unsigned explain:1;
} duf_config_cli_flags_t;

typedef struct
{
  union
  {
    duf_config_cli_flags_t flag;
    unsigned short bit;
  } v;
  duf_config_cli_actions_t act;
  duf_config_cli_disable_t disable;
  duf_config_cli_debug_t dbg;
  duf_config_cli_trace_t trace;
  duf_config_cli_output_t output;
  duf_format_t format;
  /* long limit; */
} duf_config_cli_t;



#endif
