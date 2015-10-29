#include <string.h>

#include "duf_maintenance.h"

#include "duf_context.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_li.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"

#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_levinfo.h"
#include "duf_levinfo_dbinit.h"
/* ###################################################################### */

/* 20151029.114756 */
void
duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt, duf_node_type_t node_type, int d )
{
  assert( pdi );
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );

  duf_levinfo_clear_level_d( pdi, d );
  assert( !pdi->pathinfo.levinfo[d].itemname );
  /* if ( pdi->opendir ) */
  {
    duf_levinfo_t *pli;

    pli = &pdi->pathinfo.levinfo[d];
    pli->node_type = node_type;
    pli->dirid = DUF_GET_UFIELD2( dirid );

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
    pli->nameid = DUF_GET_UFIELD2( nameid );
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
      DUF_TRACE( levinfo, 10, "levinfo openated %s; dfd:%d", pdi->pathinfo.levinfo[d].itemname, pdhlev->dfd );
      pdhlev->rdb++;
      pdhlev->source = DUF_DH_SOURCE_DB;
    }
    /* mas_free( sp ); */
  }
}
