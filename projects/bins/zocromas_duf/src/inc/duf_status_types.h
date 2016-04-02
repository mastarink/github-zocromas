#ifndef MAS_DUF_STATUS_TYPES_H
# define MAS_DUF_STATUS_TYPES_H

# include <mastar/multiconfig/muc_config_cli_types.h>                /* muc_config_cli_t */

# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_tmp_types.h"                                          /* duf_tmp_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_config_types.h"                                       /* duf_config_t; duf_config_save_t; duf_config_cfg_t; duf_config_vars_t ✗ */

typedef struct
{
  const muc_longval_extended_t *extended;
  char *optargg;
  muc_option_stage_t istage;
  unsigned noo;
  muc_option_source_t source;
} duf_xcmd_t;

typedef struct
{
  char *attached_selected;
  char *opened_name;
} duf_status_db_t;

typedef struct
{
  unsigned nopen;
  unsigned nclose;
} duf_status_dh_t;

typedef struct
{
/* struct duf_depthinfo_s *pdi; */
  duf_depthinfo_t *pdi;
} duf_status_scanner_t;

typedef struct
{
/* int actions_done; */
# if 0
  char *db_attached_selected;
  char *db_opened_name;
# else
  duf_status_db_t db;
# endif
/* int selection_done; */
# if 0
  duf_ufilter_t *selection_bound_ufilter;
# endif
/* size_t alloc_xcmds; */
/* size_t n_xcmds; */
/* duf_xcmd_t *xcmds; */
  duf_tmp_t *tmp;
  duf_status_dh_t dh;
  duf_depthinfo_t *pdilist;
  duf_status_scanner_t scn;
} duf_status_t;

#endif

/*
vi: ft=c
*/
