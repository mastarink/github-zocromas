/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_ref_def.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

#include "duf_evsql_selector_new.h"                                  /* duf_selector2sql_new; duf_selector2sql_2new; duf_expand_sql; ✗ */

#include "duf_ufilter_bind.h"                                        /* duf_bind_ufilter_uni ✗ */

#include "sql_beginning_selected.h"
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_levinfo_count.h"
/* ###################################################################### */

/* unsigned long long                                               */
/* duf_levinfo_count_childs_d( const duf_depthinfo_t * pdi, int d ) */
SRX( PDI, unsigned long long, childs, 0, levinfo_count_childs_d, const duf_depthinfo_t * pdi, int d )
{
/* unsigned long long childs = 0; */

  if ( duf_levinfo_dirid_d( pdi, d ) && duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE )
  {
  /* int rpr = 0; */
    char *sql = NULL;
    unsigned ns = 0;

  /* assert( pdi->set_selected_db ); */
  /* QT( ">> set_selected_db:%d", pdi->set_selected_db ); */
    const duf_sql_set_t set[] = {
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
#if 0
       .filter = "pttot.parentid=:parentdirID"                       /* */
       .fieldset = "childs" /* */ ,
       .selector2 = "FROM ( SELECT parents." DUF_SQL_IDFIELD " AS parentid, COUNT( * ) AS childs FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
       " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD "= ptsp.parentid) " /* */
       " GROUP BY parents." DUF_SQL_IDFIELD ") AS pttot"
#else
       .filter = "parents.rowid=:parentdirID" /* */ ,
       .fieldset = "COUNT(*) AS childs" /* */ ,
       .selector2 = " FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
       " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD "= ptsp.parentid)"
#endif
     /* */ ,
       },
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
       .filter = "pt.parentid=:parentdirID",
       .fieldset = "COUNT(*) AS childs" /* */ ,
       .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "     /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptp ON( pt.parentid = ptp." DUF_SQL_IDFIELD " ) " /* */ ,
       }
    /* */ ,
    };

    ns = pdi->set_selected_db ? 0 : 1;
  /* assert( ns == 0 ); (* really ??? obsolete ????????????? *) */
    assert( ns < sizeof( set ) / sizeof( set[0] ) );
#if 0
    sql = duf_selector2sql( &set[ns], pdi->pdi_name, &rpr );
#elif 0
    sql = duf_selector2sql_new( &set[ns], 0 /*orderid */ , pdi->pdi_name, 0, QPERRIND );
#else
    sql = CRP( selector2sql_new, &set[ns], 0 /*orderid */ , pdi->pdi_name, 0 );
#endif
    if ( QNOERR && sql )
    {
    /* QT( "@%llu (%s)", duf_levinfo_dirid_d( pdi, d ), sql ); */
      DUF_SQL_SE_START_STMT( ( duf_depthinfo_t * ) pdi, select_childs, sql, pstmt );
      DUF_SQL_SE_BIND_LL( parentdirID, duf_levinfo_dirid_d( pdi, d ), pstmt );
      DUF_SQL_SE_STEP( pstmt );
      if ( QISERR1_N( SQL_ROW ) )
      {
        childs = DUF_GET_UFIELD2( childs );
      }
      else if ( QISERR1_N( SQL_DONE ) )
      {
        childs = 0;
      }
      else
      {
        MAST_TRACE( select, 0, "@<NOT selected> (%s)", QERRNAME );
        QT( "@<NOT selected> (%s)", QERRNAME );
        assert( 0 );
      }
    /* DUF_TEST_R( rpr ); */
      DUF_SQL_SE_END_STMT( ( duf_depthinfo_t * ) pdi, select_childs, pstmt );
#if 0
      duf_li_set_childs( duf_levinfo_ptr_d( pdi, d ), childs );
#endif
    /* QT( "@%llu => %lld (%s)", duf_levinfo_dirid_d( pdi, d ), childs, sql ); */

      mas_free( sql );
    }
  }
/* return childs; */
  ERX( PDI, unsigned long long, childs, 0, levinfo_count_childs_d, const duf_depthinfo_t * pdi, int d );
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC(   unsigned long long, count_childs )
DUF_LEVINFO_FC_UP( unsigned long long, count_childs )
/* *INDENT-ON*  */
