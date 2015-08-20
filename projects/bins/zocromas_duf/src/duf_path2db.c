#define DUF_SQL_PDI_STMT

#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"
#include "duf_utils_path.h"

/* #include "duf_levinfo.h" */
#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
#include "duf_levinfo_openclose.h"

#include "duf_pdi.h"

#include "duf_sql_defs.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_tags.h"

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
        "INSERT OR IGNORE INTO " DUF_DBPREF "paths ( dev, inode, dirname, parentid ) VALUES (:Dev, :iNode, :dirName, :parentdirID )";


  DUF_SQL_START_STMT( pdi, insert_path_table, sql, r, pstmt );
  if ( !duf_levinfo_stat_dev( pdi ) )
  {
    DOR( r, duf_levinfo_statat_dh( pdi ) );
  }

  assert( duf_levinfo_stat_dev( pdi ) );

  DUF_TRACE( insert, 0, "S:%s (%lu,%lu,'%s',%llu)", sql, duf_levinfo_stat_dev( pdi ), duf_levinfo_stat_inode( pdi ),
             duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ) );
  /* DUF_SHOW_ERROR( "insert_path_index:%d", insert_path_index ); */
  DUF_SQL_BIND_LL( Dev, duf_levinfo_stat_dev( pdi ), r, pstmt );
  DUF_SQL_BIND_LL( iNode, duf_levinfo_stat_inode( pdi ), r, pstmt );
  DUF_SQL_BIND_S( dirName, duf_levinfo_itemtruename( pdi ), r, pstmt );
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
      DUF_TRACE( select, 0, "<selected>" );
      dirid = duf_sql_column_long_long( pstmt, 0 );
      DUF_TRACE( select, 0, "S:%s (%llu,'%s') ~ '%s'; dirid:%llu", sqlv, duf_levinfo_dirid_up( pdi ), truedirname,
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
        DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), dirid, duf_levinfo_dirid_up( pdi ) );
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
          pli->numfile = duf_sql_column_long_long( pstmt, 2 );
          pli->numdir = duf_sql_column_long_long( pstmt, 3 );
          /* DUF_TRACE( path, 0, "@@@numdir:%ld; numfile: %ld;", pli->numdir, pli->numfile ); */
        }
      }
    }
    else
    {
      DUF_TRACE( select, 0, "S:%s (%llu,'%s') [%d] ~ '%s'; No dirid", sqlv, duf_levinfo_dirid_up( pdi ), truedirname, *truedirname,
                 duf_levinfo_itemshowname( pdi ) );
      DUF_TRACE( select, 0, "<NOT selected> (%d)", rpr );
    }
  }
  DUF_SQL_END_STMT( select_path, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( dirid );
}

static unsigned long long
duf_dirname_pdistat2dirid_existed( duf_depthinfo_t * pdi, const char *node_selector2, int *pr )
{
  unsigned long long dirid = 0;
  int rpr = 0;

  char *sqlv = NULL;
  const char *def_node_fieldset2 = "pt." DUF_SQL_IDNAME " AS dirID " /* */
        ", pt.dirname "         /*      */
        ", tf.numfiles AS nfiles" /* */
        ", td.numdirs AS ndirs  " /*      */
        ;
  const char *def_node_selector2 = " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL " AS td ON (td.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.pathid=pt." DUF_SQL_IDNAME ") " /*      */
        " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND (:dirName IS NULL OR dirname=:dirName)";

  DEBUG_START(  );

  assert( pdi );

  sqlv = mas_strdup( "SELECT " );
  sqlv = mas_strcat_x( sqlv, def_node_fieldset2 );
  sqlv = mas_strcat_x( sqlv, " " );
#if 0
  sqlv = mas_strcat_x( sqlv, node_selector2 ? node_selector2 : def_node_selector2 );
#else
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
duf_dirname_pdistat2dirid( duf_depthinfo_t * pdi, int caninsert, /* const char *dirname_unused, const struct stat *pstat_unused, */
                           const char *node_selector2, int need_id, int *pchanges, int *pr )
{
  unsigned long long dirid = 0;
  int rpr = 0;
  static unsigned long long cccc = 0;

  DEBUG_START(  );

  cccc++;
  assert( pdi );

  /* assert( parentid_unused == duf_levinfo_dirid_up( pdi ) ); */

  DUF_TRACE( path, 10, "@@@@@@@@@@@ %llu/%llu; caninsert:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), caninsert, pdi ? 1 : 0 );
  DUF_TRACE( path, 2, "@           inserting [%40s] %d", duf_levinfo_itemshowname( pdi ), caninsert );
  if ( 1 /* && dirname_unused *//* && pstat_unused->st_dev && pstat_unused->st_ino */  )
  {
    int changes = 0;

#if 1
    dirid = duf_dirname_pdistat2dirid_existed( pdi, node_selector2, pr );
    DUF_TRACE( path, 2, "(%d) dirid before insert: %llu for '%s' at %llu", rpr, dirid, duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ) );
#endif
    if ( dirid <= 0 && caninsert && !duf_config->cli.disable.flag.insert )
    {
      DOR( rpr, duf_dirname_insert_path_table( pdi, &changes ) );
    }
    /* DUF_TRACE( select, 0, "<changes> : %d : %s", changes, duf_levinfo_itemtruename( pdi ) ); */
    DUF_TRACE( select, 0, "<changes> : %d : %s", changes, duf_levinfo_itemshowname( pdi ) );
    if ( dirid <= 0 && need_id )
    {
      if ( ( rpr == DUF_SQL_CONSTRAINT || !rpr ) && !changes )
      {
        /* DUF_SQL_CONSTRAINT should be impossible from now */
        /*  !changes should be impossible from now too */
        assert( 0 );
        dirid = duf_dirname_pdistat2dirid_existed( pdi, node_selector2, pr );
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
      DUF_TEST_R( rpr );
    }
    if ( pchanges )
      *pchanges = changes;
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data dirname(%s) OR st_dev(%lu) OR st_ino(%lu)", duf_levinfo_itemshowname( pdi ), duf_levinfo_stat_dev( pdi ),
                    duf_levinfo_stat_inode( pdi ) );
    DUF_MAKE_ERROR( rpr, DUF_ERROR_DATA );
  }
  if ( need_id && !dirid )
  {
    if ( DUF_NOERROR( rpr ) )
      DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
    DUF_SHOW_ERROR( "(3) no dirid by parentid=%llu and dirname='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) );
    DUF_TEST_RX_SHOW( rpr, "(3) no dirid" );
  }
  /* DUF_TEST_R( rpr ); */
  if ( pr )
    *pr = rpr;
  /* assert( !need_id || dirid ); */
  DEBUG_ENDULL( dirid );
  return dirid;
}

static int
duf_path_component_here2db( duf_depthinfo_t * pdi, /* const char *dirname, */ int caninsert, const char *node_selector2,
                            unsigned long long *pparentid )
{
  DEBUG_STARTR( r );

  assert( pparentid );
  assert( pdi );

#if 0
  {
    const char *s1 = duf_levinfo_itemname( pdi );

    assert( ( s1[0] == '/' && s1[1] == 0 && dirname[0] == 0 ) || 0 == strcmp( s1, dirname ) );
  }
  DUF_TRACE( depth, 0, "D:%d; dirname:%s : itemname:%s", duf_pdi_depth( pdi ), dirname, duf_levinfo_itemname( pdi ) );
#endif

  DUF_TRACE( explain, 4, "already opened (at) ≪%s≫ upfd:%d", duf_levinfo_itemshowname( pdi ), duf_levinfo_dfd( pdi ) );

  if ( r >= 0 )
  {
    int changes = 0;

    changes = 0;
    DUF_TRACE( path, 5, "to insert [%s] pdi:%d", duf_levinfo_itemshowname( pdi ), pdi ? 1 : 0 );
    /* store/check path component to db; anyway get the ID */
#if 0
    *pparentid =
          duf_dirname2dirid( pdi, dirname, caninsert, duf_levinfo_stat_dev( pdi ), duf_levinfo_stat_inode( pdi ), node_selector2,
                             1 /* need_id */ , &changes, &r );
#else
    *pparentid = duf_dirname_pdistat2dirid( pdi, caninsert, /* dirname, duf_levinfo_stat( pdi ), */ node_selector2, 1 /* need_id */ , &changes, &r );
#endif
    /* assert( *pparentid ); */
    if ( changes )
    {
      DUF_TRACE( path, 0, "@@@dir added : %s (changes:%d)", duf_levinfo_itemshowname( pdi ), changes );
      DUF_TRACE( explain, 0, "added ID: %llu for ≪%s≫", *pparentid, duf_levinfo_itemshowname( pdi ) );
      if ( r >= 0 )
        r = changes;
    }
    else
      DUF_TRACE( explain, 1, "already in db ID: %llu for ≪%s≫", *pparentid, duf_levinfo_itemshowname( pdi ) );

    duf_levinfo_set_dirid( pdi, *pparentid );
    DUF_TRACE( path, 5, "inserted [%s] AS %llu", duf_levinfo_itemshowname( pdi ), *pparentid );
    DUF_TRACE( path, 5, "ID %llu for dirname ≪%s≫", *pparentid, duf_levinfo_itemshowname( pdi ) );
  }
  else
  {
    /* duf_levinfo_goup( pdi ); */
  }

  DEBUG_ENDR( r );
}

/*
 * store/check path component to db
 *   levinfo depth 1 level lower
 *   and set each level info at levinfo
 *   anyway get the ID
 * */
static int
duf_path_component2db( duf_depthinfo_t * pdi, const char *dirname, int caninsert, const char *node_selector2, unsigned long long *pparentid )
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

  DOR( r, duf_levinfo_godown_openat_dh( pdi, 0, dirname, 0 /* ndirs */ , 0 /* nfiles */ , 0 /* is_leaf */  ) );
  /* DOR( r, duf_levinfo_openat_dh( pdi ) ); (* levinfo depth 1 level lower *) */
  DOR( r, duf_path_component_here2db( pdi, /* dirname, */ caninsert, node_selector2, pparentid ) );

  DEBUG_ENDR( r );
}

/*
run  -OPRdEifndD -523Xe /mnt/jotera/down/downd/x/ --progress
run  -OPRdEifndD -523Xe /mnt/new_media/media/photo/ --progress

************************* 12 4 4 4 4 4 8 8
1:0 [ERROR  ] 331:_duf_real_path2db          : 0.0053 : No such entry  [media]
1:0 [ERROR  ] 368:duf_real_path2db           : 0.0053 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)                                                20140901.131659 
1:0 [ERROR  ]  77:duf_pdi_init               : 0.0053 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ]  91:duf_pdi_init_msg           : 0.0053 : path:/mnt/new_media/media/photo
1:0 [ERROR  ]  92:duf_pdi_init_msg           : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ]  62:duf_add_path_uni           : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 281:duf_action_new             : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 291:duf_action_new             : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 331:main_db                    : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 337:main_db                    : 0.0054 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 338:main_db                    : 0.0054 : action FAIL ; [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ] 360:main_db                    : 0.0056 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ]  71:duf_main                   : 0.0056 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
--------------------------------------------------
 main_db ended                                                       [Unknown errcode -1895656190] (#-1895656190)
--------------------------------------------------
1:0 [ERROR  ]  94:duf_main                   : 0.0056 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)
1:0 [ERROR  ]  96:duf_main                   : 0.0056 :  - - - - - -> [Unknown errcode -1895656190] (#-1895656190)

  [Unknown errcode -1895656190] (#-1895656190; i.e.-1895626190)
*/


/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 *
 *   note: sets depth + n
 * */
/* 20150820.144154 */
static int
_duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int caninsert, const char *node_selector2 )
{
  DEBUG_STARTR( r );
  int od = 0;
  unsigned long long parentid = 0;

  assert( pdi );
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
        DOR( r, duf_path_component2db( pdi, path, caninsert, node_selector2, &parentid ) );
        DUF_TRACE( path, 1, "@@@@#%-5llu (parentid)    [%40s]", parentid, path );
/*        if ( r < 0 )
          DUF_SHOW_ERROR( "No such entry %s [%s]", real_path, path ); */
        DUF_TRACE( path, 2, "@r:%d;next                 [%40s]  #%llu:%s", r, path, parentid, nextdir && *nextdir ? nextdir : "-=NONE=-" );
        if ( caninsert /* && r > 0 */  && parentid > 0 && !( nextdir && *nextdir ) )
        {
#ifdef MAS_TRACING
          unsigned long long tagid;
#else
          unsigned long long DUF_UNUSED tagid;
#endif

          tagid = duf_add_tag( pdi, "path" /* itemtype */ , parentid /* itemid */ , "added" /* tagname */ , &r );
          DUF_TRACE( path, 2, "%d: tag \"added\": %llu", r, tagid );
          if ( 1 )
          {
            tagid = duf_add_tag( pdi, "path" /* itemtype */ , parentid /* itemid */ , "dummy" /* tagname */ , &r );
            DUF_TRACE( path, 2, "%d: tag \"dummy\": %llu", r, tagid );
          }
        }
        path = nextdir;
      }
    }
    if ( !pdi && upfd )
      close( upfd );
    upfd = 0;
  }
  duf_pdi_set_opendir( pdi, od ); /* restore saved open status */
  if ( r >= 0 && !parentid )
    DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
  DEBUG_ENDR( r );
}
/* 20150820.144142 */
unsigned long long
duf_path2dirid( const char *path, const char *node_selector2, int *pr )
{
  int rpr = 0;
  char *real_path;
  unsigned long long dirid = 0;

  real_path = duf_realpath( path, &rpr );
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .pu = NULL,
      /* .opendir = sccb ? sccb->opendir : 0, */
      .opendir = 1,
      /* .name = real_path, */
    };
    DOR( rpr, DUF_WRAPPED( duf_pdi_init ) ( &di, real_path, 0 /* caninsert */ , node_selector2, 1 /* recursive */ , 0 /* opendir */  ) );
    if ( DUF_NOERROR( rpr ) )
      dirid = duf_levinfo_dirid( &di );
    /* xchanges = di.changes; --- needless!? */
    duf_pdi_close( &di );
  }
  mas_free( real_path );
  if ( pr )
    *pr = rpr;
  return dirid;
}

/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 * */
int
duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const char *node_selector2 )
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
    DOR( r, _duf_real_path2db( pdi, real_path, caninsert, node_selector2 ) );
    duf_pdi_set_topdepth( pdi );
  }
  mas_free( real_path );

  DEBUG_ENDR( r );
}
