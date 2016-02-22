#ifndef MAS_DUF_CONFIG_TRACE_TYPES_H
# define MAS_DUF_CONFIG_TRACE_TYPES_H

# include "duf_config_output_types.h"

typedef struct
{
  duf_config_output_t output;
  double loadtime;
  unsigned fun_width;
  int temp;
  int collect;
  int any;
    /**/ int dirs;
  int filedata;
  int filenames;
/* int fill; */
  int nonew;
  int statistics;
    /**/
# if 0
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
# else
#  define ENUM_WRAPP(_n, _rf, _rf2)      int _rf2;
#  include "duf_options_enum_trace.def"
#  undef ENUM_WRAPP
# endif
} duf_config_trace_t;

#endif

/*
vi: ft=c
*/
