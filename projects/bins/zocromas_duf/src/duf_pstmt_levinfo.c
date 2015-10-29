#include "duf_maintenance.h"

#include "duf_pdi_ref.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_updown.h"
#include "duf_li.h"

#include "duf_pdi_stmt.h"
#include "duf_sql_stmt_defs.h"
#include "duf_sql_defs.h"
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

#include "evsql_selector.h"
#include "sql_tables_defs.h"    /* for duf_levinfo_childs* only */
#include "sql_selected_defs.h"

#include "duf_sql_field.h"


/* ###################################################################### */
#include "duf_pstmt_levinfo.h"
/* ###################################################################### */


/* 20150831.202009 */
int
duf_pstmt_levinfo_godown_dbopenat_dh( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );

  /* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  DUF_TRACE( explain, 2, "@ sel cb2 node" );

  DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, node_type, pstmt ) );

  assert( r < 0 || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) ); /* was set by duf_levinfo_godown */

  DEBUG_ENDR( r );
}

#ifdef DUF_NO_NUMS
unsigned long long
duf_levinfo_count_childs_d( const duf_depthinfo_t * pdi, int d )
{
  unsigned long long childs = 0;

  if ( duf_levinfo_dirid_d( pdi, d ) && duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE )
  {
    int rpr = 0;
    char *sql = NULL;
    int ns = 0;

    ns = pdi->sql_selected_done ? 0 : 1;
    /* T( ">> sql_selected_done:%d", pdi->sql_selected_done ); */
    const duf_sql_set_t set[] = {
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
#  if 0
       .filter = "pttot.parentid=:parentdirID" /* */
       .fieldset = "childs" /* */ ,
       .selector2 = "FROM ( SELECT parents." DUF_SQL_IDFIELD " AS parentid, COUNT( * ) AS childs FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
       " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD "= ptsp.parentid) " /* */
       " GROUP BY parents." DUF_SQL_IDFIELD ") AS pttot"
#  else
       .filter = "parents.rowid=:parentdirID" /* */ ,
       .fieldset = "COUNT(*) AS childs" /* */ ,
       .selector2 = " FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
       " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD "= ptsp.parentid)"
#  endif
       /* */ ,
       },
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
       .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptp ON( pt.parentid = ptp." DUF_SQL_IDFIELD " ) " /* */
       /* */ ,
       .fieldset = "COUNT(*) AS childs" /* */ ,
       .filter = "pt.parentid=:parentdirID"}
    };

    sql = duf_selector2sql( &set[ns], pdi->pdi_name, &rpr );
    if ( DUF_NOERROR( rpr ) && sql )
    {
      T( "@%llu (%s)", duf_levinfo_dirid_d( pdi, d ), sql );
      DUF_SQL_START_STMT( ( duf_depthinfo_t * ) pdi, select_childs, sql, rpr, pstmt );
      DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_d( pdi, d ), rpr, pstmt );
      DUF_SQL_STEP( rpr, pstmt );
      if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
      {
        childs = DUF_GET_UFIELD2( childs );
      }
      else if ( DUF_IS_ERROR_N( rpr, DUF_SQL_DONE ) )
      {
        childs = 0;
      }
      else
      {
        DUF_TRACE( select, 0, "@<NOT selected> (%s)", mas_error_name_i( rpr ) );
        T( "@<NOT selected> (%s)", mas_error_name_i( rpr ) );
        assert( 0 );
      }
      /* DUF_TEST_R( rpr ); */
      DUF_SQL_END_STMT( ( duf_depthinfo_t * ) pdi, select_childs, rpr, pstmt );
#  if 0
      duf_li_set_childs( duf_levinfo_ptr_d( pdi, d ), childs );
#  endif
      T( "@%llu => %lld (%s)", duf_levinfo_dirid_d( pdi, d ), childs, sql );

      mas_free( sql );
    }
  }
  return childs;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC(   unsigned long long, count_childs )
DUF_LEVINFO_FC_UP( unsigned long long, count_childs )
/* *INDENT-ON*  */
#endif
