#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H
#  include <stdio.h>            /* FILE */
#  include "duf_flags_types.h"
#  include "duf_fun_types.h"
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
  duf_argvc_t files;
  duf_argvc_t dirs;
} duf_filedirformat_t;
typedef struct
{
  int use;
  duf_filedirformat_t tree;
  duf_filedirformat_t list;
  duf_filedirformat_t gen;
} duf_asformats_t;

typedef struct
{
  unsigned level;
  unsigned handleid;
  duf_asformats_t as_formats;
  char *sformat_dirs_tree;
  char *sformat_files_tree;
  char *sformat_prefix_gen_tree;
  char *sformat_prefix_files_tree;
  char *sformat_prefix_dirs_tree;
  char *sformat_dirs_list;
  char *sformat_files_list;
  char *sformat_dirs_gen;
  char *sformat_files_gen;
  char *file;
  FILE *out;
  char *history_filename;
  char *header;
} duf_config_output_t;

typedef struct
{
  unsigned trace;
  unsigned dry_run;
  unsigned flags;
  unsigned explain;
  unsigned pdi;
  unsigned levinfo;

  unsigned action;
  unsigned any;
  unsigned calls;
  unsigned collect;
  unsigned crc32;
  unsigned temporary;
  unsigned deleted;
  unsigned depth;
  unsigned dirent;
  unsigned dirs;
  unsigned error;
  unsigned errorr;
  unsigned exif;
  unsigned filedata;
  unsigned filenames;
  /* unsigned fill; */
  unsigned fs;
  unsigned insert;
  unsigned integrity;
  unsigned match;
  unsigned md5;
  unsigned mdpath;
  unsigned mime;
  unsigned nonew;
  unsigned options;
  unsigned config;
  unsigned path;
  unsigned mod;
  unsigned sample;
  unsigned sampupd;
  unsigned scan;
  unsigned scan_dir;
  unsigned scan_reg;
  unsigned sd5;
  unsigned select;
  unsigned seq;
  unsigned db;
  unsigned sql;
  unsigned sqlite;
  unsigned statistics;
  unsigned temp;
  unsigned update;
  unsigned io;
  unsigned verbose;
  
  unsigned sccb;
  unsigned sccbh;
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
  duf_bformat_combo_t bformat;
  char option_delimiter;
  char *shorts;
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

/*
vi: ft=c
*/
