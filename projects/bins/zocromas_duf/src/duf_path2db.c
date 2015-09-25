#include <unistd.h>
#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* cli.disable.flag.insert */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
/* #include "duf_levinfo_openclose.h" */
#include "duf_levinfo_stat.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"
#include "evsql_selector.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2db.h"
/* ###################################################################### */

static int duf_levinfo_stat_insert2db( duf_depthinfo_t * pdi, int *pchanges );
static int duf_set_dirid_and_nums_from_sql( duf_depthinfo_t * pdi, const char *sqlv );
static int duf_set_dirid_and_nums_from_sql_set( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set );
static int duf_levinfo_down_stat2dirid( duf_depthinfo_t * pdi, const char *dirname, int caninsert, const duf_sql_set_t * sql_set );
static int _duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set );

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***/
/***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****/
/******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* *******/
/********* ********* ********* ********* ********* ********* ********* ********* ********* ********* ********* ********* ********* ********* ***/

static int
duf_levinfo_stat_insert2db( duf_depthinfo_t * pdi, int *pchanges )
{
  DUF_STARTR( r );
  int changes = 0;

  static const char *sql =
        "INSERT OR IGNORE INTO " DUF_SQL_TABLES_PATHS_FULL " ( dev, inode, dirname, parentid )  VALUES (:Dev, :iNode, :dirName, :parentdirID )";


  DUF_SQL_START_STMT( pdi, insert_path_table, sql, r, pstmt );

#if 0
  if ( !duf_levinfo_stat_dev( pdi ) )
    DOR( r, duf_levinfo_statat_dh( pdi ) );
#else
  DOR( r, duf_levinfo_if_statat_dh( pdi ) );
#endif
  assert( DUF_IS_ERROR( r ) || duf_levinfo_stat_dev( pdi ) );

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
  DUF_SQL_END_STMT( pdi, insert_path_table, r, pstmt );
  if ( pchanges )
    *pchanges = changes;
  DUF_ENDR( r );
}

int
duf_set_dirid_and_nums( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long nfiles, unsigned long long ndirs )
{
  DUF_STARTR( r );
  duf_levinfo_t *pli;

  pli = duf_levinfo_ptr( pdi );
  assert( pli );
  duf_levinfo_set_dirid( pdi, dirid );
  pli->numfile = nfiles;
  pli->numdir = ndirs;
  assert( dirid == duf_levinfo_dirid( pdi ) );
  DUF_ENDR( r );
}

int
duf_set_dirid_and_nums_from_pstmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt )
{
  DUF_STARTR( r );

  DOR( r, duf_set_dirid_and_nums( pdi, DUF_GET_UFIELD2( dirid ), DUF_GET_UFIELD2( nfiles ), DUF_GET_UFIELD2( ndirs ) ) );
  assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );

  DUF_ENDR( r );
}

/* current level at pdi: 
 * 1. take itemtruename
 * 2. bind to sql param `parentdirID` <= parent dir id
 * 3. bind to sql param `dirName`     <= itemtruename
 * 4. do sql
 * 5. get `dirid` field from sql result to dirid at pdi current level
 * 6. get `nfiles`, `ndirs` fields from sql result to numfile, numdir at pdi current level
 * 7. return dirid as result
 * */
static int
duf_set_dirid_and_nums_from_sql( duf_depthinfo_t * pdi, const char *sqlv )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT( pdi, select_path, sqlv, r, pstmt );
  {
    const char *truedirname;

    truedirname = duf_levinfo_itemtruename( pdi );
    DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), r, pstmt );
    /* NO: duf_bind_ufilter_uni( pstmt_selector ); */

    DUF_SQL_BIND_S( dirName, truedirname, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    assert( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) || DUF_IS_ERROR_N( r, DUF_SQL_DONE ) );
    if ( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) )
    {
      DUF_CLEAR_ERROR( r, DUF_SQL_ROW );

      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      DUF_TRACE( select, 1, "S:%s (%llu,'%s') ~ '%s'; dirid:%llu", sqlv, duf_levinfo_dirid_up( pdi ), truedirname,
                 duf_levinfo_itemshowname( pdi ), DUF_GET_UFIELD2( dirid ) );
      if ( !DUF_GET_UFIELD2( dirid ) )
      {
        /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dirname='%s'", parentid, duf_levinfo_itemname( pdi ) ); */
        DUF_SHOW_ERROR( "(1) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
        if ( DUF_NOERROR( r ) )
          DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
      }
      else
      {
        DUF_TRACE( explain, 2, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), DUF_GET_UFIELD2( dirid ),
                   duf_levinfo_dirid_up( pdi ) );
      }
      assert( DUF_GET_UFIELD2( dirid ) );
      DOR( r, duf_set_dirid_and_nums_from_pstmt( pdi, pstmt ) );
      assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else if ( DUF_IS_ERROR_N( r, DUF_SQL_DONE ) )
    {
      DUF_TRACE( select, 1, "S:%s (%llu,'%s') [%d] ~ '%s'; No dirid", sqlv, duf_levinfo_dirid_up( pdi ), truedirname, *truedirname,
                 duf_levinfo_itemshowname( pdi ) );
      DUF_TRACE( select, 10, "<NOT selected> (%d)", r );
      assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else
    {
      assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
    }
  }
  assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
  DUF_SQL_END_STMT( pdi, select_path, r, pstmt );
  DEBUG_ENDR( r );
}

static int
duf_set_dirid_and_nums_from_sql_set( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set )
{
  unsigned long long dirid = 0;

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
  DEBUG_STARTR( r );

  assert( pdi );

#if 1
  sqlv = duf_selector2sql( sql_set ? sql_set : &def_node_set, pdi->pdi_name, &r );
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

  DOR( r, duf_set_dirid_and_nums_from_sql( pdi, sqlv ) );
  if ( DUF_NOERROR( r ) )
    dirid = duf_levinfo_dirid( pdi );
  mas_free( sqlv );
  DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
  assert( dirid == duf_levinfo_dirid( pdi ) );
  /* assert( !need_id || dirid ); */
  DEBUG_ENDR( r );
}


/* TODO remake to pdiitem2dirid */
int
_duf_levinfo_stat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, int need_id )
{
  DEBUG_STARTR( r );

  assert( pdi );

  DUF_TRACE( path, 10, "@@@@@@@@@@@ %llu/%llu; caninsert:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), caninsert, pdi ? 1 : 0 );
  DUF_TRACE( path, 2, "@           inserting [%40s] %d", duf_levinfo_itemshowname( pdi ), caninsert );
  {
    int changes = 0;

    DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set /* node_selector2 */  ) );
    DUF_TRACE( path, 2, "(%d) dirid before insert: %llu for '%s' at %llu", r, duf_levinfo_dirid( pdi ), duf_levinfo_itemtruename( pdi ),
               duf_levinfo_dirid_up( pdi ) );

    if ( duf_levinfo_dirid( pdi ) <= 0 && caninsert && !DUF_CONFIGG( cli.disable.flag.insert ) )
    {
      DOR( r, duf_levinfo_stat_insert2db( pdi, &changes ) );
    }
    DUF_TRACE( select, 10, "<changes> : %d : %s", changes, duf_levinfo_itemshowname( pdi ) );
    if ( duf_levinfo_dirid( pdi ) <= 0 && need_id )
    {
      if ( changes )
      {
        if ( DUF_NOERROR( r ) )
        {
          DOR( r, duf_set_dirid_and_nums( pdi, duf_sql_last_insert_rowid(  ), 0, 0 ) );
          if ( duf_levinfo_dirid_up( pdi ) )
          {
            DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ),
                       duf_levinfo_dirid_up( pdi ) );
          }
          if ( !duf_levinfo_dirid( pdi ) )
          {
            /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dirname='%s'", parentid, duf_levinfo_itemshowname( pdi ) ); */
            DUF_SHOW_ERROR( "(2) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
            DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
          }
          DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", duf_levinfo_dirid( pdi ), duf_levinfo_itemshowname( pdi ) );
        }
      }
      else
      {
        if ( DUF_IS_ERROR_N( r, DUF_SQL_CONSTRAINT ) )
        {
          if ( caninsert )
          {
            assert( 0 );        /*  */
            DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set /* node_selector2 */  ) );
          }
          else
          {
            DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
          }
        }
        else if ( DUF_NOERROR( r ) )
        {
          if ( caninsert )
          {
            assert( 0 );        /*  */
            DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set /* node_selector2 */  ) );
          }
          else
          {
            DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
          }
        }
        /* assert( 0 );            (*  *) */
      }

      DUF_TEST_R( r );
    }
  }
  if ( need_id && !duf_levinfo_dirid( pdi ) )
  {
    if ( DUF_NOERROR( r ) )
      DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
#if 0
    DUF_SHOW_ERROR( "@@@@@@@@(%s) no dirid by parentid=%llu and dirname='%s'", duf_error_name( r ), duf_levinfo_dirid_up( pdi ),
                    duf_levinfo_itemshowname( pdi ) );
#endif
  }
  DEBUG_ENDR( r );
}

int
duf_levinfo_stat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set )
{
  DEBUG_STARTR( r );
  DOR( r, _duf_levinfo_stat2dirid( pdi, caninsert, sql_set, 1 ) );
  DEBUG_ENDR( r );
}

static int
duf_levinfo_down_stat2dirid( duf_depthinfo_t * pdi, const char *dirname, int caninsert, const duf_sql_set_t * sql_set )
{
  DEBUG_STARTR( r );
  int up_d = 0;

  assert( pdi );
  up_d = duf_pdi_depth( pdi );

  /* duf_levinfo_godown_openat_dh: 1. check depth; 2. duf_levinfo_godown */
  DOR( r, duf_levinfo_godown_openat_dh( pdi, dirname, 0 /* is_leaf */  ) );

  assert( !DUF_NOERROR( r ) || up_d + 1 == duf_pdi_depth( pdi ) );

  DOR( r, duf_levinfo_stat2dirid( pdi, caninsert, sql_set /* node_selector2 *//*, 1 -- need_id   - error (0= not) if there is no record */  ) );

  DEBUG_ENDR( r );
}

/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 *
 *   note: sets depth + n
 * */
/* 20150912.180959 */
static int
_duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{
  DEBUG_STARTR( r );
  int od = 0;

  /* unsigned long long parentid = 0; */

  assert( pdi );
  DUF_TRACE( fs, 2, "set opendir to 1 (one)" );
  od = duf_pdi_set_opendir( pdi, 1 ); /* save open status to restore */
  {
    char *path;

    path = real_path;

    DUF_TRACE( path, 6, "@@@@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
    while ( DUF_NOERROR( r ) && path && *path )
    {
      char *nextdir = path;

      nextdir = strchr( path, '/' );
      if ( nextdir )
        *nextdir++ = 0;

      DUF_TRACE( path, 6, "@@      +prepared path:[%40s]\tup to [%s]", path, nextdir );
      {
        int up_d = 0;

        up_d = duf_pdi_depth( pdi );
        /* down to next */
        DOR( r, duf_levinfo_down_stat2dirid( pdi, path, caninsert, sql_set ) );
        assert( !DUF_NOERROR( r ) || up_d + 1 == duf_pdi_depth( pdi ) );
      }
      path = nextdir;
    }
  }
  DUF_TRACE( fs, 2, "set (restore) opendir to %d", od );
  duf_pdi_set_opendir( pdi, od ); /* restore saved open status */
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
  DUF_TRACE( path, 6, "@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
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
