#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H

#  include "duf_flags_types.h"
/* duf_format_t */
#  include "duf_format_types.h"


typedef struct
{
  duf_config_act_flags_combo_t v;
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
  unsigned level;
  unsigned handleid;
  char *file;
  FILE *out;
} duf_config_output_t;

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
  unsigned scan_de_dir;
  unsigned scan_de_reg;
  unsigned temp;
  unsigned seq;
  unsigned options;
  unsigned sql;
  unsigned sqlite;
  unsigned select;
  unsigned insert;
  unsigned update;
  unsigned statistics;
  duf_config_output_t output;
} duf_config_cli_trace_t;

typedef struct
{
  duf_config_cli_flags_combo_t v;
  duf_config_cli_actions_t act;
  duf_config_cli_disable_flags_combo_t disable;
  duf_config_cli_debug_t dbg;
  duf_config_cli_trace_t trace;
  duf_config_output_t output;
  duf_format_combo_t format;
  /* long limit; */
} duf_config_cli_t;

typedef struct
{
  int count;
  const char *sql;
  const char *title;
  const char *labels[20];
} duf_infodata_t;

#endif
