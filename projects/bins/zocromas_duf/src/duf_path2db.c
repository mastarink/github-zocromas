#include <assert.h>                                                  /* assert */
#include <unistd.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"                                          /* opt.disable.flag.insert */
#include "duf_config_defs.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"
/* #include "duf_levinfo_openclose.h" */
#include "duf_levinfo_stat.h"
#include "duf_li.h"
#include "duf_pstmt_levinfo.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"

/* #include "evsql_selector.h" */
#include "evsql_selector_new.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2db.h"
/* ###################################################################### */

#ifndef MAS_SQL_TABLES_GLOBAL_DEFS_H
# error use #include "sql_tables_global_defs.h"
#endif

/* 20151026.111303 */
/* insert dir info to db by pdi and possbly fs */
static int
duf_levinfo_stat_insert2db( duf_depthinfo_t * pdi, int *pchanges )
{
  DUF_STARTR( r );
  int changes = 0;

  static const char *sql =
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_PATHS_FULL " ( dev, inode, " DUF_SQL_DIRNAMEFIELD
          ", parentid )  VALUES (:Dev, :iNode, :dirName, :parentdirID )";

  DUF_SQL_START_STMT( pdi, insert_path_table, sql, r, pstmt );

#if 0
  if ( !duf_levinfo_stat_dev( pdi ) )
    DOR( r, duf_levinfo_statat_dh( pdi ) );
#else
  DOR_LOWERE( r, duf_levinfo_if_statat_dh( pdi ), DUF_ERROR_STATAT_ENOENT );
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

/* 20151026.110612 */
/* at levinfo current level: set dirid,numdir,numfile */
static int
duf_set_dirid_and_nums( duf_depthinfo_t * pdi, unsigned long long dirid
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
                        , unsigned long long nfiles, unsigned long long ndirs
#endif
         )
{
  DUF_STARTR( r );
  duf_levinfo_t *pli;

  pli = duf_levinfo_ptr( pdi );
  assert( pli );
  duf_levinfo_set_dirid( pdi, dirid );
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
/* pli->numfile = nfiles; */
/* pli->numdir = ndirs;   */
  duf_li_set_nums( pli, ndirs, nfiles );
#else
/* duf_levinfo_make_childs( pdi ); */
#endif
  assert( dirid == duf_levinfo_dirid( pdi ) );
  DUF_ENDR( r );
}

/* 20151026.110729 */
/* at levinfo current level: set dirid,numdir,numfile
 * WRAPper for duf_set_dirid_and_nums */
static int
duf_set_dirid_and_nums_from_pstmt( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt )
{
  DUF_STARTR( r );

  DOR( r, duf_set_dirid_and_nums( pdi, DUF_GET_UFIELD2( dirid )
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
                                  , DUF_GET_UFIELD2( nfiles ), DUF_GET_UFIELD2( ndirs )
#endif
        ) );                                                         /* at levinfo current level: set dirid,numdir,numfile */
  assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );

  DUF_ENDR( r );
}

/* 20151026.111126 */
/* at levinfo current level: set dirid,numdir,numfile by pdi and sql; */
static int
duf_set_dirid_and_nums_from_sql( duf_depthinfo_t * pdi, const char *sqlv )
{
  DUF_STARTR( r );

  assert( pdi->pdi_name );
  DUF_SQL_START_STMT( pdi, select_path, sqlv, r, pstmt );
  DUF_TRACE( path, 2, "(%d:%s) dirid: %llu for '%s' at %llu; '%s'", r, mas_error_name_i( r ), duf_levinfo_dirid( pdi ),
             duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ), sqlv );
  {
    const char *truedirname;

    truedirname = duf_levinfo_itemtruename( pdi );
    DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid_up( pdi ), r, pstmt );
  /* NO: duf_bind_ufilter_uni( pstmt_selector ); */
    DUF_SQL_BIND_S( dirName, truedirname, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
  /* assert( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) || DUF_IS_ERROR_N( r, DUF_SQL_DONE ) ); */
    if ( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) )
    {
      DUF_CLEAR_ERROR( r, DUF_SQL_ROW );

      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      DUF_TRACE( select, 1, "S:%s (%llu,'%s') ~ '%s'; dirid:%llu", sqlv, duf_levinfo_dirid_up( pdi ), truedirname,
                 duf_levinfo_itemshowname( pdi ), DUF_GET_UFIELD2( dirid ) );
      if ( !DUF_GET_UFIELD2( dirid ) )
      {
      /* DUF_SHOW_ERROR( "(1) no dirid by parentid=%llu and " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) ); */
        if ( DUF_NOERROR( r ) )
          DUF_MAKE_ERRORM( r, DUF_ERROR_NOT_IN_DB, "(1) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ),
                           duf_levinfo_itemshowname( pdi ) );
      }
      else
      {
        DUF_TRACE( explain, 2, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), DUF_GET_UFIELD2( dirid ),
                   duf_levinfo_dirid_up( pdi ) );
      }
      assert( DUF_GET_UFIELD2( dirid ) );
      DOR( r, duf_set_dirid_and_nums_from_pstmt( pdi, pstmt ) );     /* at levinfo current level: set dirid,numdir,numfile; WRAPper for duf_set_dirid_and_nums */
      assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else if ( DUF_IS_ERROR_N( r, DUF_SQL_DONE ) )
    {
    /* DUF_CLEAR_ERROR( r, DUF_SQL_DONE ); */
      DUF_TRACE( select, 1, "S:%s (%llu,'%s') [%d] ~ '%s'; No dirid", sqlv, duf_levinfo_dirid_up( pdi ), truedirname, *truedirname,
                 duf_levinfo_itemshowname( pdi ) );
      DUF_TRACE( select, 10, "<NOT selected> (%d)", r );
      assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) );
    }
    else
    {
    /* assert( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) ); */
    }
  }
  assert( DUF_IS_ERROR( r ) || ( DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) ) );
  DUF_SQL_END_STMT( pdi, select_path, r, pstmt );
  DUF_ENDR( r );
}

/* 20151026.111923 */
/* at levinfo current level: set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */
static int
duf_set_dirid_and_nums_from_sql_set( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set )
{
  unsigned long long dirid = 0;

  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt." DUF_SQL_IDFIELD " AS dirid "                   /* */
            ", pt." DUF_SQL_DIRNAMEFIELD " AS dname "                /*      */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
            ", tf.numfiles AS nfiles"                                /* */
            ", td.numdirs AS ndirs  "                                /*      */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
            ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs "                /* */
            ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles "           /* */
#endif
            ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "        /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
# ifdef DUF_USE_TMP_PATHTOT_DIRS_TABLE
            " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
# else
            " LEFT JOIN ( SELECT parents." DUF_SQL_IDFIELD " AS Pathid, COUNT( * ) AS numdirs " /* */
            "   FROM " DUF_SQL_TABLES_PATHS_FULL " AS pts "          /* */
#  if 1
            "       LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptsp ON( pts.parentid = ptsp." DUF_SQL_IDFIELD " ) " /* */
            "            JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD " = ptsp.parentid ) " /* */
#  else
            "            JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON( parents." DUF_SQL_IDFIELD " = pts.parentid ) " /* */
#  endif
            "   GROUP BY parents." DUF_SQL_IDFIELD ") AS td  ON (td.Pathid=pt." DUF_SQL_IDFIELD ") "
# endif
# ifdef DUF_USE_TMP_PATHTOT_FILES_TABLE
            " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
# else
            " LEFT JOIN ( SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min( size ) AS minsize, max( size ) AS maxsize " /* */
            "   FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "       /* */
            "       LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
            "   GROUP BY fn.Pathid ) AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD " ) "
# endif
#endif
            " WHERE " DUF_DBPREF "pt.ParentId=:parentdirID AND (:dirName IS NULL OR dname=:dirName)" /* */
  };
  DUF_STARTR( r );

  assert( pdi );
  assert( pdi->pdi_name );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */
#if 0
  sqlv = duf_selector2sql( sql_set ? sql_set : &def_node_set, pdi->pdi_name, &r );
#else
  sqlv = duf_selector2sql_new( sql_set ? sql_set : &def_node_set, pdi->pdi_name, 0 /*total */ , &r );
#endif
  DOR( r, duf_set_dirid_and_nums_from_sql( pdi, sqlv ) );            /* at levinfo current level: set dirid,numdir,numfile by pdi and sql; */
  DUF_TRACE( path, 2, "(%d:%s) dirid: %llu for '%s' at %llu", r, mas_error_name_i( r ), duf_levinfo_dirid( pdi ), duf_levinfo_itemtruename( pdi ),
             duf_levinfo_dirid_up( pdi ) );
  if ( DUF_NOERROR( r ) )
    dirid = duf_levinfo_dirid( pdi );
  mas_free( sqlv );
  DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, duf_levinfo_itemshowname( pdi ) );
  assert( dirid == duf_levinfo_dirid( pdi ) );
/* assert( !need_id || dirid ); */
  DUF_ENDR( r );
}

/* TODO remake to pdiitem2dirid?? */
/* at levinfo current level: set dirid,numdir,numfile */
static int
_duf_levinfo_stat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set, int need_id )
{
  DUF_STARTR( r );

  assert( pdi );
  assert( pdi->pdi_name );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  DUF_TRACE( path, 10, "@@@@@@@@@@@ %llu/%llu; caninsert:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), caninsert, pdi ? 1 : 0 );
  DUF_TRACE( path, 2, "@inserting [%s] caninsert:%d", duf_levinfo_itemshowname( pdi ), caninsert );
  {
    int changes = 0;

    DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set ) );   /* at levinfo current level:
                                                                        set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */
    DUF_TRACE( path, 2, "(%d:%s) dirid before insert: %llu for '%s' at %llu", r, mas_error_name_i( r ), duf_levinfo_dirid( pdi ),
               duf_levinfo_itemtruename( pdi ), duf_levinfo_dirid_up( pdi ) );

    if ( duf_levinfo_dirid( pdi ) <= 0 && caninsert && !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      DOR( r, duf_levinfo_stat_insert2db( pdi, &changes ) );         /* insert dir info to db by pdi and possbly fs */
    }
    DUF_TRACE( select, 10, "<changes> : %d : %s", changes, duf_levinfo_itemshowname( pdi ) );
    if ( duf_levinfo_dirid( pdi ) <= 0 && need_id )
    {
      if ( changes )
      {
        if ( DUF_NOERROR( r ) )
        {
          DOR( r, duf_set_dirid_and_nums( pdi, duf_sql_last_insert_rowid(  )
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
                                          , 0, 0
#endif
                ) );                                                 /* at levinfo current level:
                                                                        set dirid from last inserted record (numdir=0,numfile=0) */
          if ( duf_levinfo_dirid_up( pdi ) )
          {
            DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", duf_levinfo_itemshowname( pdi ), duf_levinfo_dirid_up( pdi ),
                       duf_levinfo_dirid_up( pdi ) );
          }
          if ( !duf_levinfo_dirid( pdi ) )
          {
          /* DUF_SHOW_ERROR( "(2) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ), duf_levinfo_itemshowname( pdi ) ); */
            DUF_MAKE_ERRORM( r, DUF_ERROR_NOT_IN_DB, "(2) no dirid by parentid=%llu AND " DUF_SQL_DIRNAMEFIELD "='%s'", duf_levinfo_dirid_up( pdi ),
                             duf_levinfo_itemshowname( pdi ) );
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
            assert( 0 );                                             /*  */
            DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set ) ); /* at levinfo current level:
                                                                              set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */

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
            assert( 0 );                                             /*  */
            DOR( r, duf_set_dirid_and_nums_from_sql_set( pdi, sql_set ) ); /* at levinfo current level:
                                                                              set dirid,numdir,numfile by pdi and sql set (duf_sql_set_t); */

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
    _DUF_SHOW_ERROR( "@@@@@@@@(%s) no dirid by parentid=%llu and " DUF_SQL_DIRNAMEFIELD "='%s'", mas_error_name( r ), duf_levinfo_dirid_up( pdi ),
                     duf_levinfo_itemshowname( pdi ) );
#endif
  }
  DUF_ENDR( r );
}

/* 20151026.112249 */
/* at levinfo current level: set dirid,numdir,numfile; WRAPper for _duf_levinfo_stat2dirid */
int
duf_levinfo_stat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set )
{
  DUF_STARTR( r );
  DOR( r, _duf_levinfo_stat2dirid( pdi, caninsert, sql_set, 1 ) );   /* at levinfo current level: set dirid,numdir,numfile */
  DUF_ENDR( r );
}

/* 20151026.112425 */
/* at levinfo down level: set dirid,numdir,numfile; make level current */
static int
duf_levinfo_down_stat2dirid( duf_depthinfo_t * pdi, const char *directory_name, int caninsert, const duf_sql_set_t * sql_set )
{
  DUF_STARTR( r );
  int up_d = 0;

  assert( pdi );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  up_d = duf_pdi_depth( pdi );

  DOR( r, duf_levinfo_godown_openat_dh( pdi, directory_name, DUF_NODE_NODE /* node_type */  ) ); /* check depth; 
                                                                                                    resets levinfo  (currenl level);
                                                                                                    may change levinfo (for upper level);
                                                                                                    does not really open dh?!;
                                                                                                    XXX equal XXX to duf_levinfo_godown 
                                                                                                    - except DUF_ERROR_TOO_DEEP ? XXX; */

  assert( !DUF_NOERROR( r ) || up_d + 1 == duf_pdi_depth( pdi ) );
  DOR( r, duf_levinfo_stat2dirid( pdi, caninsert, sql_set /*, 1 -- need_id   - error (0= not) if there is no record */  ) ); /* at levinfo current level:
                                                                                                                                set dirid,numdir,numfile;
                                                                                                                                WRAPper for _duf_levinfo_stat2dirid */

  DUF_ENDR( r );
}

/* 20151026.113322 */
/* parse real_path to components and store/check each to db,
 *   setting each level info to levinfo; note: sets depth + n */
static int
_duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int caninsert, const duf_sql_set_t * sql_set /* const char *node_selector2 */  )
{
  DUF_STARTR( r );
  int od = 0;

/* unsigned long long parentid = 0; */
  assert( pdi );
  assert( pdi->pdi_name );
  assert( duf_levinfo_dirid( pdi ) == 0 );
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  DUF_TRACE( fs, 2, "set opendir to 1 (one)" );
  od = duf_pdi_set_opendir( pdi, 1 );                                /* save open status to restore */
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

      DUF_TRACE( path, 6, "@@      +prepared path:[%s]\tup to [%s]", path, nextdir );
      {
        int up_d = 0;

        up_d = duf_pdi_depth( pdi );
      /* down to next */
        DOR( r, duf_levinfo_down_stat2dirid( pdi, path, caninsert, sql_set ) ); /* at levinfo down level: set dirid,numdir,numfile; make level current */
        assert( !DUF_NOERROR( r ) || up_d + 1 == duf_pdi_depth( pdi ) );
      }
      path = nextdir;
    }
  }
  DUF_TRACE( fs, 2, "set (restore) opendir to %d", od );
  duf_pdi_set_opendir( pdi, od );                                    /* restore saved open status */
  assert( !DUF_NOERROR( r ) || !caninsert || !real_path || duf_levinfo_dirid( pdi ) > 0 );
  assert( !DUF_NOERROR( r ) || !caninsert || !real_path || duf_levinfo_path( pdi ) );
  DUF_ENDR( r );
}

/* parse real_path to components and store/check each to db, setting each level info to levinfo */
int
duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set )
{
  DUF_STARTR( r );
  char *real_path;

  assert( pdi );
  assert( pdi->pdi_name );
/* assert( pdi->depth == -1 ); */
  assert( !sql_set || sql_set->type == DUF_NODE_NODE );              /* 20160214.162255 */

  real_path = mas_strdup( rpath );

  assert( real_path != rpath );                                      /* MEMORY ERROR !! FIXME  FIXME  FIXME  FIXME  FIXME  FIXME */

  DUF_TRACE( path, 6, "@@@%s PATHID for       [%40s]", caninsert ? "ADD" : "GET", real_path );
  {
    DUF_TRACE( explain, 0, "real_path: ≪%s≫", real_path );
  /* _duf_real_path2db
   *  need a copy of real_path to modify during parse
   *
   *   note: sets depth + n
   * */
    DOR( r, _duf_real_path2db( pdi, real_path, caninsert, sql_set ) ); /* parse real_path to components and store/check each to db,
                                                                          setting each level info to levinfo;
                                                                          -XXX- sets depth + n -XXX- */
    duf_pdi_set_topdepth( pdi );
  }
  mas_free( real_path );

  DUF_ENDR( r );
}
