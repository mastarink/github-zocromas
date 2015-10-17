#include <string.h>
#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_config_util.h"
#include "duf_config_defs.h"     /* DUF_ACTG_FLAG => DUF_OPTION */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb.h"
#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_dir_scan2_stages.h"
/* ###################################################################### */


/*
 *  - pstmt - for 1 node data, obtained from db
 *  - pdi
 *  - sccb
 *
 *  duf_sccbh_eval_db_node_before
 * */
#ifdef MAS_TRACING
#  define DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION(stagename) \
    int \
    duf_sccbh_eval_db_node_## stagename( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt, duf_sccb_handle_t *sccbh ) \
    { \
      DEBUG_STARTR( r ); \
      unsigned long long diridpdi; \
 \
      diridpdi = duf_levinfo_dirid( PDI ); \
      DUF_TRACE( scan, 4, "? scan node [" #stagename "]2 by %5llu", diridpdi ); \
      if ( DUF_ACTG_FLAG( allow_dirs ) ) \
      { \
	PDI->items.total++; \
	PDI->items.dirs++; \
 \
        DUF_TRACE( scan, 4, "? (dirs+) scan node [" #stagename "]2 by %5llu", diridpdi ); \
	if ( SCCB->node_scan_ ## stagename ## 2_deleted && duf_levinfo_if_deleted( PDI ) ) \
	{ \
	  DUF_TRACE( scan, 4, "scan node " #stagename "2_deleted by %5llu", diridpdi ); \
	   /* scanner = SCCB->node_scan_ ## stagename ## 2_deleted */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2_deleted( pstmt, /* diridpdi, */ PDI ) ); \
	} \
	else if ( SCCB->node_scan_ ## stagename ## 2 ) \
	{ \
	  DUF_TRACE( scan, 4, "scan node " #stagename "2 by %5llu", diridpdi ); \
	  /* scanner = SCCB->node_scan_ ## stagename ## 2 */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2( pstmt, /* diridpdi, */ PDI ) ); \
	} \
	else \
	{ \
	  DUF_TRACE( scan, 4, "NOT scan node " #stagename "2 by %5llu - sccb->node_scan_" #stagename "2 empty for %s", \
		      		diridpdi, duf_uni_scan_action_title( SCCB ) ); \
	} \
      } \
      else if ( SCCB->node_scan_ ## stagename ## 2 ) \
      { \
	DUF_TRACE( explain, 1, "to scan node " #stagename "2 use %s", DUF_OPT_NAME( FLAG_ALLOW_DIRECTORIES ) ); \
	DUF_TRACE( scan, 1, "to scan node " #stagename "2 use %s", DUF_OPT_NAME( FLAG_ALLOW_DIRECTORIES ) ); \
      } \
      else \
      { \
	DUF_TRACE( scan, 4, "NOT scan " #stagename "2 ( -d or --dirs absent )" ); \
      } \
      DEBUG_ENDR( r ); \
    }
#else
#  define DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( stagename ) \
    int \
    duf_sccbh_eval_db_node_## stagename( duf_scanstage_t scanstage DUF_UNUSED, duf_stmnt_t * pstmt, duf_sccb_handle_t *sccbh ) \
    { \
      DEBUG_STARTR( r ); \
 \
      if ( DUF_ACTG_FLAG( allow_dirs ) ) \
      { \
	PDI->items.total++; \
	PDI->items.dirs++; \
 \
	if ( SCCB->node_scan_ ## stagename ## 2_deleted && duf_levinfo_item_deleted( PDI ) ) \
	{ \
	     /* scanner = SCCB->node_scan_ ## stagename ## 2_deleted */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2_deleted( pstmt, /* diridpdi, */ PDI ) ); \
	} \
	else if ( SCCB->node_scan_ ## stagename ## 2 ) \
	{ \
	  /* scanner = SCCB->node_scan_ ## stagename ## 2 */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2( pstmt, /* diridpdi, */ PDI ) ); \
	} \
      } \
      DEBUG_ENDR( r ); \
    }
#endif

/* 20150820.085420 */
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( before );
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( middle );
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( after );
