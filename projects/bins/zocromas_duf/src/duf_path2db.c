#define DUF_SQL_PDI_STMT

#include <unistd.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h" /* cli.disable.flag.insert */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
/* #include "duf_levinfo_openclose.h" */
#include "duf_levinfo_stat.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_selector.h"

#include "duf_sql.h"
#include "duf_sql2.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2db.h"
/* ###################################################################### */


static int
duf_dirname_insert_path_table( duf_depthinfo_t * pdi, int *pchanges )
{
  DUF_STARTR( r );
  int changes = 0;

  static const char *sql =
        "INSERT OR IGNORE INTO " DUF_SQL_TABLES_PATHS_FULL " ( dev, inode, dirname, parentid ) VALUES (:Dev, :iNode, :dirName, :parentdirID )";


  DUF_SQL_START_STMT( pdi, insert_path_table, sql, r, pstmt );

#if 0
  if ( !duf_levinfo_stat_dev( pdi ) )
    DOR( r, duf_levinfo_statat_dh( pdi ) );
#else
  DOR( r, duf_levinfo_if_statat_dh( pdi ) );
#endif
  assert( duf_levinfo_stat_dev( pdi ) );

  DUF_TRACE( insert, 0, "S:%s (%lu,%lu,'%s',%llu)", sql, duf_levinfo_stat_dev( pdi ), duf_levinfo_stat_inode( pdi ),
             duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ) );
  /* DUF_SHOW_ERROR( "insert_path_index:%d", insert_path_index ); */
  DUF_SQL_BIND_LL( Dev, /*    */ duf_levinfo_stat_dev( pdi ), /*    */ r, pstmt );
  DUF_SQL_BIND_LL( iNode, /*  */ duf_levinfo_stat_inode( pdi ), /*  */ r, pstmt );
  DUF_SQL_BIND_S( dirName, /* */ duf_levinfo_itemtruename( pdi ), /**/ r, pstmt );
  /* DUF_SQL_BIND_LL( parentdirID, parentid, r, pstmt ); */
  DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), r, pstmt );
  /* NO: duf_bind_ufilter_uni( pstmt_selector ); */

  DUF_SQL_STEP( r, pstmt );
  DUF_SQL_CHANGES( changes, r, pstmt );
  DUF_SQL_END_STMT( insert_path_table, r, pstmt );
  if ( pchanges )
    *pchanges = changes;
  DUF_ENDR( r );
}

static unsigned long long
_duf_dirname_pdistat2dirid_existed( duf_depthinfo_t * pdi, const char *sqlv, int *pr )
{
  unsigned long long dirid = 0;
  int rpr = 0;

  DEBUG_START(  );
  DUF_SQL_START_STMT( pdi, select_path, sqlv, rpr, pstmt );
  {
    const char *truedirname;

    truedirname = duf_levinfo_itemtruename( pdi );
    DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), rpr, pstmt );
    /* NO: duf_bind_ufilter_uni( pstmt_selector ); */

    DUF_SQL_BIND_S( dirName, truedirname, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR( rpr, DUF_SQL_ROW ) )
    {
      rpr = 0;
      DUF_TRACE( select, 0, "<selected> %s", sqlv );
#if 0
      dirid = duf_sql_column_long_long( pstmt, 0 );
#else
      dirid = DUF_GET_UFIELD2( dirid );
#endif
      assert( DUF_GET_UFIELD2( dirid ) == dirid );

      DUF_TRACE( select, 1, "S:%s (%llu,'%s') ~ '%s'; dirid:%llu", sqlv, duf_levinfo_dirid_up( pdi ), truedirname,
                 duf_levinfo_itemshowname( pdi ), dirid );
      if ( !dirid )
      {
        /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dirname='%s'", parentid, duf_levinfo_itemname( pdi ) ); */
        DUF_SHOW_ERROR( "(1) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
        if ( rpr >= 0 )
          DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
      }
      else
      {
        DUF_TRACE( explain, 2, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), dirid, duf_levinfo_dirid_up( pdi ) );
      }
      assert( dirid );
      if ( rpr >= 0 )
      {
        duf_levinfo_t *pli;

        pli = duf_levinfo_ptr( pdi );
        if ( pli )
        {
          pli->dirid = dirid;
          /* pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) ); */
#if 0
          pli->numfile = duf_sql_column_long_long( pstmt, 2 );
          pli->numdir = duf_sql_column_long_long( pstmt, 3 );
#elif 0
          pli->numfile = duf_sql_column_long_long( pstmt, 4 );
          pli->numdir = duf_sql_column_long_long( pstmt, 5 );
#else
          pli->numfile = DUF_GET_UFIELD2( nfiles );
          pli->numdir = DUF_GET_UFIELD2( ndirs );
#endif

          DUF_TRACE( path, 20, "@@@ numdir:%ld; numfile:%ld [%s]", pli->numdir, pli->numfile, sqlv );

          /* DUF_TRACE( path, 0, "@@@numdir:%ld; numfile: %ld;", pli->numdir, pli->numfile ); */
        }
      }
    }
    else
    {
      DUF_TRACE( select, 1, "S:%s (%llu,'%s') [%d] ~ '%s'; No dirid", sqlv, duf_levinfo_dirid_up( pdi ), truedirname, *truedirname,
                 duf_levinfo_itemshowname( pdi ) );
      DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
    }
  }
  DUF_SQL_END_STMT( select_path, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( dirid );
}

static unsigned long long
duf_dirname_pdistat2dirid_existed( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set, int *pr )
{
  unsigned long long dirid = 0;
  int rpr = 0;

  char *sqlv = NULL;

#if 1
  duf_sql_set_t def_node_set = {
    .fieldset = "pt." DUF_SQL_IDNAME " AS dirid " /* */
          ", pt.dirname "       /*      */
          ", tf.numfiles AS nfiles" /* */
          ", td.numdirs AS ndirs  " /*      */
          ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL " AS td ON (td.pathid=pt." DUF_SQL_IDNAME ") " /*      */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.pathid=pt." DUF_SQL_IDNAME ") " /*      */
    " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND (:dirName IS NULL OR dirname=:dirName)"
  };
#else
  const char *def_node_fieldset2 = "pt." DUF_SQL_IDNAME " AS dirid " /* */
        ", pt.dirname "         /*      */
        ", tf.numfiles AS nfiles" /* */
        ", td.numdirs AS ndirs  " /*      */
        ;
  const char *def_node_selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL " AS td ON (td.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND (:dirName IS NULL OR dirname=:dirName)";
#endif
  DEBUG_START(  );

  assert( pdi );

#if 1
  sqlv = duf_selector2sql( sql_set ? sql_set : &def_node_set, pdi->pdi_name  );
#else
  sqlv = mas_strdup( "SELECT " );
  sqlv = mas_strcat_x( sqlv, def_node_fieldset2 );
  sqlv = mas_strcat_x( sqlv, " " );
  if ( node_selector2 )
  {
    sqlv = mas_strcat_x( sqlv, node_selector2 );
  }
  else
  {
    sqlv = mas_strcat_x( sqlv, def_node_selector2 );
  }
#endif

  dirid = _duf_dirname_pdistat2dirid_existed( pdi, sqlv, &rpr );
  mas_free( sqlv );
  DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
  if ( pr )
    *pr = rpr;
  /* assert( !need_id || dirid ); */
  DEBUG_ENDULL( dirid );
}


/* TODO remake to pdiitem2dirid */
unsigned long long
duf_dirname_pdistat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, int need_id, int *pchanges, int *pr )
{
  unsigned long long dirid = 0;
  int rpr = 0;

  DEBUG_START(  );

  assert( pdi );

  DUF_TRACE( path, 10, "@@@@@@@@@@@ %llu/%llu; caninsert:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), caninsert, pdi ? 1 : 0 );
  DUF_TRACE( path, 2, "@           inserting [%40s] %d", duf_levinfo_itemshowname( pdi ), caninsert );
  {
    int changes = 0;

    dirid = duf_dirname_pdistat2dirid_existed( pdi, sql_set /* node_selector2 */ , pr );
    DUF_TRACE( path, 2, "(%d) dirid before insert: %llu for '%s' at %llu", rpr, dirid, duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ) );

    if ( dirid <= 0 && caninsert && !duf_config->cli.disable.flag.insert )
    {
      DOR( rpr, duf_dirname_insert_path_table( pdi, &changes ) );
    }
    DUF_TRACE( select, 10, "<changes> : %d : %s", changes, duf_levinfo_itemshowname( pdi ) );
    if ( dirid <= 0 && need_id )
    {
#if 1
      if ( changes )
      {
        if ( DUF_NOERROR( rpr ) )
        {
          dirid = duf_sql_last_insert_rowid(  );
          if ( dirid )
            DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), dirid, duf_levinfo_dirid_up( pdi ) );
          else
          {
            /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dirname='%s'", parentid, duf_levinfo_itemshowname( pdi ) ); */
            DUF_SHOW_ERROR( "(2) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
            DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
          }
          assert( dirid );
          DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
        }
      }
      else
      {
        if ( DUF_IS_ERROR( rpr, DUF_SQL_CONSTRAINT ) || DUF_NOERROR( rpr ) )
        {
          if ( caninsert )
          {
            assert( 0 );
            dirid = duf_dirname_pdistat2dirid_existed( pdi, sql_set /* node_selector2 */ , pr );
          }
          else
          {
            DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
          }
        }
      }
#else
      if ( ( rpr == DUF_SQL_CONSTRAINT || !rpr ) && !changes )
      {
        /* DUF_SQL_CONSTRAINT should be impossible from now */
        /*  !changes should be impossible from now too */
        if ( caninsert )
        {
          assert( 0 );
          dirid = duf_dirname_pdistat2dirid_existed( pdi, sql_set /* node_selector2 */ , pr );
        }
        else
        {
          DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
        }
      }
      else if ( !rpr /* assume SQLITE_OK */  && changes )
      {
        dirid = duf_sql_last_insert_rowid(  );
        if ( !dirid )
        {
          /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dirname='%s'", parentid, duf_levinfo_itemshowname( pdi ) ); */
          DUF_SHOW_ERROR( "(2) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
          if ( rpr >= 0 )
            DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
        }
        else
        {
          DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), dirid, duf_levinfo_dirid_up( pdi ) );
        }
        assert( dirid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
      }
#endif
      DUF_TEST_R( rpr );
    }
    if ( pchanges )
      *pchanges = changes;
  }
  if ( need_id && !dirid )
  {
    if ( DUF_NOERROR( rpr ) )
      DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
    DUF_SHOW_ERROR( "(3) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
    DUF_TEST_RX_SHOW( rpr, "(3) no dirid" );
  }
  if ( pr )
    *pr = rpr;
  assert( !need_id || dirid );
  DEBUG_ENDULL( dirid );
  return dirid;
}

/* 20150831.142443 */
static int
duf_path_component_here2db( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, unsigned long long *pparentid )
{
  DEBUG_STARTR( r );
  unsigned long long parentid = 0;

  assert( pparentid );
  assert( pdi );

  {
    int changes = 0;

    changes = 0;
    DUF_TRACE( path, 5, "to insert [%s]", duf_levinfo_itemshowname( pdi ) );
    /* store/check path component to db; anyway get the ID */
    parentid = duf_dirname_pdistat2dirid( pdi, caninsert, sql_set /* node_selector2 */ , 1 /* need_id */ , &changes, &r );
    if ( changes )
    {
      DUF_TRACE( path, 0, "@@@dir added : %s (changes:%d)", duf_levinfo_itemshowname( pdi ), changes );
      DUF_TRACE( explain, 0, "added ID: %llu for ≪%s≫", parentid, duf_levinfo_itemshowname( pdi ) );
      if ( r >= 0 )
        r = changes;
    }
    else
      DUF_TRACE( explain, 2, "already in db ID: %llu for ≪%s≫", parentid, duf_levinfo_itemshowname( pdi ) );

    DUF_TRACE( path, 5, "inserted [%s] AS %llu", duf_levinfo_itemshowname( pdi ), parentid );
    DUF_TRACE( path, 5, "ID %llu for dirname ≪%s≫", parentid, duf_levinfo_itemshowname( pdi ) );
  }
  if ( pparentid )
    *pparentid = parentid;
  DEBUG_ENDR( r );
}

/*
 * store/check path component to db
 *   levinfo depth 1 level lower
 *   and set each level info at levinfo
 *   anyway get the ID
 * */
static int
duf_path_component2db( duf_depthinfo_t * pdi, const char *dirname, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */ ,
                       unsigned long long *pparentid )
{
  DEBUG_STARTR( r );

  assert( pparentid );
  assert( pdi );
  DUF_TRACE( depth, 0, "%s; depth:%d", dirname, duf_pdi_depth( pdi ) );

  if ( duf_pdi_depth( pdi ) >= 0 )
    DUF_TRACE( depth, 0, "D:%d; dirname:%s : itemname:%s", duf_pdi_depth( pdi ), dirname, duf_levinfo_itemshowname( pdi ) );

  if ( duf_pdi_depth( pdi ) >= 0 )
  {
    /* not here! */
    /* const char *s1 = duf_levinfo_itemname( pdi ); */
    /*  */
    /* assert( ( s1[0] == '/' && s1[1] == 0 && dirname[0] == 0 ) || 0 == strcmp( s1, dirname ) ); */
  }
  /* duf_levinfo_godown_openat_dh: 1. check depth; 2. duf_levinfo_godown */
  DOR( r, duf_levinfo_godown_openat_dh( pdi, dirname, 0 /* is_leaf */  ) );
  /* DOR( r, duf_levinfo_openat_dh( pdi ) ); (* levinfo depth 1 level lower *) */
  DOR( r, duf_path_component_here2db( pdi, /* dirname, */ caninsert, sql_set /* node_selector2 */ , pparentid ) );

  DEBUG_ENDR( r );
}

/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 *
 *   note: sets depth + n
 * */
/* 20150820.144154 */
static int
_duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{
  DEBUG_STARTR( r );
  int od = 0;
  unsigned long long parentid = 0;

  assert( pdi );
  DUF_TRACE( fs, 0, "set opendir to 1 (one)" );
  od = duf_pdi_set_opendir( pdi, 1 ); /* save open status to restore */
  {
    int upfd = 0;
    char *path;

    path = real_path;

    DUF_TRACE( path, 0, "@@@@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
    while ( r >= 0 && path && *path )
    {
      char *nextdir = path;

      DUF_TRACE( path, 4, "@@       -prepare path:[%40s]\tunder %llu", path, parentid );
      {
        /* extracting pathname component */
        while ( nextdir && *nextdir && *nextdir != '/' )
          nextdir++;
        if ( nextdir && *nextdir == '/' )
          *nextdir++ = 0;
      }
      {

        DUF_TRACE( path, 1, "@@      +prepared path:[%40s]\tup to [%s]", path, nextdir );
/* now path is pathname component;
 *
 * store/check it to db
 *   levinfo depth 1 level lower
 *   and set each level info at levinfo
 *   anyway get the ID
 *   
 *     depth + 1
 * */
        DOR( r, duf_path_component2db( pdi, path, caninsert, sql_set, &parentid ) );
        if ( DUF_NOERROR( r ) )
          duf_levinfo_set_dirid( pdi, parentid );
        DUF_TRACE( path, 1, "@@@@#%-5llu (parentid)    [%40s]", parentid, path );
/*        if ( r < 0 )
          DUF_SHOW_ERROR( "No such entry %s [%s]", real_path, path ); */
        DUF_TRACE( path, 2, "@r:%d;next                 [%40s]  #%llu:%s", r, path, parentid, nextdir && *nextdir ? nextdir : "-=NONE=-" );
#if 0
        if ( caninsert /* && r > 0 */  && parentid > 0 && !( nextdir && *nextdir ) )
        {
#  ifdef MAS_TRACING
          unsigned long long tagid;
#  else
          unsigned long long DUF_UNUSED tagid;
#  endif

          tagid = duf_add_tag( pdi, "path" /* itemtype */ , parentid /* itemid */ , "added" /* tag name */ , &r );
          DUF_TRACE( path, 2, "%d: tag \"added\": %llu", r, tagid );
          if ( 1 )
          {
            tagid = duf_add_tag( pdi, "path" /* itemtype */ , parentid /* itemid */ , "dummy" /* tag name */ , &r );
            DUF_TRACE( path, 2, "%d: tag \"dummy\": %llu", r, tagid );
          }
        }
#endif
        path = nextdir;
      }
    }
    if ( !pdi && upfd )
      close( upfd );
    upfd = 0;
  }
  DUF_TRACE( fs, 0, "set (restore) opendir to %d", od );
  duf_pdi_set_opendir( pdi, od ); /* restore saved open status */
  if ( r >= 0 && !parentid )
    DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  DEBUG_ENDR( r );
}

/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 * */
int
duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{
  DEBUG_STARTR( r );
  char *real_path;

  assert( pdi );
  /* assert( pdi->depth == -1 ); */

  real_path = mas_strdup( rpath );
  DUF_TRACE( path, 0, "@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
  {
    DUF_TRACE( explain, 0, "real_path: ≪%s≫", real_path );
    /* _duf_real_path2db
     *  need a copy of real_path to modify during parse
     *
     *   note: sets depth + n
     * */
    DOR( r, _duf_real_path2db( pdi, real_path, caninsert, sql_set /* node_selector2 */  ) );
    duf_pdi_set_topdepth( pdi );
  }
  mas_free( real_path );

  DEBUG_ENDR( r );
}
