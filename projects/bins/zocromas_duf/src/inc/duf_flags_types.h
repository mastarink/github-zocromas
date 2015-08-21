#ifndef MAS_DUF_FLAGS_TYPES_H
#  define MAS_DUF_FLAGS_TYPES_H


typedef struct
{
  unsigned info:1;
  unsigned vacuum:1;
  unsigned remove_database:1;
  unsigned drop_tables:1;
  unsigned create_tables:1;
  unsigned add_path:1;
  unsigned sd5:1;
  unsigned md5:1;
  unsigned crc32:1;
  unsigned mime:1;
  unsigned exif:1;

  unsigned mdpath:1;

  unsigned dirs:1;
  unsigned files:1;

  unsigned dirent:1;
  unsigned filedata:1;
  unsigned filenames:1;

  unsigned integrity:1;
  unsigned collect:1;
  
  unsigned progress:1;
  unsigned use_binformat:1;

  unsigned summary:1;
  unsigned interactive:1;
#if 0
  unsigned beginning_sql:1; /* do 'beginning' sql's again (first time mandatory!) */
#endif
  unsigned do_sccbs:1;
  unsigned fast:1;
} duf_config_act_flags_t;
typedef union
{
  duf_config_act_flags_t flag;
  unsigned bit;
} duf_config_act_flags_combo_t;

/* ######################################## */

typedef struct
{
  unsigned calculate:1;
  unsigned insert:1;
  unsigned update:1;
  unsigned fs:1;
  unsigned memusage:1;
} duf_config_cli_disable_flags_t;

typedef union
{
  duf_config_cli_disable_flags_t flag;
  unsigned short sbit;
} duf_config_cli_disable_flags_combo_t;

/* ######################################## */

typedef struct
{
  unsigned recursive:1;
} duf_ufilter_flags_t;

typedef union
{
  duf_ufilter_flags_t flag;
  unsigned short sbit;
} duf_ufilter_flags_combo_t;

/* ######################################## */

typedef struct
{
  unsigned dry_run:1;
} duf_config_cli_flags_t;

typedef union
{
  duf_config_cli_flags_t flag;
  unsigned short sbit;
} duf_config_cli_flags_combo_t;

/* ######################################## */

typedef union
{
  duf_config_act_flags_t act;
  duf_ufilter_flags_t u;
  duf_config_cli_flags_t cli;
  duf_config_cli_disable_flags_t disable;
  unsigned bit;
  unsigned short sbit;
} duf_anyflag_t;


#endif

/*
vi: ft=c
*/
