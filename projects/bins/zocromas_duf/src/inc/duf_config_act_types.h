#ifndef MAS_DUF_CONFIG_ACT_TYPES_H
#define MAS_DUF_CONFIG_ACT_TYPES_H


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

typedef struct
{
  duf_config_act_flags_combo_t v;
  unsigned sample;
  unsigned sampupd;
} duf_config_opt_actions_t;


#endif
