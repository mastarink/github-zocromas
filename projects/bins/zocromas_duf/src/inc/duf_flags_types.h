#ifndef MAS_DUF_FLAGS_TYPES_H
#  define MAS_DUF_FLAGS_TYPES_H


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
  unsigned allow_vacuum:1;
  unsigned allow_remove_database:1;
  unsigned allow_drop_tables:1;
  unsigned allow_clean_tables:1;
  unsigned allow_create_database:1;
  unsigned allow_create_tables:1;

  unsigned allow_dirs:1;
  unsigned allow_sub:1;
  unsigned allow_files:1;



  unsigned progress:1;
  unsigned use_binformat:1;

  unsigned summary:1;
  unsigned interactive:1;
  unsigned fast:1;
  unsigned fresh:1;
  unsigned nocolor:1;
  unsigned force_color:1;

  unsigned add_path:1;
  unsigned dirent:1;

  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;

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
  DUF_FLAG_rfilter_recursive = 0,
  DUF_FLAG_rfilter_linear,
  DUF_FLAG_rfilter_testflag,
  DUF_FLAG_rfilter_testiflag,
  DUF_FLAG_rfilter_testnoflag,
} duf_rfilter_flags_enum_t;
typedef struct
{
  unsigned recursive:1;
  unsigned linear:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_rfilter_flags_t;

typedef union
{
  duf_rfilter_flags_t flag;
  unsigned short sbit;
} duf_rfilter_flags_combo_t;

/* ######################################## */

/* ######################################## */


#endif

/*
vi: ft=c
*/
