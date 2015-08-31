#define DUF_SQL_PDI_STMT

/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"

#include "duf_pdi.h"
#include "duf_pdi_stmt.h"

#include "duf_sql.h"
#include "duf_sql2.h"

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
  static const char *sqlv = "SELECT rowid AS tagnameid FROM " DUF_DBPREF "tagnames WHERE name=:tagName";

  {
    DUF_SQL_START_STMT( pdi, insert_tagname, sql, rpr, pstmt );

    DUF_SQL_BIND_S( tagName, tag_name, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    DUF_SQL_CHANGES( changes, rpr, pstmt );
    DUF_SQL_END_STMT( insert_tagname, rpr, pstmt );
  }
  DUF_TRACE( path, 2, "@           inserting tag_name; tag_name %s; changes:%u", tag_name, changes );
  if ( ( rpr == DUF_SQL_CONSTRAINT || !rpr ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tagname, sqlv, rpr, pstmt );
    DUF_SQL_BIND_S( tagName, tag_name, rpr, pstmt );

    DUF_SQL_STEP( rpr, pstmt );
    if ( rpr == DUF_SQL_ROW )
    {
      rpr = 0;
      DUF_TRACE( select, 0, "<selected>" );
      tagnameid = duf_sql_column_long_long( pstmt, 0 );
      DUF_TRACE( path, 2, "@           inserting tag_name %s; selected tagnameid:%llu", tag_name, tagnameid );
    }

    DUF_SQL_END_STMT( select_tagname, rpr, pstmt );
  }
  else
  {
    tagnameid = duf_sql_last_insert_rowid(  );
    DUF_TRACE( path, 2, "@           inserting tag_name %s; last_insert tagnameid:%llu", tag_name, tagnameid );
  }
  if ( pr )
    *pr = rpr;
  return tagnameid;
}

unsigned long long
duf_add_tag( duf_depthinfo_t * pdi, const char *itemtype, unsigned long long itemid, const char *tag_name, int *pr )
{
  int rpr = 0;
  unsigned long long tagid DUF_UNUSED = 0;
  unsigned long long tagnameid = 0;
  int changes = 0;
  static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "tags ( tagnameid, itemtype, itemid ) VALUES (:tagNameID, :itemType, :itemID )";
  static const char *sqlv = "SELECT rowid AS tagid FROM " DUF_DBPREF "tags WHERE tagnameid=:tagNameID AND itemtype=:itemType AND itemid=:itemID";

  tagnameid = duf_add_tagname( pdi, tag_name, &rpr );
  {
    DUF_SQL_START_STMT( pdi, insert_tag, sql, rpr, pstmt );

    DUF_SQL_BIND_LL( tagNameID, tagnameid, rpr, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, rpr, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    DUF_SQL_CHANGES( changes, rpr, pstmt );
    DUF_SQL_END_STMT( insert_tag, rpr, pstmt );
  }
  DUF_TRACE( path, 2, "@           inserting tag; tag_name %s; changes:%u", tag_name, changes );
  if ( ( DUF_IS_ERROR( rpr, DUF_SQL_CONSTRAINT ) || !rpr ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tag, sqlv, rpr, pstmt );
    DUF_SQL_BIND_LL( tagNameID, tagnameid, rpr, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, rpr, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, rpr, pstmt );

    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR( rpr, DUF_SQL_ROW ) )
    {
      rpr = 0;
      DUF_TRACE( select, 0, "<selected>" );
      tagid = duf_sql_column_long_long( pstmt, 0 );
      DUF_TRACE( path, 2, "@           inserting tag_name %s; selected tagid:%llu", tag_name, tagnameid );
    }

    DUF_SQL_END_STMT( select_tag, rpr, pstmt );
  }
  else
  {
    tagid = duf_sql_last_insert_rowid(  );
    DUF_TRACE( path, 2, "@           inserting tag_name %s; last_insert tagid:%llu", tag_name, tagnameid );
  }
  if ( pr )
    *pr = rpr;
  return tagid;
}
