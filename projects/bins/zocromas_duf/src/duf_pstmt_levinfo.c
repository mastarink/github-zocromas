#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_updown.h"
#include "duf_li.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_filters.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_defs.h"
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

#include "evsql_selector.h"
#include "sql_tables_defs.h"    /* for duf_levinfo_childs* only */
#include "sql_selected_defs.h"
#include "duf_ufilter_bind.h"


#include "sql_beginning_selected.h"

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

unsigned long long
duf_levinfo_count_childs_d( const duf_depthinfo_t * pdi, int d )
{
  unsigned long long childs = 0;

  if ( duf_levinfo_dirid_d( pdi, d ) && duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE )
  {
    int rpr = 0;
    char *sql = NULL;
    unsigned ns = 0;

    /* assert( pdi->sql_selected_done ); */
    /* T( ">> sql_selected_done:%d", pdi->sql_selected_done ); */
    const duf_sql_set_t set[] = {
      {
       .name = "childs",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
#if 0
       .filter = "pttot.parentid=:parentdirID" /* */
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
       .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
       " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptp ON( pt.parentid = ptp." DUF_SQL_IDFIELD " ) " /* */ ,
       }
      /* */ ,
    };

    ns = pdi->sql_selected_done ? 0 : 1;
    assert( ns < sizeof( set ) / sizeof( set[0] ) );
    sql = duf_selector2sql( &set[ns], pdi->pdi_name, &rpr );
    if ( DUF_NOERROR( rpr ) && sql )
    {
      /* T( "@%llu (%s)", duf_levinfo_dirid_d( pdi, d ), sql ); */
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
#if 0
      duf_li_set_childs( duf_levinfo_ptr_d( pdi, d ), childs );
#endif
      /* T( "@%llu => %lld (%s)", duf_levinfo_dirid_d( pdi, d ), childs, sql ); */

      mas_free( sql );
    }
  }
  return childs;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC(   unsigned long long, count_childs )
DUF_LEVINFO_FC_UP( unsigned long long, count_childs )
/* *INDENT-ON*  */

unsigned long long
duf_levinfo_count_gfiles_d( const duf_depthinfo_t * pdi, int d )
{
  unsigned long long gfiles = 0;

  if ( duf_levinfo_dirid_d( pdi, d ) && duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE )
  {
    int rpr = 0;
    char *sql = NULL;
    unsigned ns = 0;

    const duf_sql_set_t set[] = {
      {
       .name = "gfiles",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
       .fieldset = "COUNT(*) AS gfiles" /* */ ,
       .cte = "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS  ( " /* */
       " SELECT fpt." DUF_SQL_IDFIELD ",fpt.parentid " /* */
       "   FROM " DUF_SQL_TABLES_PATHS_FULL "  AS fpt  " /* */
       "     WHERE (parentid=:parentdirID OR " DUF_SQL_IDFIELD "=:parentdirID)" /* */
       "     UNION "            /* */
       "     SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid " /* */
       "      FROM cte_paths "  /* */
       "      JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " /* */
       "     ) ",               /* */
       .selector2_cte = " FROM cte_paths  AS ptenuf " /* */
       " LEFT JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (ptenuf." DUF_SQL_IDFIELD "=fn.Pathid) " /* */
       " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
       " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
       " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
       " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL /*                    */ " AS x  ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
       " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL /*              */ " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
       " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL /*                    */ " AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
       " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                     */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
       ,
       /* .filter = "fn.file_name LIKE '%a%'" (* *) , */
       .filter = DUF_SQL_UFILTER_BINDINGS,
       },
#if 0
      {
       .name = "gchilds",
       .type = DUF_NODE_NODE,
       .expand_sql = 1,
       .fieldset = "COUNT(*) AS gchilds" /* */ ,
       .cte = "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS  ( " /* */
       " SELECT dpt." DUF_SQL_IDFIELD ",dpt.parentid " /* */
       "   FROM " DUF_SQL_TABLES_PATHS_FULL "  AS dpt " /* */
       "     WHERE parentid=:parentdirID" /* */
       "     UNION "            /* */
       "     SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid " /* */
       "      FROM cte_paths "  /* */
       "      JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " /* */
       "     ) " /* */ ,
       .selector2_cte = "   FROM cte_paths  AS ptenud " /* */
       "   LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS dpt ON (ptenud." DUF_SQL_IDFIELD "=dpt." DUF_SQL_IDFIELD ") " /* */
       }
#endif
    };
    /* ns = pdi->sql_selected_done ? 0 : 1; */
    assert( ns < sizeof( set ) / sizeof( set[0] ) );
    sql = duf_selector2sql( &set[ns], pdi->pdi_name, &rpr );
    if ( DUF_NOERROR( rpr ) && sql )
    {
      /* T( "@%llu (%s)", duf_levinfo_dirid_d( pdi, d ), sql ); */
      DUF_SQL_START_STMT( ( duf_depthinfo_t * ) pdi, select_childs, sql, rpr, pstmt );
      DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_d( pdi, d ), rpr, pstmt );

      DOR( rpr, duf_bind_ufilter_uni( pstmt, duf_pdi_pu( pdi ), duf_pdi_py( pdi ), NULL ) );

      DUF_SQL_STEP( rpr, pstmt );
      if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
      {
        gfiles = DUF_GET_UFIELD2( gfiles );
      }
      else if ( DUF_IS_ERROR_N( rpr, DUF_SQL_DONE ) )
      {
        gfiles = 0;
      }
      else
      {
        DUF_TRACE( select, 0, "@<NOT selected> (%s)", mas_error_name_i( rpr ) );
        T( "@<NOT selected> (%s)", mas_error_name_i( rpr ) );
        assert( 0 );
      }
      /* DUF_TEST_R( rpr ); */
      DUF_SQL_END_STMT( ( duf_depthinfo_t * ) pdi, select_childs, rpr, pstmt );
#if 0
      duf_li_set_childs( duf_levinfo_ptr_d( pdi, d ), gfiles );
#endif
      /* T( "@%llu => %lld (%s)", duf_levinfo_dirid_d( pdi, d ), gfiles, sql ); */

      mas_free( sql );
    }
  }
  else
  {
    /* assert( 0 ); */
  }
  return gfiles;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC(   unsigned long long, count_gfiles )
DUF_LEVINFO_FC_UP( unsigned long long, count_gfiles )
/* *INDENT-ON*  */
