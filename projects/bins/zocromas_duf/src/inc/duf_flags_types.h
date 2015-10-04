#ifndef MAS_DUF_FLAGS_TYPES_H
#  define MAS_DUF_FLAGS_TYPES_H


typedef struct
{
  unsigned info:1;
  unsigned vacuum:1;
  unsigned remove_database:1;
  unsigned drop_tables:1;
  unsigned create_database:1;
  unsigned create_tables:1;
  unsigned add_path:1;

  /* obsolete */
  unsigned sd5_obs:1;
  unsigned md5_obs:1;
  unsigned sha1_obs:1;
  unsigned crc32_obs:1;
  unsigned mime_obs:1;
  unsigned exif_obs:1;
  unsigned mdpath_obs:1;

  unsigned allow_dirs:1;
  unsigned allow_files:1;

  unsigned dirent:1;
  unsigned filedata:1;
  unsigned filenames:1;

  unsigned collect_obs:1;

  unsigned progress:1;
  unsigned use_binformat:1;

  unsigned summary:1;
  unsigned interactive:1;
#  if 0
  unsigned beginning_sql:1;     /* do 'beginning' sql's again (first time mandatory!) */
#  endif
  unsigned do_sccbs:1;
  unsigned fast:1;
} duf_config_act_flags_t;
typedef struct
{
  unsigned overwrite:1;
  unsigned append:1;
} duf_config_output_flags_t;

typedef union
{
  duf_config_act_flags_t flag;
  unsigned bit;
} duf_config_act_flags_combo_t;
typedef union
{
  duf_config_output_flags_t flag;
  unsigned bit;
} duf_config_output_flags_combo_t;


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
} duf_rfilter_flags_t;

typedef union
{
  duf_rfilter_flags_t flag;
  unsigned short sbit;
} duf_rfilter_flags_combo_t;

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
  duf_config_output_flags_t output;
  duf_rfilter_flags_t rec_etc;
  duf_config_cli_flags_t cli;
  duf_config_cli_disable_flags_t disable;
  unsigned bit;
  unsigned short sbit;
} duf_anyflag_t;


#endif

/*
vi: ft=c
*/
