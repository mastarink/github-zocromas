#include <string.h>

/* #include <mastar/tools/mas_utils_path.h> */

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"



#include "duf_dh.h"


#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_ref.h"
/* ###################################################################### */

/* 20150904.122230 */
duf_levinfo_t *
duf_pi_ptr_d( const duf_pathinfo_t * pi, int d )
{
  assert( pi );
  assert( d >= 0 );
  assert( pi->levinfo );

  return pi ? &pi->levinfo[d] : NULL;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( duf_levinfo_t , ptr )
DUF_PATHINFO_FC_UP_REF( duf_levinfo_t , ptr )
/* *INDENT-ON*  */

/* 20150904.122234 */
const char *
duf_pi_path_d( const duf_pathinfo_t * pi, int d )
{
  char *path = NULL;
  duf_levinfo_t *pli;

  pli = duf_pi_ptr_d( pi, d );
  assert( pli );

#if 0
  if ( pli->fullpath )
  {
    path = pli->fullpath;
  }
  else
  {
    {
      size_t len = 2;
      char *p;

      assert( pi );
      assert( d >= 0 );
      for ( int i = 0; i <= d; i++ )
      {
        assert( &pli[i - d] == &pi->levinfo[i] );
        assert( pi->levinfo[i].itemname );
        len += strlen( pi->levinfo[i].itemname ) + 1;
      }
      path = mas_malloc( len );
      p = path;

      for ( int i = 0; i <= d; i++ )
      {
        size_t l;

        if ( p == path || *( p - 1 ) != '/' )
          *p++ = '/';
        *p = 0;
        DUF_TRACE( path, 4, "path:%s", path );
        l = strlen( pi->levinfo[i].itemname );
        if ( l > 0 )
        {
          strcpy( p, pi->levinfo[i].itemname );
          p += l;
          *p++ = '/';
        }
        *p = 0;
      }
    }
    assert( d >= 0 );
    pli->fullpath = path;
  }
#else
  if ( !pli->fullpath )
    pli->fullpath = duf_li_path( duf_pi_ptr_d( pi, 0 ), d + 1 );
#endif
  DUF_TRACE( path, 4, "fullpath:%s", path );
  return pli->fullpath;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, path )
DUF_PATHINFO_FC_UP( const char *, path )
DUF_PATHINFO_FC_TOP( const char *, path )
/* *INDENT-ON*  */


/* 20150904.122510 */
const char *
duf_pi_relpath_d( const duf_pathinfo_t * pi, int d )
{
  const char *toppath;
  const char *path;
  size_t len;

  toppath = duf_pi_path_top( pi );
  len = strlen( toppath );
  path = duf_pi_path_d( pi, d );
  return path + len;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, relpath )
DUF_PATHINFO_FC_UP( const char *, relpath )
/* *INDENT-ON*  */

/* 20150904.122152 */
const char *
duf_pi_path_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_path( pi );
  return p ? p : q;
}

/* 20150904.122155 */
char *
duf_pi_path_qdup( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_path_q( pi, q );
  return p ? mas_strdup( p ) : NULL;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_4GET( int, dfd, lev_dh.dfd )
/* *INDENT-ON*  */



/* 20150904.122200 */
const char *
duf_pi_itemshowname_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  n = duf_pi_itemtruename_d( pi, d );
  /* return n ? ( *n ? n : "/" ) : n; */
  return n && !*n ? "/" : n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemshowname )
DUF_PATHINFO_FC_UP_REF( const char, itemshowname )
/* *INDENT-ON*  */

/* 20150904.122206 */
const char *
duf_pi_itemshowname_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_itemshowname( pi );
  return p ? p : q;
}

/* 20150904.122209 */
const char *
duf_pi_itemtruename_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  duf_levinfo_t *pli;

  pli = duf_pi_ptr_d( pi, d );

  n = pli ? pli->itemname : NULL;
  /* return n ? ( *n ? n : "/" ) : n; */
  return n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemtruename )
DUF_PATHINFO_FC_UP_REF( const char, itemtruename )
/* *INDENT-ON*  */

/* 20150904.122217 */
const char *
duf_pi_itemtruename_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pi_itemtruename( pi );
  return p ? p : q;
}

/************************************************************************/
/* 20150904.122223 */
struct stat *
duf_pi_stat_d( const duf_pathinfo_t * pi, int d )
{
  struct stat *pst = NULL;

  /* if ( pdi->opendir ) */
  {
    duf_dirhandle_t *pdh;

    pdh = duf_pi_pdh_d( pi, d );

    if ( pdh->rs > 0 && pdh->source == DUF_DH_SOURCE_FS )
      pst = &pdh->st;
  }
  /* TODO NOT here: assert( pst->st_dev ); 
   * (used to check presence, so no stat is OK)
   * TODO */
  return pst;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( struct stat, stat )
DUF_PATHINFO_FC_UP_REF( struct stat, stat )
/* *INDENT-ON*  */

/* *INDENT-OFF*  */
DUF_PATHINFO_3GET_REF( duf_dirhandle_t, pdh, lev_dh )
/* *INDENT-ON*  */

DUF_PATHINFO_ST_FLD_NAME( ino, inode );
DUF_PATHINFO_ST_FLD( dev );
DUF_PATHINFO_ST_FLD( mode );
DUF_PATHINFO_ST_FLD( nlink );
DUF_PATHINFO_ST_FLD( uid );
DUF_PATHINFO_ST_FLD( gid );
DUF_PATHINFO_ST_TYP_FLD( blkcnt, blocks );
DUF_PATHINFO_ST_FLD( blksize );
DUF_PATHINFO_ST_TYP_FLD( off, size );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, atime, asec );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, mtime, msec );
DUF_PATHINFO_ST_TYP_FLD_NAME( time, ctime, csec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, atim.tv_nsec, ansec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, mtim.tv_nsec, mnsec );
DUF_PATHINFO_ST_TYP_FLD_NAME( long, ctim.tv_nsec, cnsec );

/************************************************************************/
void
duf_pi_set_topdepth( duf_pathinfo_t * pi )
{
  if ( pi )
    pi->topdepth = pi->depth;
}

int
duf_pi_topdepth( const duf_pathinfo_t * pi )
{
  return pi ? pi->topdepth : 0;
}

int
duf_pi_maxdepth( const duf_pathinfo_t * pi )
{
  return pi ? pi->maxdepth : 0;
}

duf_levinfo_t *
duf_pi_levinfo( const duf_pathinfo_t * pi )
{
  return pi ? pi->levinfo : NULL;
}

int
duf_pi_depth( const duf_pathinfo_t * pi )
{
  return pi ? pi->depth : 0;
}

int
duf_pi_deltadepth_d( const duf_pathinfo_t * pi, int d )
{
  return pi ? d - duf_pi_topdepth( pi ) : 0;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( int, deltadepth )
DUF_PATHINFO_FC_UP( int, deltadepth )
/* *INDENT-ON*  */
