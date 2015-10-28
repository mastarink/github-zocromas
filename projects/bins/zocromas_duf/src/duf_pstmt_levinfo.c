#include "duf_maintenance.h"

#include "duf_pdi_ref.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_updown.h"

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
int
duf_levinfo_make_childs_d( duf_depthinfo_t * pdi DUF_UNUSED, int d DUF_UNUSED )
{
  int rpr DUF_UNUSED = 0;

  if ( duf_levinfo_dirid_d( pdi, d ) )
  {
    unsigned long long childs = 0;
    char *sql = NULL;
    int ns = 1;

    const duf_sql_set_t set[] = {
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
       .selector2 = " FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pt " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptp ON( pt.parentid = ptp." DUF_SQL_IDFIELD " ) " /* */
       /* */ ,
       .fieldset = "COUNT(*) AS childs" /* */ ,
       .filter = "pt.parentid=:parentdirID" /* */
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
    /* T( "@@%s", sql ); */
    if ( sql )
    {
      DUF_SQL_START_STMT( pdi, select_childs, sql, rpr, pstmt );
      DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_d( pdi, d ), rpr, pstmt );
      DUF_SQL_STEP( rpr, pstmt );
      if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
      {
        childs = DUF_GET_UFIELD2( childs );
      }
      else
      {
        DUF_TRACE( select, 0, "@<NOT selected> (%d)", rpr );
        assert( 0 );
      }
      /* DUF_TEST_R( rpr ); */
      DUF_SQL_END_STMT( pdi, select_childs, rpr, pstmt );
      mas_free( sql );
      /* T( "@@childs %d - %llu", d, childs ); */
    }
    duf_levinfo_ptr_d( pdi, d )->childs = childs;
    duf_levinfo_ptr_d( pdi, d )->numchild = childs;
  }
  return 0;
}

DUF_LEVINFO_F( int, make_childs ) DUF_LEVINFO_F_UP( int, make_childs )
#endif
