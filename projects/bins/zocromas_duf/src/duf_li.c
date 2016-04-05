/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */
#include "duf_pdi_ref.h"

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_evsql_selector_new.h"                                  /* duf_selector2sql_new; duf_selector2sql_2new; duf_expand_sql; ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_levinfo_structs.h"

/* ###################################################################### */
#include "duf_li.h"
/* ###################################################################### */

/* int                                            */
/* duf_li_calc_depth( const duf_levinfo_t * pli ) */
SRX( LI, int, d, -1, li_calc_depth, const duf_levinfo_t * pli )
{
/* int d = -1; */

  while ( pli && pli->itemname )
  {
  /* if ( d < 0 ) */
  /*   d = 0;     */
    d++;
    pli++;
  }
/* return d; */
  ERX( LI, int, d, 0, li_calc_depth, const duf_levinfo_t * pli );
}

/* char *                                              */
/* duf_li_path( const duf_levinfo_t * pli, int count ) */
SRX( LI, char *, path, NULL, li_path, const duf_levinfo_t * pli, int count )
{
/* char *path = NULL; */
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
    MAST_TRACE( path, 4, "path:%s", path );
    l = strlen( pli[i].itemname );
    if ( l > 0 )
    {
      strcpy( p, pli[i].itemname );
      p += l;
      *p++ = '/';
    }
    *p = 0;
  }

/* return path; */
  ERX( LI, char *, path, NULL, li_path, const duf_levinfo_t * pli, int count );
}

static void
li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg )
{
  assert( pli );
  pli->db.dirid = DUF_GET_QUFIELD3( pstmt_arg, dirid );
  if ( DUF_GET_QSFIELD3( pstmt_arg, dfname ) )
  {
    assert( !pli->itemname );
    pli->itemname = mas_strdup( DUF_GET_QSFIELD3( pstmt_arg, dfname ) );
  }

  pli->db.nameid = DUF_GET_QUFIELD3( pstmt_arg, nameid );
  {
    duf_dirhandle_t *pdhlev = &pli->lev_dh;

    pdhlev->st.st_dev = DUF_GET_QUFIELD3( pstmt_arg, dev );
    pdhlev->st.st_ino = DUF_GET_QUFIELD3( pstmt_arg, inode );
    pdhlev->st.st_mode = DUF_GET_QUFIELD3( pstmt_arg, filemode );
    pdhlev->st.st_nlink = DUF_GET_QUFIELD3( pstmt_arg, nlink );
    pdhlev->st.st_uid = DUF_GET_QUFIELD3( pstmt_arg, uid );
    pdhlev->st.st_gid = DUF_GET_QUFIELD3( pstmt_arg, gid );
    pdhlev->st.st_rdev = DUF_GET_QUFIELD3( pstmt_arg, rdev );
    pdhlev->st.st_size = DUF_GET_QUFIELD3( pstmt_arg, filesize );
    pdhlev->st.st_blksize = DUF_GET_QUFIELD3( pstmt_arg, blksize );
    pdhlev->st.st_blocks = DUF_GET_QUFIELD3( pstmt_arg, blocks );
  /* pdhlev->st.st_atim =; */
  /* pdhlev->st.st_mtim =; */
  /* pdhlev->st.st_ctim =; */
    pdhlev->rdb++;
    pdhlev->source = DUF_DH_SOURCE_DB;
  }
/* mas_free( sp ); */
}

/* void                                                                                            */
/* duf_li_dbinit( duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d ) */
SRN( LI, void, li_dbinit, duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d )
{
  pli->d = d;
  pli->node_type = node_type;
  li_dbinit( pli, pstmt_arg );
  ERN( LI, void, li_dbinit, duf_levinfo_t * pli, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d );
}

static
SR( LI, dirid2li_sql_existed, duf_depthinfo_t * pditemp, const char *sqlv, unsigned long long dirid, duf_levinfo_t * pli,
    unsigned long long *pparentid )
{
/* DUF_STARTR( r ); */

  DUF_SQL_SE_START_STMT( pditemp, dirid2li_existed, sqlv, pstmt_local );
  {
    DUF_SQL_SE_BIND_LL( dirID, dirid, pstmt_local );
    DUF_SQL_SE_STEP( pstmt_local );
    if ( QISERR1_N( SQL_ROW ) )
    {
      MAST_TRACE( select, 0, "<selected> %s", sqlv );

      duf_li_dbinit( pli, pstmt_local, DUF_NODE_NODE, -2 );
      if ( pparentid )
        *pparentid = DUF_GET_QUFIELD3( pstmt_local, parentid );
    }
    else
    {
      MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
    }
  }
  DUF_SQL_SE_END_STMT( pditemp, dirid2li_existed, pstmt_local );
/* DUF_ENDR( r ); */
  ER( LI, dirid2li_sql_existed, duf_depthinfo_t * pditemp, const char *sqlv, unsigned long long dirid, duf_levinfo_t * pli,
      unsigned long long *pparentid );
}

/* dev, inode, rdev, mode, nlink, uid, gid, blksize, blocks, size, atim, atimn, mtim, mtimn, ctim, ctimn, dir_name, parentid, priority, last_updated, inow */
SR( LI, dirid2li_existed, duf_depthinfo_t * pditemp, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid )
{
/* DUF_STARTR( r ); */
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name, pt." DUF_SQL_DIRNAMEFIELD " AS dfname " /* */
            ", pt." DUF_SQL_IDFIELD " AS dirid "                     /* */
            ", pt." DUF_SQL_IDFIELD " AS nameid, pt.mode AS filemode, pt.size AS filesize " /* */
            ", pt.dev, pt.inode, pt.rdev, pt.mode, pt.nlink, pt.uid, pt.gid, pt.blksize, pt.blocks " /* */
            ", pt.size, pt.atim, pt.atimn, pt.mtim, pt.mtimn, pt.ctim, pt.ctimn ",
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "        /* */
    " WHERE pt." DUF_SQL_IDFIELD "=:dirID"
  };

  assert( pditemp );

  {
#if 0
    sqlv = duf_selector2sql( &def_node_set, duf_pdi_name( pditemp ), &r );
#elif 0
    sqlv = duf_selector2sql_new( &def_node_set, 0 /* orderid */ , duf_pdi_name( pditemp ), 0, QPERRIND );
#else
    sqlv = CRP( selector2sql_new, &def_node_set, 0 /* orderid */ , duf_pdi_name( pditemp ), 0 );
#endif
    if ( sqlv )
    {
      CR( dirid2li_sql_existed, pditemp, sqlv, dirid, pli, pparentid );
      mas_free( sqlv );
    }
  }

/* DUF_ENDR( r ); */
  ER( LI, dirid2li_existed, duf_depthinfo_t * pditemp, unsigned long long dirid, duf_levinfo_t * pli, unsigned long long *pparentid );
}

static
SR( LI, nameid2li_sql_existed, duf_depthinfo_t * pditemp, const char *sqlv, unsigned long long nameid, duf_levinfo_t * pli,
    unsigned long long *pdirid )
{
/* DUF_STARTR( r ); */

  DUF_SQL_SE_START_STMT( pditemp, nameid2li_existed, sqlv, pstmt_local );
  {
    DUF_SQL_SE_BIND_LL( nameID, nameid, pstmt_local );
    DUF_SQL_SE_STEP( pstmt_local );
    if ( QISERR1_N( SQL_ROW ) )
    {
      MAST_TRACE( select, 0, "<selected> %s", sqlv );

      duf_li_dbinit( pli, pstmt_local, DUF_NODE_LEAF, -2 );
      if ( pdirid )
        *pdirid = DUF_GET_QUFIELD3( pstmt_local, dirid );
    }
    else
    {
      MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
    }
  }
  DUF_SQL_SE_END_STMT( pditemp, nameid2li_existed, pstmt_local );
/* DUF_ENDR( r ); */
  ER( LI, nameid2li_sql_existed, duf_depthinfo_t * pditemp, const char *sqlv, unsigned long long nameid, duf_levinfo_t * pli,
      unsigned long long *pdirid );
}

/* dev, inode, rdev, mode, nlink, uid, gid, blksize, blocks, size, atim, atimn, mtim, mtimn, ctim, ctimn, dir_name, parentid, priority, last_updated, inow */
SR( LI, nameid2li_existed, duf_depthinfo_t * pditemp, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid )
{
/* DUF_STARTR( r ); */
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "fn.file_name AS file_name, fn.file_name AS dfname, fn." DUF_SQL_IDFIELD " AS nameid " /* */
            ", fd.mode AS filemode, fd.size AS filesize "            /* */
            ", pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS dir_name, pt." DUF_SQL_IDFIELD " AS dirid " /* */
            ", fd.dev, fd.inode, fd.rdev, fd.mode, fd.nlink, fd.uid, fd.gid, fd.blksize, fd.blocks " /* */
            ", fd.size, fd.atim, fd.atimn, fd.mtim, fd.mtimn, fd.ctim, fd.ctimn ",
    .selector2 = " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "    /* */
            " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON(pt." DUF_SQL_IDFIELD "=fn.Pathid) " /* */
            " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON(fd." DUF_SQL_IDFIELD "=fn.dataid) " /* */
    " WHERE fn." DUF_SQL_IDFIELD "=:nameID"
  };

  assert( pditemp );

  {
#if 0
    sqlv = duf_selector2sql( &def_node_set, duf_pdi_name( pditemp ), &r );
#elif 0
    sqlv = duf_selector2sql_new( &def_node_set, 0 /* orderid */ , duf_pdi_name( pditemp ), 0, QPERRIND );
#else
    sqlv = CRP( selector2sql_new, &def_node_set, 0 /* orderid */ , duf_pdi_name( pditemp ), 0 );
#endif
    if ( sqlv )
    {
      CR( nameid2li_sql_existed, pditemp, sqlv, nameid, pli, pdirid );
      mas_free( sqlv );
    }
  }

/* DUF_ENDR( r ); */
  ER( LI, nameid2li_existed, duf_depthinfo_t * pditemp, unsigned long long nameid, duf_levinfo_t * pli, unsigned long long *pdirid );
}

/* const char *                           */
/* duf_li_itemname( duf_levinfo_t * pli ) */
SRX( LI, const char *, itemname, NULL, li_itemname, duf_levinfo_t * pli )
{
  itemname = pli ? pli->itemname : NULL;
  ERX( LI, const char *, itemname, NULL, li_itemname, duf_levinfo_t * pli );
}
