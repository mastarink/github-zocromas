#ifndef MAS_DUF_OPTION_DATA_TYPES_H
#define MAS_DUF_OPTION_DATA_TYPES_H

/* #include "duf_config_cli_types.h" */
struct duf_config_cli_s ;
/* typedef struct duf_config_cli_s duf_config_cli_t; */

typedef int ( *duf_xclarifier_t ) ( struct duf_config_cli_s  * cli, const duf_longval_extended_t * extended, const char *optargg,
                                    const duf_longval_extended_vtable_t * xvtable, unsigned noo, duf_option_stage_t istage,
                                    duf_option_source_t source );

typedef struct
{
  unsigned string_split:1;
  unsigned noo:1;
/* const char *string; */
  char *string_copy;
  char value_separator;
  duf_option_source_t source;
  duf_option_stage_t stage;
  const duf_longval_extended_t *extended;

  size_t name_offset;
  char *name;
  char *optarg;
  int has_arg;

  const duf_longval_extended_vtable_t *xvtable;
  duf_xclarifier_t clarifier;
  duf_found_extended_array_t xfound;
  signed long doindex;
  unsigned clarified[DUF_OPTION_STAGE_MAX + 1];
} duf_option_data_t;



typedef struct
{
  size_t size;
  size_t count;
  duf_option_data_t *pods;
  duf_option_stage_t last_stage;
  duf_option_source_t last_source;
  size_t stage_index[DUF_OPTION_STAGE_MAX + 1];
  size_t stage_count[DUF_OPTION_STAGE_MAX + 1];
  size_t source_index[DUF_OPTION_STAGE_MAX + 1][DUF_OPTION_SOURCE_MAX + 1];
  size_t source_count[DUF_OPTION_STAGE_MAX + 1][DUF_OPTION_SOURCE_MAX + 1];
} duf_option_adata_t;


#endif
