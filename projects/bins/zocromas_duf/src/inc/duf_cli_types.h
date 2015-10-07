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
  int debug;
  unsigned nosqlite;
  int verbose;
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
  duf_filedirformat_t tree;
  duf_filedirformat_t list;
  duf_filedirformat_t gen;
} duf_asformats_t;

typedef struct
{
  char *dirs_tree;
  char *files_tree;
  
  char *prefix_gen_tree;
  char *prefix_files_tree;
  char *prefix_dirs_tree;
  
  char *dirs_list;
  char *files_list;

  char *dirs_gen;
  char *files_gen;
} duf_sformats_t;

typedef struct
{
  duf_config_output_flags_combo_t v;
  unsigned level;
  unsigned fun_width;
  unsigned handleid;
  unsigned max_width;
  duf_asformats_t as_formats;
#if 0
  char *sformat_dirs_tree;
  char *sformat_files_tree;
  char *sformat_prefix_gen_tree;
  char *sformat_prefix_files_tree;
  char *sformat_prefix_dirs_tree;
  char *sformat_dirs_list;
  char *sformat_files_list;
  char *sformat_dirs_gen;
  char *sformat_files_gen;
#else
  duf_sformats_t sformat;
#endif
  char *file;
  FILE *out;
  char *history_filename;
  char *header_tty;
} duf_config_output_t;

typedef struct
{
  duf_config_output_t output;

  int trace;
  int dry_run;
  int flags;
  int explain;
  int pdi;
  int levinfo;

  int action;
  int any;
  int calls;
  int collect;
  int crc32;
  int temporary;
  int deleted;
  int depth;
  int dirent;
  int dirs;
  int error;
  int handle_error;
  int errorr;
  int exif;
  int filedata;
  int filenames;
  /* int fill; */
  int fs;
  int insert;
  int match;
  int md5;
  int sha1;
  int mdpath;
  int mime;
  int nonew;
  int options;
  int config;
  int path;
  int mod;
  int sample;
  int sampupd;
  int scan;
  int scan_dir;
  int scan_reg;
  int sd5;
  int select;
  int seq;
  int db;
  int sql;
  int sqlite;
  int statistics;
  int temp;
  int todo;
  int update;
  int io;
  int verbose;

  int sccb;
  int sccbh;
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
