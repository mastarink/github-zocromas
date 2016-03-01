#include <assert.h>                                                  /* assert */

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_db_defs.h"

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */
#include "duf_sql_stmt_defs.h"                                       /* DUF_SQL_BIND_S_OPT etc. ♠ */
#include "duf_sql_prepared.h"
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ♠ */

/* ###################################################################### */
#include "duf_tags.h"
/* ###################################################################### */

static unsigned long long
duf_add_tagname( duf_depthinfo_t * pdi, const char *tag_name, int *pr )
{
  int rpr = 0;
  unsigned long long tagnameid = 0;
  int changes = 0;
  static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "tagnames ( name ) VALUES (:tagName )";
  static const char *sqlv = "SELECT " DUF_SQL_IDFIELD " AS tagnameId FROM " DUF_DBPREF "tagnames WHERE name=:tagName";

  {
    DUF_SQL_START_STMT( pdi, insert_tagname, sql, rpr, pstmt );

    DUF_SQL_BIND_S( tagName, tag_name, rpr, pstmt );
    DUF_SQL_STEPC( rpr, pstmt );
    DUF_SQL_CHANGES( changes, rpr, pstmt );
    DUF_SQL_END_STMT( pdi, insert_tagname, rpr, pstmt );
  }
  MAST_TRACE( path, 2, "@           inserting tag_name; tag_name %s; changes:%u", tag_name, changes );
  if ( ( DUF_IS_ERROR_N( rpr, DUF_SQL_CONSTRAINT ) || DUF_NOERROR( rpr ) ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tagname, sqlv, rpr, pstmt );
    DUF_SQL_BIND_S( tagName, tag_name, rpr, pstmt );

    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
    {
    /* rpr = 0; */
      MAST_TRACE( select, 0, "<selected>" );
#if 0
      tagnameid = duf_sql_column_long_long( pstmt, 0 );
#else
      tagnameid = DUF_GET_UFIELD2( tagnameId );
#endif
      MAST_TRACE( sql, 4, "tagnameid from tagnameId:%llu", tagnameid );
      MAST_TRACE( path, 2, "@           inserting tag_name %s; selected tagnameid:%llu", tag_name, tagnameid );
    }

    DUF_SQL_END_STMT( pdi, select_tagname, rpr, pstmt );
  }
  else
  {
    tagnameid = duf_sql_last_insert_rowid(  );
    MAST_TRACE( path, 2, "@           inserting tag_name %s; last_insert tagnameid:%llu", tag_name, tagnameid );
  }
  if ( pr )
    *pr = rpr;
  assert( tagnameid );
  return tagnameid;
}

unsigned long long
duf_add_tag( duf_depthinfo_t * pdi, const char *itemtype, unsigned long long itemid, const char *tag_name, int *pr )
{
  int rpr = 0;
  unsigned long long tagid = 0;
  unsigned long long tagnameid = 0;
  int changes = 0;
  static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "tags ( tagnameid, itemtype, itemid ) VALUES (:tagNameID, :itemType, :itemID )";
  static const char *sqlv =
          "SELECT " DUF_SQL_IDFIELD " AS tagId FROM " DUF_DBPREF "tags WHERE tagnameid=:tagNameID AND itemtype=:itemType AND itemid=:itemID";

  tagnameid = duf_add_tagname( pdi, tag_name, &rpr );
  {
    DUF_SQL_START_STMT( pdi, insert_tag, sql, rpr, pstmt );

    assert( tagnameid );
    DUF_SQL_BIND_LL( tagNameID, tagnameid, rpr, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, rpr, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, rpr, pstmt );
    DUF_SQL_STEPC( rpr, pstmt );
    DUF_SQL_CHANGES( changes, rpr, pstmt );
    DUF_SQL_END_STMT( pdi, insert_tag, rpr, pstmt );
  }
  MAST_TRACE( path, 2, "@           inserting tag; tag_name %s; changes:%u", tag_name, changes );
  if ( ( DUF_IS_ERROR_N( rpr, DUF_SQL_CONSTRAINT ) || DUF_NOERROR( rpr ) ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tag, sqlv, rpr, pstmt );
    DUF_SQL_BIND_LL( tagNameID, tagnameid, rpr, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, rpr, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, rpr, pstmt );

    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
    {
    /* rpr = 0; */
      MAST_TRACE( select, 0, "<selected>" );
#if 0
      tagid = duf_sql_column_long_long( pstmt, 0 );
#else
      tagid = DUF_GET_UFIELD2( tagId );
#endif
      MAST_TRACE( path, 2, "@           inserting tag_name %s; selected tagid:%llu", tag_name, tagnameid );
    }

    DUF_SQL_END_STMT( pdi, select_tag, rpr, pstmt );
  }
  else
  {
    tagid = duf_sql_last_insert_rowid(  );
    MAST_TRACE( path, 2, "@           inserting tag_name %s; last_insert tagid:%llu", tag_name, tagnameid );
  }
  if ( pr )
    *pr = rpr;
  return tagid;
}
