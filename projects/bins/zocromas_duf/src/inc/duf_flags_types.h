#ifndef MAS_DUF_FLAGS_TYPES_H
# define MAS_DUF_FLAGS_TYPES_H

typedef enum
{
  DUF_FLAG_act_info,
  DUF_FLAG_act_allow_vacuum,
  DUF_FLAG_act_allow_remove_database,
  DUF_FLAG_act_allow_drop_tables,
  DUF_FLAG_act_allow_clean_tables,
  DUF_FLAG_act_allow_create_database,
  DUF_FLAG_act_allow_create_tables,

  DUF_FLAG_act_allow_dirs,
  DUF_FLAG_act_allow_sub,
  DUF_FLAG_act_allow_files,

  DUF_FLAG_act_progress,
  DUF_FLAG_act_use_binformat,

  DUF_FLAG_act_summary,
  DUF_FLAG_act_interactive,
  DUF_FLAG_act_fast,
  DUF_FLAG_act_fresh,
  DUF_FLAG_act_nocolor,
  DUF_FLAG_act_force_color,

  DUF_FLAG_act_add_path,
  DUF_FLAG_act_dirent,

  DUF_FLAG_act_testflag,
  DUF_FLAG_act_testiflag,
  DUF_FLAG_act_testnoflag,

} duf_config_act_flags_enum_t;
typedef struct
{
  unsigned info:1;
  unsigned allow_vacuum:1; /* => processing / db options */
  unsigned allow_remove_database:1; /* => processing / db options */
  unsigned allow_drop_tables:1; /* => processing / db options */
  unsigned allow_clean_tables:1; /* => processing / db options */
  unsigned allow_create_database:1; /* => processing / db options */
  unsigned allow_create_tables:1; /* => processing / db options */

  unsigned allow_dirs:1; /* => processing / db options */
  unsigned allow_sub:1; /* => processing / db options */
  unsigned allow_files:1; /* => processing / db options */

  unsigned summary:1; /* statistic or processing */
  unsigned interactive:1; /* commands options */
  unsigned fast:1; /* => processing / db options */
  unsigned fresh:1; /* => processing / db options */

  unsigned add_path:1; /* => processing / db options */
  unsigned dirent:1; /* => processing / db options */

  unsigned testflag:1; /* => test options */
  unsigned testiflag:1; /* => test options */
  unsigned testnoflag:1; /* => test options */

} duf_config_act_flags_t;

typedef union
{
  duf_config_act_flags_t flag;
  unsigned bit;
} duf_config_act_flags_combo_t;

/* ######################################## */
typedef enum
{
  DUF_FLAG_disable_calculate = 0,
  DUF_FLAG_disable_insert,
  DUF_FLAG_disable_update,
  DUF_FLAG_disable_fs,
  DUF_FLAG_disable_memusage,
  DUF_FLAG_disable_testflag,
  DUF_FLAG_disable_testiflag,
  DUF_FLAG_disable_testnoflag,
} duf_config_opt_disable_flags_enum_t;
typedef struct
{
  unsigned calculate:1;
  unsigned insert:1;
  unsigned update:1;
  unsigned fs:1;
  unsigned memusage:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_config_opt_disable_flags_t;

typedef union
{
  duf_config_opt_disable_flags_t flag;
  unsigned short sbit;
} duf_config_opt_disable_flags_combo_t;

/* ######################################## */
typedef enum
{
  DUF_FLAG_rec_etc_recursive = 0,
  DUF_FLAG_rec_etc_linear,
  DUF_FLAG_rec_etc_testflag,
  DUF_FLAG_rec_etc_testiflag,
  DUF_FLAG_rec_etc_testnoflag,
} duf_rec_etc_flags_enum_t;
typedef struct
{
  unsigned recursive:1;
  unsigned linear:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_rec_etc_flags_t;

typedef union
{
  duf_rec_etc_flags_t flag;
  unsigned short sbit;
} duf_rec_etc_flags_combo_t;

/* ######################################## */

/* ######################################## */

#endif

/*
vi: ft=c
*/
