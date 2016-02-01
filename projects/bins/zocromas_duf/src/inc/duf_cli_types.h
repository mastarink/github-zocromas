#ifndef MAS_DUF_CLI_TYPES_H
#  define MAS_DUF_CLI_TYPES_H
#  include <stdio.h>            /* FILE */

#  include <mastar/tools/mas_argvc_types.h>

#  include "duf_flags_types.h"
#  include "duf_fun_types.h"
/* duf_format_t */
#  include "duf_format_types.h"
#  include "duf_option_types.h"


typedef struct
{
  duf_config_act_flags_combo_t v;
  unsigned sample;
  unsigned sampupd;
} duf_config_opt_actions_t;

typedef struct
{
  int debug;
  unsigned nosqlite;
  int verbose;
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_opt_debug_t;

typedef struct
{
  mas_argvc_t files;
  mas_argvc_t dirs;
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
  unsigned handleid;
  unsigned max_width;
  duf_asformats_t as_formats;
#  if 0
  char *sformat_dirs_tree;
  char *sformat_files_tree;
  char *sformat_prefix_gen_tree;
  char *sformat_prefix_files_tree;
  char *sformat_prefix_dirs_tree;
  char *sformat_dirs_list;
  char *sformat_files_list;
  char *sformat_dirs_gen;
  char *sformat_files_gen;
#  else
  duf_sformats_t sformat;
#  endif
  char *file;
  FILE *out;
  /* char *history_filename; */
  char *header_tty;
} duf_config_output_t;

typedef struct
{
  duf_config_output_t output;
  double loadtime;
  unsigned fun_width;
  int temp;
  int collect;
  int any;
  int verbose;
/**/
  int dirs;
  int filedata;
  int filenames;
  /* int fill; */
  int nonew;
  int statistics;
/**/
#if 0
  int action;
  int calls;
  int config;
  int crc32;
  int db;
  int deleted;
  int depth;
  int dirent;
  int dry_run;
  int errorr;
  int errors;
  int exif;
  int explain;
  
  int flags;
  int fs;
  int handle_error;
  int insert;
  int io;
  int levinfo;
  int match;
  int md5;
  int mdpath;
  int mime;
  int mod;
  int options;
  int path;
  int pdi;
  int sample;
  int sampupd;
  int scan;
  int scan_dir;
  int scan_reg;
  int sccb;
  int sccbh;
  int sd5;
  int select;
  int seq;
  int sha1;
  int sql;
  int sqlite;
  int temporary;
  int todo;
  int trace;
  int update;
#else
#  define ENUM_WRAPP(_n, _rf, _rf2)      int _rf2; 
  #  include "duf_options_enum_trace.def"
#undef ENUM_WRAPP
#endif
} duf_config_opt_trace_t;

typedef struct
{
  duf_config_opt_flags_combo_t v;
  duf_config_opt_actions_t act;
  duf_config_opt_disable_flags_combo_t disable;
  duf_config_opt_debug_t dbg;
  duf_config_opt_trace_t trace;
  duf_config_output_t output;
  duf_bformat_combo_t bformat;
  char option_delimiter;
} duf_config_opt_t;

typedef struct
{
  char *shorts;
  mas_cargvc_t carg;
  mas_argvc_t targ;
  int targ_offset;

  duf_option_t *longopts_table;
  const duf_longval_extended_table_t **xtable_multi;
  /* long limit; */
  char *history_filename;
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
