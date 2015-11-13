#include <string.h>

#include "duf_maintenance.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_pdi_stmt.h"

#include "duf_sql_stmt_defs.h"
#include "evsql_selector.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

#include "sql_beginning_tables.h"



/* ###################################################################### */
#include "duf_li.h"
/* ###################################################################### */
#ifndef DUF_NO_NUMS
void
duf_li_set_nums( duf_levinfo_t * pli, unsigned long long ndirs, unsigned long long nfiles )
{
  if ( pli )
  {
    pli->numdir = ndirs;
    pli->numfile = nfiles;
    /* T("@@@@ %s>>%llu", pli->itemname, ndirs ); */
  }
}
#else
#  if 0
void
duf_li_set_childs( duf_levinfo_t * pli, unsigned long long childs )
{
  if ( pli )
  {
    pli->childs = childs;
    /* pli->numchild = childs; */
  }
}
#  endif
#endif

int
duf_li_calc_depth( const duf_levinfo_t * pli )
{
  int d = -1;

  while ( pli && pli->itemname )
  {
    /* if ( d < 0 ) */
    /*   d = 0;     */
    d++;
    pli++;
  }
  return d;
}

char *
duf_li_path( const duf_levinfo_t * pli, int count )
{
  char *path = NULL;

  {
    size_t len = 2;
    char *p;

    assert( count > 0 );
    for ( int i = 0; i < count; i++ )
    {
      assert( pli[i].itemname );
      len += strlen( pli[i].itemname ) + 1;
    }
    path = mas_malloc( len );
    p = path;

    for ( int i = 0; i < count; i++ )
    {
      size_t l;

      if ( p == path || *( p - 1 ) != '/' )
        *p++ = '/';
      *p = 0;
      DUF_TRACE( path, 4, "path:%s", path );
      l = strlen( pli[i].itemname );
      if ( l > 0 )
      {
        strcpy( p, pli[i].itemname );
        p += l;
        *p++ = '/';
      }
      *p = 0;
    }
  }
  return path;
}

static void
_duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt )
{
  assert( pli );
  pli->db.dirid = DUF_GET_UFIELD2( dirid );
  if ( DUF_GET_SFIELD2( dfname ) )
  {
    assert( !pli->itemname );
    pli->itemname = mas_strdup( DUF_GET_SFIELD2( dfname ) );
  }

#ifndef DUF_NO_NUMS
  /* pli->numdir = DUF_GET_UFIELD2( ndirs );   */
  /* pli->numfile = DUF_GET_UFIELD2( nfiles ); */
  duf_li_set_nums( pli, DUF_GET_UFIELD2( ndirs ), DUF_GET_UFIELD2( nfiles ) );
#else
  /* if ( duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE ) */
  /*   duf_levinfo_make_childs_d( pdi, d );                    */
#endif
  pli->db.nameid = DUF_GET_UFIELD2( nameid );
  {
    duf_dirhandle_t *pdhlev = &pli->lev_dh;

    pdhlev->st.st_dev = DUF_GET_UFIELD2( dev );
    pdhlev->st.st_ino = DUF_GET_UFIELD2( inode );
    pdhlev->st.st_mode = DUF_GET_UFIELD2( filemode );
    pdhlev->st.st_nlink = DUF_GET_UFIELD2( nlink );
    pdhlev->st.st_uid = DUF_GET_UFIELD2( uid );
    pdhlev->st.st_gid = DUF_GET_UFIELD2( gid );
    pdhlev->st.st_rdev = DUF_GET_UFIELD2( rdev );
    pdhlev->st.st_size = DUF_GET_UFIELD2( filesize );
    pdhlev->st.st_blksize = DUF_GET_UFIELD2( blksize );
    pdhlev->st.st_blocks = DUF_GET_UFIELD2( blocks );
    /* pdhlev->st.st_atim =; */
    /* pdhlev->st.st_mtim =; */
    /* pdhlev->st.st_ctim =; */
    pdhlev->rdb++;
    pdhlev->source = DUF_DH_SOURCE_DB;
  }
  /* mas_free( sp ); */
}

void
duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d )
{
  pli->d = d;
  pli->node_type = node_type;
  _duf_li_dbinit( pli, pstmt );
}

static int
_duf_dirid2li_existed( duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT( pdi, dirid2li_existed, sqlv, r, pstmt );
  {
    DUF_SQL_BIND_LL( dirID, dirid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    if ( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) )
    {
      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      duf_li_dbinit( pli, pstmt, DUF_NODE_NODE, -2 );
      if ( pparentid )
        *pparentid = DUF_GET_UFIELD2( parentid );
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
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name, pt." DUF_SQL_DIRNAMEFIELD " AS dfname " /* */
          ", pt." DUF_SQL_IDFIELD " AS dirid " /* */
          ", pt." DUF_SQL_IDFIELD " AS nameid, pt.mode AS filemode, pt.size AS filesize " /* */
          ", pt.dev, pt.inode, pt.rdev, pt.mode, pt.nlink, pt.uid, pt.gid, pt.blksize, pt.blocks " /* */
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


static int
_duf_nameid2li_existed( duf_depthinfo_t * pdi, const char *sqlv, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT( pdi, nameid2li_existed, sqlv, r, pstmt );
  {
    DUF_SQL_BIND_LL( nameID, nameid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    if ( DUF_IS_ERROR_N( r, DUF_SQL_ROW ) )
    {
      DUF_TRACE( select, 0, "<selected> %s", sqlv );

      duf_li_dbinit( pli, pstmt, DUF_NODE_LEAF, -2 );

      if ( pdirid )
        *pdirid = DUF_GET_UFIELD2( dirid );
    }
    else
    {
      DUF_TRACE( select, 10, "<NOT selected> (%d)", r );
    }
  }
  DUF_SQL_END_STMT( pdi, nameid2li_existed, r, pstmt );
  DEBUG_ENDR( r );
}

/* dev, inode, rdev, mode, nlink, uid, gid, blksize, blocks, size, atim, atimn, mtim, mtimn, ctim, ctimn, dir_name, parentid, priority, last_updated, inow */
int
duf_nameid2li_existed( duf_depthinfo_t * pdi, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid )
{
  DEBUG_STARTR( r );
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "fn.file_name AS file_name, fn.file_name AS dfname, fn." DUF_SQL_IDFIELD " AS nameid " /* */
          ", fd.mode AS filemode, fd.size AS filesize " /* */
          ", pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS dir_name, pt." DUF_SQL_IDFIELD " AS dirid " /* */
          ", fd.dev, fd.inode, fd.rdev, fd.mode, fd.nlink, fd.uid, fd.gid, fd.blksize, fd.blocks " /* */
          ", fd.size, fd.atim, fd.atimn, fd.mtim, fd.mtimn, fd.ctim, fd.ctimn ",
    .selector2 = " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON(pt." DUF_SQL_IDFIELD "=fn.Pathid) " /* */
          " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON(fd." DUF_SQL_IDFIELD "=fn.dataid) " /* */
#ifndef DUF_NO_NUMS
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
          " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
    " WHERE fn." DUF_SQL_IDFIELD "=:nameID"
  };

  assert( pdi );

  {
    sqlv = duf_selector2sql( &def_node_set, pdi->pdi_name, &r );
    if ( sqlv )
    {
      DOR( r, _duf_nameid2li_existed( pdi, sqlv, nameid, pli, pdirid ) );
      mas_free( sqlv );
    }
  }

  DEBUG_ENDR( r );
}
