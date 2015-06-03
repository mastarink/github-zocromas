#define DUF_SQL_PDI_STMT

/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"

#include "duf_pdi.h"

#include "duf_sql.h"
#include "duf_sql2.h"

/* ###################################################################### */
#include "duf_tags.h"
/* ###################################################################### */

static unsigned long long
duf_add_tagname( duf_depthinfo_t * pdi, const char *tagname, int *pr )
{
  int r = 0;
  unsigned long long tagnameid = 0;
  int changes = 0;
  static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "tagnames ( name ) VALUES (:tagName )";
  static const char *sqlv = "SELECT rowid AS tagnameid FROM " DUF_DBPREF "tagnames WHERE name=:tagName";

  {
    DUF_SQL_START_STMT( pdi, insert_tagname, sql, r, pstmt );

    DUF_SQL_BIND_S( tagName, tagname, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( insert_tagname, r, pstmt );
  }
  DUF_TRACE( path, 2, "@           inserting tagname; tagname %s; changes:%u", tagname, changes );
  if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tagname, sqlv, r, pstmt );
    DUF_SQL_BIND_S( tagName, tagname, r, pstmt );

    DUF_SQL_STEP( r, pstmt );
    if ( r == DUF_SQL_ROW )
    {
      r = 0;
      DUF_TRACE( select, 0, "<selected>" );
      tagnameid = duf_sql_column_long_long( pstmt, 0 );
      DUF_TRACE( path, 2, "@           inserting tagname %s; selected tagnameid:%llu", tagname, tagnameid );
    }

    DUF_SQL_END_STMT( select_tagname, r, pstmt );
  }
  else
  {
    tagnameid = duf_sql_last_insert_rowid(  );
    DUF_TRACE( path, 2, "@           inserting tagname %s; last_insert tagnameid:%llu", tagname, tagnameid );
  }
  if ( pr )
    *pr = r;
  return tagnameid;
}

unsigned long long
duf_add_tag( duf_depthinfo_t * pdi, const char *itemtype, unsigned long long itemid, const char *tagname, int *pr )
{
  int r = 0;
  unsigned long long tagid DUF_UNUSED = 0;
  unsigned long long tagnameid = 0;
  int changes = 0;
  static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "unitags ( tagnameid, itemtype, itemid ) VALUES (:tagNameID, :itemType, :itemID )";
  static const char *sqlv = "SELECT rowid AS tagid FROM " DUF_DBPREF "unitags WHERE tagnameid=:tagNameID AND itemtype=:itemType AND itemid=:itemID";

  tagnameid = duf_add_tagname( pdi, tagname, &r );
  {
    DUF_SQL_START_STMT( pdi, insert_tag, sql, r, pstmt );

    DUF_SQL_BIND_LL( tagNameID, tagnameid, r, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, r, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( insert_tag, r, pstmt );
  }
  DUF_TRACE( path, 2, "@           inserting tag; tagname %s; changes:%u", tagname, changes );
  if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
  {
    DUF_SQL_START_STMT( pdi, select_tag, sqlv, r, pstmt );
    DUF_SQL_BIND_LL( tagNameID, tagnameid, r, pstmt );
    DUF_SQL_BIND_S( itemType, itemtype, r, pstmt );
    DUF_SQL_BIND_LL( itemID, itemid, r, pstmt );

    DUF_SQL_STEP( r, pstmt );
    if ( r == DUF_SQL_ROW )
    {
      r = 0;
      DUF_TRACE( select, 0, "<selected>" );
      tagid = duf_sql_column_long_long( pstmt, 0 );
      DUF_TRACE( path, 2, "@           inserting tagname %s; selected tagid:%llu", tagname, tagnameid );
    }

    DUF_SQL_END_STMT( select_tag, r, pstmt );
  }
  else
  {
    tagid = duf_sql_last_insert_rowid(  );
    DUF_TRACE( path, 2, "@           inserting tagname %s; last_insert tagid:%llu", tagname, tagnameid );
  }
  if ( pr )
    *pr = r;
  return tagid;
}
