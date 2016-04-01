#ifndef MAS_DUF_HOOK_TYPES_H
# define MAS_DUF_HOOK_TYPES_H

# include <mastar/tools/mas_argvc_types.h>

/* # include "duf_record_types.h" */
# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_config_act_types.h"                                   /* duf_config_act_flags(_combo|_enum|)_t; duf_config_opt_actions_t ✗ */

/* typedef int ( *duf_scan_hook2_file_func_t ) ( duf_stmnt_t * pstmt_x, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh ); */
/* typedef int ( *duf_scan_hook2_item_func_t ) ( duf_stmnt_t * pstmt_x, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh ); */

typedef int ( *duf_anyhook_t ) ( void );

typedef int ( *duf_str_cb2_t ) ( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_x, duf_scanstage_t scanstage );

/* KNOWN duf_sel_cb_t callbacks:
 * duf_sel_cb_field_by_sccb	: str_cb_unused	, str_cb_udata_unused, pdi_unused
 * duf_sel_cb_levinfo		: str_cb_unused	, str_cb_udata_unused, xpdi_unused,	sccb_unused
 * duf_sel_cb_name_parid	: str_cb_unused	, str_cb_udata_unused, 			sccb_unused
 * duf_sel_cb_leaf		:		, sel_cb_udata_unused
 * duf_sel_cb_node		:		, sel_cb_udata_unused
*/

typedef int ( *duf_sel_cb2_t ) ( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_x, duf_str_cb2_t str_cb, duf_scanstage_t scanstage );

typedef struct duf_action_table_s duf_action_table_t;
struct duf_action_table_s
{
  unsigned end_of_table:1;
  unsigned in_use:1;
  unsigned tovector:1;
  duf_config_act_flags_combo_t on;
  duf_config_act_flags_combo_t off;
  duf_scan_callbacks_t *sccb;
};

#endif

/*
vi: ft=c
*/
