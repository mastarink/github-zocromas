#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */

#include "duf_maintenance.h"

/* #include "duf_pdi.h" */
#include "duf_pdi_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"

/* ###################################################################### */
#include "duf_levinfo_updown.h"
/* ###################################################################### */



static void
duf_levinfo_countdown_dirs( duf_depthinfo_t * pdi )
{
  /* assert( pdi );                */
  /* assert( pdi->levinfo );       */
  /* {                             */
  /*   int d = pdi->depth - 1;     */
  /*                               */
  /*   if ( d >= 0 )               */
  /*     pdi->levinfo[d].numdir--; */
  /* }                             */
  duf_levinfo_t *up;

  up = duf_levinfo_ptr_up( pdi );
  if ( up )
    up->numdir--;
}

static int
duf_levinfo_check_depth( const duf_depthinfo_t * pdi, int is_leaf )
{
  DEBUG_STARTR( r );
  int delta;

  delta = ( is_leaf ? 1 : 0 );
  delta = 0;
  /* if ( duf_pdi_recursive( pdi ) )               */
  /* {                                             */
  if ( duf_pdi_depth( pdi ) > duf_pdi_maxdepth( pdi ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
  else if ( !duf_pdi_is_good_depth( pdi, delta ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_TOO_DEEP );

  if ( r < 0 )
    DUF_TRACE( depth, 0, "(%d) DEPTH: d=%d; max:%d; top:%d; delta:%d; R:%d; ", r, duf_pdi_depth( pdi ), duf_pdi_maxdepth( pdi ),
               duf_pdi_topdepth( pdi ), delta, duf_pdi_recursive( pdi ) );
  /* }                                             */
  /* else if ( duf_pdi_reldepth( pdi ) > delta )   */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_MAX_DEPTH);    */
  DEBUG_ENDR( r );
}

static int
duf_levinfo_godown_dnn( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                        unsigned long long nfiles, int is_leaf )
{
  DEBUG_STARTR( r );

  assert( pdi );

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  if ( DUF_NOERROR( r ) )
  {
    int d;

    d = ++pdi->depth;
    assert( pdi->depth >= 0 );
    assert( d >= 0 );
    assert( d == pdi->depth );
    /* assume only level 0 may have dirid==0 -- AND: simply dirid not set */
    /* assert( d == 0 || ( d > 0 && dirid  ) ); */

    /*!! if ( d <= pdi->maxdepth ) */
    {
      assert( pdi->levinfo );
      /* assert( dirid == 0 ); */
      /* DUF_TRACE(temp,0,"%llu - %llu" , duf_levinfo_dirid( pdi ), dirid ); */
      duf_levinfo_init_level_d( pdi, itemname, dirid, ndirs, nfiles, is_leaf, d );


      DUF_TRACE( explain, 2, "level down: %d; ≪%s≫  [%s]", d, is_leaf ? "leaf" : "node", duf_levinfo_itemshowname( pdi ) );
      if ( is_leaf )
        DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf    =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
      else
      {
        duf_levinfo_countdown_dirs( pdi );
        DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node:   =>  by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
                   duf_levinfo_itemshowname( pdi ) );
      }
    }
  }
  DEBUG_ENDR( r );
}

int
duf_levinfo_godown( duf_depthinfo_t * pdi, const char *itemname, int is_leaf )
{
  return duf_levinfo_godown_dnn( pdi, 0 /* dirid */ , itemname, 0 /* ndirs */ , 0 /* nfiles */ , is_leaf );
}

/*
 * 1. check depth
 * 2. duf_levinfo_godown_dnn
 * 2.1. check depth
 * */
static int
duf_levinfo_godown_openat_dnn_dh( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                                  unsigned long long nfiles, int is_leaf )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  assert( dirid == 0 );
  DOR_NOE( r, duf_levinfo_godown_dnn( pdi, dirid, itemname, ndirs, nfiles, is_leaf ), DUF_ERROR_TOO_DEEP );

  DEBUG_ENDR_NOE( r, DUF_ERROR_TOO_DEEP );
}

int
duf_levinfo_godown_openat_dh( duf_depthinfo_t * pdi, const char *itemname, int is_leaf )
{
  return duf_levinfo_godown_openat_dnn_dh( pdi, 0 /* dirid */ , itemname, 0 /* ndirs */ , 0 /*nfiles */ , is_leaf );
}

static int
duf_levinfo_godown_dbopenat_dnn_dh( duf_depthinfo_t * pdi, unsigned long long dirid, const char *itemname, unsigned long long ndirs,
                                    unsigned long long nfiles, int is_leaf, duf_sqlite_stmt_t * pstmt )
{
  DEBUG_STARTR( r );

  DOR( r, duf_levinfo_check_depth( pdi, is_leaf ) );
  if ( r >= 0 )
  {
    assert( dirid == 0 );
#if 0
    DOR_NOE( r, duf_levinfo_godown_dnn( pdi, dirid, itemname, ndirs, nfiles, is_leaf ), DUF_ERROR_TOO_DEEP );
#else
    /* DUF_GET_UFIELD2( dirid ) needless! can be 0 */
    DOR_NOE( r, duf_levinfo_godown_dnn( pdi, DUF_GET_UFIELD2( dirid ), DUF_GET_SFIELD2( dfname ), DUF_GET_UFIELD2( ndirs ), DUF_GET_UFIELD2( nfiles ), is_leaf ),
             DUF_ERROR_TOO_DEEP );
#endif
    {
      int rd;

      rd = r;
      /* do same as duf_levinfo_openat_dh, but from db: */
      DUF_E_NO( DUF_ERROR_OPENAT_ENOENT );
      DOR( r, duf_levinfo_dbopenat_dh( pdi, pstmt, is_leaf ) );
      DUF_E_YES( DUF_ERROR_OPENAT_ENOENT );

      if ( rd >= 0 && duf_levinfo_item_deleted( pdi ) )
        DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT );

      if ( duf_levinfo_item_deleted( pdi ) )
      {
        DUF_TRACE( fs, 0, "@@(%d;%d;D:%d)? levinfo u/d openated %s; dfd:%d", r, rd, duf_levinfo_item_deleted( pdi ), duf_levinfo_itemshowname( pdi ),
                   duf_levinfo_dfd( pdi ) );
      }
      if ( r < 0 && rd >= 0 )
        duf_levinfo_goup( pdi );
    }
  }
  DEBUG_ENDR_NOE( r, DUF_ERROR_TOO_DEEP );
}

int
duf_levinfo_godown_dbopenat_dh( duf_depthinfo_t * pdi, int is_leaf, duf_sqlite_stmt_t * pstmt )
{
  return duf_levinfo_godown_dbopenat_dnn_dh( pdi, 0 /* dirid */ , NULL /* itemname */ , 0 /* ndirs */ ,
                                             0 /* nfiles */ , is_leaf, pstmt );
}

int
duf_levinfo_goup( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );

  if ( duf_levinfo_is_leaf( pdi ) )
    DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan leaf  <=             - %s", duf_pdi_depth( pdi ), duf_levinfo_itemshowname( pdi ) );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan node: <=    by %5llu - %s", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ),
               duf_levinfo_itemshowname( pdi ) );
  {
    int d = pdi->depth--;

    if ( duf_levinfo_opened_dh_d( pdi, d ) > 0 )
      DOR( r, duf_levinfo_closeat_dh_d( pdi, d ) );

    assert( pdi->levinfo[d].lev_dh.dfd == 0 );

    DUF_TEST_R( r );
    if ( r < 0 )
      DUF_SHOW_ERROR( "(%d) close error; L%d", r, pdi->depth );
    DUF_TRACE( explain, 2, "level up:   %d", d );
    assert( pdi->levinfo );
    duf_levinfo_clear_level_d( pdi, d );
    d = pdi->depth;
  }
  DEBUG_ENDR( r );
}
