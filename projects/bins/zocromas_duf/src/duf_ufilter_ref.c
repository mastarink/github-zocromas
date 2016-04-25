#include <assert.h>                                                  /* assert */
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_ufilter_structs.h"
/* ###################################################################### */
#include "duf_ufilter_ref.h"
/* ###################################################################### */

int
duf_ufilter_max_rel_depth( const duf_ufilter_t * pu )
{
  return pu ? pu->max_rel_depth : 0;
}

int
duf_ufilter_use_format( const duf_ufilter_t * pu )
{
  return pu ? pu->use_format : 0;
}

const char *
duf_ufilter_format_name( const duf_ufilter_t * pu MAS_UNUSED, unsigned id MAS_UNUSED )
{
  /* const char *ff[2]={NULL,NULL}; */
  /* return ff[0]; */
  return ( pu && ( id < ( sizeof( pu->format_anames ) / sizeof( char * ) ) ) )? pu->format_anames[id] : NULL;
}

const char *
duf_ufilter_list_format_name( const duf_ufilter_t * pu )
{
  return pu ? pu->format_names.list : NULL;
}

const char *
duf_ufilter_pack_format_name( const duf_ufilter_t * pu )
{
  return pu ? pu->format_names.pack : NULL;
}

const char *
duf_ufilter_tree_format_name( const duf_ufilter_t * pu )
{
  return pu ? pu->format_names.tree : NULL;
}

const char *
duf_ufilter_dirs_format_name( const duf_ufilter_t * pu )
{
  return pu ? pu->format_names.dirs : NULL;
}

const char *
duf_ufilter_tree_prefix_format_name( const duf_ufilter_t * pu )
{
  return pu ? pu->format_names.tree_prefix : NULL;
}

int
duf_ufilter_orderid( const duf_ufilter_t * pu )
{
  return pu ? pu->orderid : 0;
}

int
duf_ufilter_std_leaf_set_num( const duf_ufilter_t * pu )
{
  return pu ? pu->std_leaf_set_num : 0;
}

const char *
duf_ufilter_std_leaf_set_name( const duf_ufilter_t * pu )
{
  return pu ? pu->std_leaf_set_name : 0;
}

SR( OTHER, ufilter_max_filter, const duf_ufilter_t * pu, unsigned seq, const duf_items_t * pitems )
{
  if ( pu->max_seq && seq >= pu->max_seq )
    ERRMAKE( MAX_SEQ_REACHED );
  else if ( pu->maxitems.files && pitems->files >= pu->maxitems.files )
    ERRMAKE( MAX_REACHED );
  else if ( pu->maxitems.dirs && pitems->dirs >= pu->maxitems.dirs )
    ERRMAKE( MAX_REACHED );
  else if ( pu->maxitems.total && pitems->total >= pu->maxitems.total )
    ERRMAKE( MAX_REACHED );
  ER( OTHER, ufilter_max_filter, const duf_ufilter_t * pu, unsigned seq, const duf_items_t * pitems );
}
