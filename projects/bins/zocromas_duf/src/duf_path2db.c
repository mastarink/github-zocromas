#define DUF_SQL_PDI_STMT

#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"
#include "duf_utils_path.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_defs.h"

#include "duf_sql.h"
#include "duf_sql2.h"

/* ###################################################################### */
#include "duf_path2db.h"
/* ###################################################################### */




/* insert path into db; return ID */
unsigned long long
duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int ifadd, dev_t dev_id, ino_t dir_ino,
                      /* unsigned long long parentid_unused : unused, */ int need_id, int *pchanges, int *pr )
{
  unsigned long long dirid = 0;
  int r = 0;

  DEBUG_START(  );
  assert( pdi );

  /* assert( parentid_unused == duf_levinfo_dirid_up( pdi ) ); */

  /* DUF_TRACE( temp, 0, "@@@@@@@@@@@ %llu/%llu/%llu; ifadd:%d; pdi:%d", parentid_unused, duf_levinfo_dirid( pdi ), */
  /*            duf_levinfo_dirid_up( pdi ), ifadd, pdi ? 1 : 0 );                                                  */
  DUF_TRACE( path, 2, "@@@@@@@@@@@ %llu/%llu; ifadd:%d; pdi:%d", duf_levinfo_dirid( pdi ), duf_levinfo_dirid_up( pdi ), ifadd, pdi ? 1 : 0 );
  /* unsigned char c1 = ( unsigned char ) ( dename ? *dename : 0 ); */
  if ( dename /* && dev_id && dir_ino */  )
  {
    int changes = 0;

    if ( ifadd && !duf_config->cli.disable.flag.insert )
    {
      static const char *sql =
            "INSERT OR IGNORE INTO " DUF_DBPREF "paths ( dev, inode, dirname, parentid ) VALUES (:Dev, :iNode, :dirName, :parentID )";
      if ( pdi )
      {
        DUF_SQL_START_STMT( pdi, insert_path, sql, r, pstmt );

        DUF_TRACE( insert, 0, "S:%s (%lu,%lu,'%s',%llu)", sql, dev_id, dir_ino, dename, duf_levinfo_dirid_up( pdi ) );
        /* DUF_SHOW_ERROR( "insert_path_index:%d", insert_path_index ); */
        DUF_SQL_BIND_LL( Dev, dev_id, r, pstmt );
        DUF_SQL_BIND_LL( iNode, dir_ino, r, pstmt );
        DUF_SQL_BIND_S( dirName, dename, r, pstmt );
        /* DUF_SQL_BIND_LL( parentID, parentid, r, pstmt ); */
        DUF_SQL_BIND_LL( parentID, duf_levinfo_dirid_up( pdi ), r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( insert_path, r, pstmt );
      }
      else
      {
      }
    }
    else
    {
    }
    /* sql = NULL; */
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    /* if ( r == DUF_SQL_CONSTRAINT ) */
    if ( need_id )
    {
      if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
      {
        /* duf_sqlite_stmt_t *pstmt = NULL; */
        const char *sql = "SELECT " DUF_DBPREF "paths." DUF_SQL_IDNAME " AS dirID, " DUF_DBPREF "paths.dirname " /*      */
              ", tf.numfiles AS nfiles, td.numdirs AS ndirs " /*      */
              " FROM " DUF_DBPREF "paths LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.pathid=" DUF_DBPREF "paths." DUF_SQL_IDNAME ") " /*      */
              " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.pathid=" DUF_DBPREF "paths." DUF_SQL_IDNAME ") " /*      */
              " WHERE " DUF_DBPREF "paths.parentid=:parentID AND dirname=:dirName";

        if ( pdi )
        {
          DUF_SQL_START_STMT( pdi, select_path, sql, r, pstmt );
          /* DUF_TRACE( select, 0, "S:%s (%llu,'%s')", sql, parentid, dename ); */
          DUF_TRACE( select, 0, "S:%s (%llu,'%s')", sql, duf_levinfo_dirid_up( pdi ), dename );
          /* DUF_SQL_BIND_LL( parentID, parentid, r, pstmt ); */
          DUF_SQL_BIND_LL( parentID, duf_levinfo_dirid_up( pdi ), r, pstmt );
          DUF_SQL_BIND_S( dirName, dename, r, pstmt );
          DUF_SQL_STEP( r, pstmt );
          if ( r == DUF_SQL_ROW )
          {
            DUF_TRACE( current, 0, "<selected>" );
            dirid = duf_sql_column_long_long( pstmt, 0 );
            if ( need_id && !dirid )
            {
              /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename ); */
              DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", duf_levinfo_dirid_up( pdi ), dename );
            }
            else
            {
              DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, duf_levinfo_dirid_up( pdi ) );
            }
            assert( !need_id || dirid );
            {
              duf_levinfo_t *pli;

              pli = duf_levinfo_ptr( pdi );
              if ( pli )
              {
                pli->dirid = dirid;
                /* pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) ); */
                pli->numfile = duf_sql_column_long_long( pstmt, 2 );
                pli->numdir = duf_sql_column_long_long( pstmt, 3 );
              }
            }
            r = 0;
          }
          else
          {
            DUF_TEST_R( r );
            DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
          }
          DUF_SQL_END_STMT( select_path, r, pstmt );
        }
        else
        {
          DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
          /* DUF_SQL_BIND_LL( dirID, parentid, r, pstmt ); */
          DUF_SQL_BIND_LL( dirID, duf_levinfo_dirid_up( pdi ), r, pstmt );
          DUF_SQL_BIND_S( dirName, dename, r, pstmt );
          DUF_SQL_STEP( r, pstmt );

          if ( r == DUF_SQL_ROW )
          {
            DUF_TRACE( current, 0, "<selected>" );
            dirid = duf_sql_column_long_long( pstmt, 0 );
            if ( need_id && !dirid )
            {
              /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename ); */
              DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", duf_levinfo_dirid_up( pdi ), dename );
            }
            else
            {
              DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, duf_levinfo_dirid_up( pdi ) );
            }
            assert( !need_id || dirid );
            {
              duf_levinfo_t *pli;

              pli = duf_levinfo_ptr( pdi );
              if ( pli )
              {
                /* needless ?? FIXME */
                pli->dirid = dirid;
                /* needless ?? FIXME */
                pli->itemname = mas_strdup( duf_sql_column_string( pstmt, 1 ) );
                /* needless ?? FIXME */
                pli->numfile = duf_sql_column_long_long( pstmt, 2 );
                /* needless ?? FIXME */
                pli->numdir = duf_sql_column_long_long( pstmt, 3 );
              }
            }
            r = 0;
          }
          else
          {
            DUF_TEST_R( r );
            DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
          }
          DUF_SQL_END_STMT_NOPDI( r, pstmt );
        }
        DUF_TEST_R( r );
        DUF_TRACE( collect, 1, "sometime inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      else if ( !r /* assume SQLITE_OK */  && changes )
      {
        dirid = duf_sql_last_insert_rowid(  );
        if ( need_id && !dirid )
        {
          /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename ); */
          DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", duf_levinfo_dirid_up( pdi ), dename );
        }
        else
        {
          DUF_TRACE( explain, 0, "   ≪%s≫ in db as %llu @ %llu", dename, dirid, duf_levinfo_dirid_up( pdi ) );
        }
        assert( !need_id || dirid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dirid=%llu:'%s'", dirid, dename );
      }
      DUF_TEST_R( r );
    }
    if ( pchanges )
      *pchanges = changes;
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data dename(%s) OR dev_id(%lu) OR dir_ino(%lu)", dename, dev_id, dir_ino );
    r = DUF_ERROR_DATA;
  }
  if ( pr )
    *pr = r;
  if ( need_id && !dirid )
  {
    /* DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", parentid, dename ); */
    DUF_SHOW_ERROR( "no dirid by parentid=%llu and dename='%s'", duf_levinfo_dirid_up( pdi ), dename );
  }
  DUF_TEST_R( r );
  /* assert( !need_id || dirid ); */
  DEBUG_ENDULL( dirid );
  return dirid;
}

/*
 * store/check path component to db
 *   levinfo depth 1 level lower
 *   and set each level info at levinfo
 *   anyway get the ID
 * */
static int
duf_path_component2db( duf_depthinfo_t * pdi, const char *insdir, int caninsert, unsigned long long *pparentid )
{
  DEBUG_STARTR( r );

  /* assert( pdi );                                                                          */
  assert( pparentid );
  DOR( r, duf_levinfo_godown_openat_dh( pdi, 0, insdir, 0 /* ndirs */ , 0 /* nfiles */ , 0 /* is_leaf */  ) );
  /* DOR( r, duf_levinfo_openat_dh( pdi ) ); (* levinfo depth 1 level lower *) */

  DUF_TRACE( explain, 4, "already opened (at) ≪%s≫ upfd:%d", insdir, duf_levinfo_dfd( pdi ) );

  if ( r >= 0 )
  {
    int changes;

    changes = 0;
    DUF_TRACE( path, 0, "to insert [%s] pdi:%d", insdir ? insdir : "/", pdi ? 1 : 0 );
    /* store/check path component to db; anyway get the ID */
    *pparentid = duf_insert_path_uni2( pdi, insdir, caninsert, duf_levinfo_stat_dev( pdi ), duf_levinfo_stat_inode( pdi ), 1 /*need_id */ ,
                                       &changes, &r );
    /* assert( *pparentid ); */
    if ( changes )
      DUF_TRACE( explain, 0, "added ID: %llu for ≪%s≫", *pparentid, insdir );
    else
      DUF_TRACE( explain, 1, "already in db ID: %llu for ≪%s≫", *pparentid, insdir );

    duf_levinfo_set_dirid( pdi, *pparentid );
    DUF_TRACE( path, 0, "inserted [%s] AS %llu", insdir, *pparentid );
    DUF_TRACE( path, 0, "ID %llu for insdir ≪%s≫", *pparentid, insdir );
  }
  else
  {
    /* duf_levinfo_goup( pdi ); */
  }
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
static int
_duf_real_path2db( duf_depthinfo_t * pdi, char *real_path, int ifadd )
{
  DEBUG_STARTR( r );
  int od = 0;
  unsigned long long parentid = 0;

  assert( pdi );
  od = duf_pdi_set_opendir( pdi, 1 ); /* save open status to restore */
  {
    int upfd = 0;
    char *dir;

    dir = real_path;

    DUF_TRACE( path, 0, "%s PATHID for %s", ifadd ? "ADD" : "GET", real_path );
    while ( r >= 0 && dir && *dir )
    {
      /* struct stat *pst_dir = NULL; */
      char *nextdir = dir;

      DUF_TRACE( path, 0, "prepare [%s] under %llu", dir, parentid );
      {
        /* extracting pathname component */
        while ( nextdir && *nextdir && *nextdir != '/' )
          nextdir++;
        if ( nextdir && *nextdir == '/' )
          *nextdir++ = 0;
      }
      {
        DUF_TRACE( path, 0, "prepared [%s] up to [%s]", dir, nextdir );
/* now dir is pathname component;
 *
 * store/check it to db
 *   levinfo depth 1 level lower
 *   and set each level info at levinfo
 *   anyway get the ID
 *   
 *     depth + 1
 * */
        DOR( r, duf_path_component2db( pdi, dir, ifadd, &parentid ) );
        if ( r < 0 )
          DUF_SHOW_ERROR( "No such entry %s [%s]", real_path, dir );
        dir = nextdir;
      }
      DUF_TRACE( path, 0, "next [%s] under %llu", dir, parentid );
    }
    if ( !pdi && upfd )
      close( upfd );
    upfd = 0;
  }
  duf_pdi_set_opendir( pdi, od ); /* restore saved open status */
  if ( r >= 0 && !parentid )
    r = DUF_ERROR_NOT_IN_DB;
  DEBUG_ENDR( r );
}

/*
 * parse real_path to components and store/check each to db,
 *   setting each level info to levinfo
 * */
int
duf_real_path2db( duf_depthinfo_t * pdi, const char *rpath, int ifadd )
{
  DEBUG_STARTR( r );
  char *real_path;

  assert( pdi );
  /* assert( pdi->depth == -1 ); */

  real_path = mas_strdup( rpath );

  DUF_TRACE( explain, 0, "real_path: ≪%s≫", real_path );
  /* _duf_real_path2db
   *  need a copy of real_path to modify during parse
   *
   *   note: sets depth + n
   * */
  r = _duf_real_path2db( pdi, real_path, ifadd );
  duf_pdi_set_topdepth( pdi );
  DUF_TEST_R( r );

  mas_free( real_path );

  DEBUG_ENDR( r );
}

unsigned long long
duf_path2db( const char *path, int *pr )
{
  int r = 0;
  char *real_path;
  unsigned long long dirid = 0;

  real_path = duf_realpath( path, &r );
  duf_depthinfo_t di = {.depth = -1,
    .seq = 0,
    .levinfo = NULL,
    .pu = &duf_config->u,
    /* .opendir = sccb ? sccb->opendir : 0, */
    .opendir = 1,
    /* .name = real_path, */
  };
  if ( r >= 0 )
    r = duf_pdi_init_wrap( &di, real_path, 0, 1 /* recursive */  );
  if ( r >= 0 )
    dirid = duf_levinfo_dirid( &di );

  duf_pdi_close( &di );
  mas_free( real_path );
  return dirid;
}
