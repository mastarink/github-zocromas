#ifndef MAS_MUC_OPTION_DATA_TYPES_H
# define MAS_MUC_OPTION_DATA_TYPES_H

# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ; muc_limits_stage_t; muc_extended_stageopts_t */
struct muc_config_cli_s;

/* typedef struct muc_config_cli_s muc_config_cli_t; */

typedef int ( *muc_xclarifier_t ) ( struct muc_config_cli_s * cli, const muc_longval_extended_t * extended, const char *optargg,
                                    const muc_longval_extended_vtable_t * xvtable, unsigned noo, muc_option_stage_t istage,
                                    muc_option_source_t source );

struct muc_option_data_s
{
  unsigned string_split:1;
  unsigned noo:1;
/* const char *string; */
  char *string_copy;
  char value_separator;
  muc_option_source_t source;
  muc_option_stage_t stage;
  const muc_longval_extended_t *extended;

  size_t name_offset;
  char *name;
  char *optarg;
  int has_arg;

  const muc_longval_extended_vtable_t *xvtable;
  muc_xclarifier_t clarifier;
  muc_found_extended_array_t xfound;
  signed long doindex;
  unsigned clarified[MUC_OPTION_STAGE_MAX + 1];
};
typedef struct muc_option_data_s muc_option_data_t;

struct muc_option_adata_s
{
  size_t size;
  size_t count;
  muc_option_data_t *pods;
  muc_option_stage_t last_stage;
  muc_option_source_t last_source;
  size_t stage_index[MUC_OPTION_STAGE_MAX + 1];
  size_t stage_count[MUC_OPTION_STAGE_MAX + 1];
  size_t source_index[MUC_OPTION_STAGE_MAX + 1][MUC_OPTION_SOURCE_MAX + 1];
  size_t source_count[MUC_OPTION_STAGE_MAX + 1][MUC_OPTION_SOURCE_MAX + 1];
};
typedef struct muc_option_adata_s muc_option_adata_t;

#endif
