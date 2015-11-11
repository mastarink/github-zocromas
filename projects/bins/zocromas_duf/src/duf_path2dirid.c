/* 20150901.122226 */
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"



#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"
#include "evsql_selector.h"

/* #include "duf_sql.h" */
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2dirid.h"
/* ###################################################################### */

/* 20150901.121302
 * use temporary inited pdi
 * to get dirid for path
 * */
unsigned long long
duf_path2dirid( const char *path, int *pr )
{
  int rpr = 0;
  char *real_path;
  unsigned long long dirid = 0;

  real_path = mas_realpath( path /* , &rpr */  );
  if ( !real_path )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_PATH );

  if ( DUF_NOERROR( rpr ) )
  {
    duf_depthinfo_t di = { 0 };

#if 0
    DOR( rpr, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL /* pu */ , real_path, NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                            0 /* opendir */  ) );
#else
    DOR( rpr, duf_pdi_init_min( &di, real_path ) );
#endif



    if ( DUF_NOERROR( rpr ) )
      dirid = duf_levinfo_dirid( &di );
    /* xchanges = di.changes; --- needless!? */
    duf_pdi_shut( &di );
  }
  mas_free( real_path );
  if ( pr )
    *pr = rpr;
  return dirid;
}

static char *
_duf_dirid2name_existed( duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid, unsigned long long *pparentid, int *pr )
{
  int rpr = 0;
  char *name = NULL;

  DEBUG_START(  );
  DUF_SQL_START_STMT( pdi, dirid2name_existed, sqlv, rpr, pstmt );
  {
    DUF_SQL_BIND_LL( dirID, dirid, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
    {
      /* rpr = 0; */
      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      name = mas_strdup( DUF_GET_SFIELD2( name ) );
      if ( pparentid )
        *pparentid = DUF_GET_UFIELD2( parentid );
    }
    else
    {
      DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
    }
  }
  DUF_SQL_END_STMT( pdi, dirid2name_existed, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDS( name );
}

char *
duf_dirid2name_existed( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid, int *pr )
{
  char *name = NULL;

  DEBUG_START(  );

  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name " /* */
          ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
#ifndef DUF_NO_NUMS
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
    " WHERE pt." DUF_SQL_IDFIELD "=:dirID"
  };

  assert( pdi );

  {
    sqlv = duf_selector2sql( &def_node_set, pdi->pdi_name, pr );
    if ( sqlv )
    {
      name = _duf_dirid2name_existed( pdi, sqlv, dirid, pparentid, pr );

      mas_free( sqlv );
    }
  }
  DEBUG_ENDS( name );
}

static int
_duf_dirid2li_existed( duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT( pdi, dirid2li_existed, sqlv, r, pstmt );
  {
    DUF_SQL_BIND_LL( dirID, dirid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    memset( pli, 0, sizeof( duf_levinfo_t ) );
    if ( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) )
    {
      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      /* for ( int icol = 0; icol < duf_sql_column_count( pstmt ); icol++ ) */
      /* {                                                                  */
      /*   const char *fldname;                                             */
      /*                                                                    */
      /*   fldname = duf_sql_column_name( pstmt, icol );                    */
      /*   T( "@@F:%s", fldname );                                          */
      /* }                                                                  */
      pli->itemname = mas_strdup( DUF_GET_SFIELD2( name ) );
      pli->db.dirid = DUF_GET_UFIELD2( dirid );
      if ( pparentid )
        *pparentid = DUF_GET_UFIELD2( parentid );
      /* inode, rdev, mode, nlink, uid, gid, blksize, blocks, size, atim, atimn, mtim, mtimn, ctim, ctimn */
#if 1
      pli->lev_dh.st.st_dev = DUF_GET_UFIELD2( dev );
      pli->lev_dh.st.st_ino = DUF_GET_UFIELD2( inode );
      pli->lev_dh.st.st_rdev = DUF_GET_UFIELD2( rdev );
      pli->lev_dh.st.st_mode = DUF_GET_UFIELD2( mode );
      pli->lev_dh.st.st_nlink = DUF_GET_UFIELD2( nlink );
      pli->lev_dh.st.st_uid = DUF_GET_UFIELD2( uid );
      pli->lev_dh.st.st_gid = DUF_GET_UFIELD2( gid );
      pli->lev_dh.st.st_blksize = DUF_GET_UFIELD2( blksize );
      pli->lev_dh.st.st_blocks = DUF_GET_UFIELD2( blocks );
      pli->lev_dh.st.st_size = DUF_GET_UFIELD2( size );
#endif
      /* pli->lev_dh.st.st_atim = DUF_GET_UFIELD2( atim );   */
      /* pli->lev_dh.st.st_atimn = DUF_GET_UFIELD2( atimn ); */
      /* pli->lev_dh.st.st_mtim = DUF_GET_UFIELD2( mtim );   */
      /* pli->lev_dh.st.st_mtimn = DUF_GET_UFIELD2( mtimn ); */
      /* pli->lev_dh.st.st_ctim = DUF_GET_UFIELD2( ctim );   */
      /* pli->lev_dh.st.st_ctimn = DUF_GET_UFIELD2( ctimn ); */
      pli->node_type = DUF_NODE_NODE;

      /* name = mas_strdup( DUF_GET_SFIELD2( name ) ); */
      /* if ( pparentid ) */
      /* *pparentid = DUF_GET_UFIELD2( parentid ); */
    }
    else
    {
      DUF_TRACE( select, 10, "<NOT selected> (%d)", r );
    }
  }
  DUF_SQL_END_STMT( pdi, dirid2li_existed, r, pstmt );
  DEBUG_ENDR( r );
}

/* dev, inode, rdev, mode, nlink, uid, gid, blksize, blocks, size, atim, atimn, mtim, mtimn, ctim, ctimn, dir_name, parentid, priority, last_updated, inow */
int
duf_dirid2li_existed( duf_depthinfo_t * pdi, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid )
{
  DEBUG_STARTR( r );
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name, pt." DUF_SQL_IDFIELD " AS dirid " /* */
          " , pt.dev, pt.inode, pt.rdev, pt.mode, pt.nlink, pt.uid, pt.gid, pt.blksize, pt.blocks " /* */
          ", pt.size, pt.atim, pt.atimn, pt.mtim, pt.mtimn, pt.ctim, pt.ctimn ",
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
#ifndef DUF_NO_NUMS
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
    " WHERE pt." DUF_SQL_IDFIELD "=:dirID"
  };

  assert( pdi );

  {
    sqlv = duf_selector2sql( &def_node_set, pdi->pdi_name, &r );
    if ( sqlv )
    {
      DOR( r, _duf_dirid2li_existed( pdi, sqlv, dirid, pli, pparentid ) );
      mas_free( sqlv );
    }
  }

  DEBUG_ENDR( r );
}

char *
duf_dirid2path( unsigned long long dirid, int *pr )
{
  char *path = NULL;

  DEBUG_START(  );
  int rpr = 0;
  int done = 0;
  int depth = 0;

  duf_depthinfo_t di = { 0 };
#if 0
  DOR( rpr, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL /* pu */ , NULL /* real_path */ , NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                          0 /* opendir */  ) );
#else
  DOR( rpr, duf_pdi_init_min( &di, NULL /* real_path */  ) );
#endif
  do
  {
    char *name = NULL;

    name = duf_dirid2name_existed( &di, dirid, &dirid, &rpr );
    if ( name )
    {
      char *opath;

      opath = path;
      path = mas_normalize_path_plus( *name ? name : "/", opath, NULL );
      mas_free( opath );
      /* T( "(%llu) '%s' => path:%s", dirid, name, path ); */
      depth++;
    }
    else
      done = 1;
    mas_free( name );
  }
  while ( !done );
  duf_pdi_shut( &di );

  if ( pr )
    *pr = rpr;
  DEBUG_ENDS( path );
}
