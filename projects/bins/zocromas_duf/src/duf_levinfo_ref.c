#include <string.h>
#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_dh.h"


#include "duf_pdi.h"

#include "duf_levinfo.h"

/* ###################################################################### */
/* #include "duf_levinfo_ref.h" */
/* ###################################################################### */

#define DUF_LEVINFO_F(typ, n) typ duf_levinfo_ ## n(  duf_depthinfo_t * pdi ) \
	{ \
	  typ result = (0); \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  result = duf_levinfo_ ## n ## _d( pdi, pdi->depth ); \
	  return result; \
	}

#define DUF_LEVINFO_F_UP(typ, n) typ duf_levinfo_ ## n ## _up(  duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->depth > 0 ? duf_levinfo_ ## n ## _d( pdi, pdi->depth - 1 ) : 0; \
	}

#define DUF_LEVINFO_FC(typ, n) typ duf_levinfo_ ## n( const duf_depthinfo_t * pdi ) \
	{ \
	  typ result = (0); \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  result = duf_levinfo_ ## n ## _d( pdi, pdi->depth ); \
	  return result; \
	}

#define DUF_LEVINFO_FC_UP(typ, n) typ duf_levinfo_ ## n ## _up( const duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->depth > 0 ? duf_levinfo_ ## n ## _d( pdi, pdi->depth - 1 ) : 0; \
	}


#define DUF_LEVINFO_FS(typ, n) void duf_levinfo_set_ ## n( duf_depthinfo_t * pdi, typ setarg ) \
	{ \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  duf_levinfo_set_ ## n ## _d( pdi, setarg, pdi->depth ); \
	}

#define DUF_LEVINFO_FS_UP(typ, n) void duf_levinfo_set_ ## n ## _up( duf_depthinfo_t * pdi, typ setarg ) \
	{ \
	  assert( pdi ); \
	  if ( pdi->depth > 0 ) \
	    duf_levinfo_set_ ## n ## _d( pdi, setarg, pdi->depth - 1 ); \
	}



/************************************************************************/
duf_levinfo_t *
duf_levinfo_ptr_d( const duf_depthinfo_t * pdi, int d )
{
  return d >= 0 && pdi ? &pdi->levinfo[d] : NULL;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( duf_levinfo_t *, ptr  )
DUF_LEVINFO_FC_UP( duf_levinfo_t *, ptr  )
/* *INDENT-ON*  */

/************************************************************************/


int
duf_levinfo_is_leaf_d( const duf_depthinfo_t * pdi, int d )
{

  assert( pdi );
  assert( pdi->levinfo );
  assert( d >= 0 );
  return pdi->levinfo[d].is_leaf ? 1 : 0;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( int, is_leaf )
DUF_LEVINFO_FC_UP( int, is_leaf )
/* *INDENT-ON*  */

/************************************************************************/


struct stat *
duf_levinfo_stat_d( const duf_depthinfo_t * pdi, int d )
{
  struct stat *pst = NULL;

  assert( pdi );
  /* if ( pdi->opendir ) */
  {
    assert( pdi->levinfo );
    assert( d >= 0 );
    if ( pdi->levinfo[d].lev_dh.rs > 0 )
      pst = &pdi->levinfo[d].lev_dh.st;
  }
  return pst;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( struct stat *, stat )
DUF_LEVINFO_FC_UP( struct stat *, stat )
/* *INDENT-ON*  */

ino_t
duf_levinfo_stat_inode( const duf_depthinfo_t * pdi )
{
  struct stat *st;

  st = duf_levinfo_stat( pdi );
  return st ? st->st_ino : 0;
}

dev_t
duf_levinfo_stat_dev( const duf_depthinfo_t * pdi )
{
  struct stat *st;

  st = duf_levinfo_stat( pdi );
  return st ? st->st_dev : 0;
}

/************************************************************************/

int
duf_levinfo_item_deleted_d( const duf_depthinfo_t * pdi, int d )
{

  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  return d >= 0 ? ( pdi->levinfo[d].deleted ? 1 : 0 ) : 0;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( int, item_deleted )
DUF_LEVINFO_FC_UP( int, item_deleted )
/* *INDENT-ON*  */

/************************************************************************/

const char *
duf_levinfo_itemname_d( const duf_depthinfo_t * pdi, int d )
{
  const char *n = NULL;

  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d <= pdi->maxdepth ); */
  /* assert( d >= 0 ); */
  if ( d >= 0 )
    n = pdi->levinfo[d].itemname;
  return n ? ( *n ? n : "/" ) : "";
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( const char *, itemname )
DUF_LEVINFO_FC_UP( const char *, itemname )
/* *INDENT-ON*  */

const char *
duf_levinfo_itemname_q( const duf_depthinfo_t * pdi, const char *q )
{
  const char *p;

  p = duf_levinfo_itemname( pdi );
  return p ? p : q;
}

/************************************************************************/

/* SET */
void
duf_levinfo_set_items_files_d( duf_depthinfo_t * pdi, unsigned long long n, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  if ( d >= 0 )
    pdi->levinfo[d].items.files = n;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FS( unsigned long long, items_files )
DUF_LEVINFO_FS_UP( unsigned long long, items_files )
/* *INDENT-ON*  */

/************************************************************************/

unsigned long long
duf_levinfo_items_files_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  return d >= 0 ? pdi->levinfo[d].items.files : 0;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( unsigned long long, items_files )
DUF_LEVINFO_FC_UP( unsigned long long, items_files )
/* *INDENT-ON*  */

/************************************************************************/

unsigned long long
duf_levinfo_dirid_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  return d >= 0 ? pdi->levinfo[d].dirid : 0;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( unsigned long long, dirid )
DUF_LEVINFO_FC_UP( unsigned long long, dirid )
/* *INDENT-ON*  */

/************************************************************************/

unsigned long long
duf_levinfo_nodedirid_d( const duf_depthinfo_t * pdi, int d )
{
  unsigned long long nodedirid = 0;

  nodedirid = duf_levinfo_dirid_d( pdi, d - ( duf_levinfo_is_leaf_d( pdi, d ) ? 1 : 0 ) );
  return nodedirid;
}
/* *INDENT-OFF*  */
DUF_LEVINFO_FC( unsigned long long, nodedirid )
DUF_LEVINFO_FC_UP( unsigned long long, nodedirid )
/* *INDENT-ON*  */

/************************************************************************/

void
duf_levinfo_set_dirid_d( duf_depthinfo_t * pdi, unsigned long long dirid, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  if ( d >= 0 )
    pdi->levinfo[d].dirid = dirid;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FS( unsigned long long, dirid )
DUF_LEVINFO_FS_UP( unsigned long long, dirid )
/* *INDENT-ON*  */

/************************************************************************/

int
duf_levinfo_numdir_d( const duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  assert( pdi->levinfo );
  /* assert( d >= 0 ); */
  return d >= 0 ? pdi->levinfo[d].numdir : -1;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( int, numdir )
DUF_LEVINFO_FC_UP( int, numdir )
/* *INDENT-ON*  */

/************************************************************************/



/* SET */

void
duf_levinfo_set_context_d( duf_depthinfo_t * pdi, void *ctx, int d )
{
  assert( pdi );
  {
    assert( d >= 0 );
    assert( pdi->levinfo );
    duf_clear_context( &pdi->levinfo[d].context );
    duf_set_context( &pdi->levinfo[d].context, ctx );
  }
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FS( void *, context )
DUF_LEVINFO_FS_UP( void *, context )
/* *INDENT-ON*  */

/************************************************************************/

void *
duf_levinfo_context_d( const duf_depthinfo_t * pdi, int d )
{
  void *ctx = NULL;

  assert( pdi );

  {
    assert( d >= 0 );
    assert( pdi->levinfo );
    ctx = duf_context( &pdi->levinfo[d].context );
  }
  return ctx;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( void *, context )
DUF_LEVINFO_FC_UP( void *, context )
/* *INDENT-ON*  */

/************************************************************************/

duf_dirhandle_t *
duf_levinfo_pdh_d( const duf_depthinfo_t * pdi, int d )
{
  duf_dirhandle_t *pdhlev = NULL;

  assert( pdi );
  /* if ( pdi->opendir ) */
  {
    /* DUF_SHOW_ERROR( "IS_LEAF:%d", pdi->levinfo[d].is_leaf ); */

    assert( d >= 0 );
    assert( pdi->levinfo );
    pdhlev = &pdi->levinfo[d].lev_dh;
  }
  return pdhlev;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( duf_dirhandle_t *, pdh )
DUF_LEVINFO_FC_UP( duf_dirhandle_t*, pdh )
/* *INDENT-ON*  */

/************************************************************************/

int
duf_levinfo_dfd_d( const duf_depthinfo_t * pdi, int d )
{
  int r = 0;

  assert( pdi );
  /* if ( pdi->opendir ) */
  {
    assert( pdi->levinfo );
    assert( d >= 0 );
    /* duf_dirhandle_t *pdhlev = &pdi->levinfo[d].lev_dh; */

    /* r = pdhlev->dfd; */
    r = pdi->levinfo[d].lev_dh.dfd;
  }
  return r;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( int, dfd )
DUF_LEVINFO_FC_UP( int, dfd )
/* *INDENT-ON*  */

/* int                                                                     */
/* duf_levinfo_dfd( duf_depthinfo_t * pdi )                                */
/* {                                                                       */
/*   assert( pdi );                                                        */
/*   return duf_levinfo_dfd_d( pdi, pdi->depth );                          */
/* }                                                                       */
/*                                                                         */
/* int                                                                     */
/* duf_levinfo_dfd_up( duf_depthinfo_t * pdi )                             */
/* {                                                                       */
/*   assert( pdi );                                                        */
/*   return pdi->depth > 0 ? duf_levinfo_dfd_d( pdi, pdi->depth - 1 ) : 0; */
/* }                                                                       */

/************************************************************************/

int
duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  {
    int r = 0;

    assert( d >= 0 );
    assert( pdi->levinfo );
    if ( duf_levinfo_dfd_d( pdi, d ) )
      r = duf_close_dh( &pdi->levinfo[d].lev_dh );
    assert( duf_levinfo_dfd_d( pdi, d ) == 0 );

    DUF_TEST_R( r );
    return r;
  }
}

/* *INDENT-OFF*  */
DUF_LEVINFO_F( int, closeat_dh )
DUF_LEVINFO_F_UP( int, closeat_dh )
/* *INDENT-ON*  */

/************************************************************************/

const char *
duf_levinfo_path_d( const duf_depthinfo_t * pdi, int d )
{
  char *path = NULL;

  assert( pdi );
  {
    assert( d >= 0 );
    assert( pdi->levinfo );
    if ( pdi->levinfo[d].is_leaf )
      d--;
    {
      assert( d >= 0 );
      if ( pdi->levinfo[d].fullpath )
      {
        path = pdi->levinfo[d].fullpath;
      }
      else
        /* if ( pdi->path ) */
      {
        /* size_t lenp = strlen( pdi->path ); */
        size_t len = 2;
        char *p;

        for ( int i = 0; i <= d; i++ )
        {
          assert( pdi->levinfo[i].itemname );
          len += strlen( pdi->levinfo[i].itemname ) + 1;
        }
        path = mas_malloc( len );
        /* path = strcpy( path, pdi->path ); */
        p = path;
        for ( int i = 0; i <= d; i++ )
        {
          size_t l;

          if ( p == path || *( p - 1 ) != '/' )
            *p++ = '/';
          *p = 0;
          l = strlen( pdi->levinfo[i].itemname );
          if ( l > 0 )
          {
            strcpy( p, pdi->levinfo[i].itemname );
            p += l;
            /* *p++ = '/'; */
          }
          *p = 0;
        }
        assert( d >= 0 );
        pdi->levinfo[d].fullpath = path;
      }
      /* else                                */
      /* {                                   */
      /*   DUF_SHOW_ERROR( "pdi->path not set" ); */
      /* }                                   */
    }
  }
  if ( !path )
    DUF_SHOW_ERROR( "no path returned" );
  return path;
}

/* *INDENT-OFF*  */
DUF_LEVINFO_FC( const char *, path )
DUF_LEVINFO_FC_UP( const char *, path )
/* *INDENT-ON*  */

const char *
duf_levinfo_path_q( const duf_depthinfo_t * pdi, const char *q )
{
  const char *p;

  p = duf_levinfo_path( pdi );
  return p ? p : q;
}

/************************************************************************/
