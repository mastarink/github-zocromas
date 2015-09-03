#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_context.h"

#include "duf_dh.h"


#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_ref.h"
/* ###################################################################### */


duf_levinfo_t *
duf_pathinfo_ptr_d( const duf_pathinfo_t * pi, int d )
{
  assert( d >= 0 );
  assert( pi->levinfo );
  return d >= 0 && pi ? &pi->levinfo[d] : NULL;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( duf_levinfo_t , ptr )
DUF_PATHINFO_FC_UP_REF( duf_levinfo_t , ptr )
/* *INDENT-ON*  */


const char *
duf_pathinfo_path_d( const duf_pathinfo_t * pi, int d )
{
  char *path = NULL;

  assert( d >= 0 );
  if ( duf_pathinfo_ptr_d( pi, d )->fullpath )
  {
    path = duf_pathinfo_ptr_d( pi, d )->fullpath;
  }
  else
  {
    size_t len = 2;
    char *p;

    for ( int i = 0; i <= d; i++ )
    {
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
    assert( d >= 0 );
    duf_pathinfo_ptr_d( pi, d )->fullpath = path;
    DUF_TRACE( path, 4, "fullpath:%s", path );
  }
  return path;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, path )
DUF_PATHINFO_FC_UP( const char *, path )
DUF_PATHINFO_FC_TOP( const char *, path )
/* *INDENT-ON*  */



const char *
duf_pathinfo_relpath_d( const duf_pathinfo_t * pi, int d )
{
  const char *toppath;
  const char *path;
  size_t len;

  toppath = duf_pathinfo_path_top( pi );
  len = strlen( toppath );
  path = duf_pathinfo_path_d( pi, d );
  return path + len;
}
/* *INDENT-OFF*  */
DUF_PATHINFO_FC( const char *, relpath )
DUF_PATHINFO_FC_UP( const char *, relpath )
/* *INDENT-ON*  */

const char *
duf_pathinfo_path_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pathinfo_path( pi );
  return p ? p : q;
}

char *
duf_pathinfo_path_qdup( const duf_pathinfo_t * pi, const char *q )
{
  char *path = NULL;
  const char *p;

  p = duf_pathinfo_path_q( pi, q );
  if ( p )
    path = mas_strdup( p );
  return path;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_4GET( int, dfd, lev_dh.dfd )
/* *INDENT-ON*  */




const char *
duf_pathinfo_itemshowname_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  n = duf_pathinfo_ptr_d( pi, d )->itemname;
  /* return n ? ( *n ? n : "/" ) : n; */
  return n && !*n ? "/" : n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemshowname )
DUF_PATHINFO_FC_UP_REF( const char, itemshowname )
/* *INDENT-ON*  */

const char *
duf_pathinfo_itemshowname_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pathinfo_itemshowname( pi );
  return p ? p : q;
}

const char *
duf_pathinfo_itemtruename_d( const duf_pathinfo_t * pi, int d )
{
  const char *n = NULL;

  n = duf_pathinfo_ptr_d( pi, d )->itemname;
  /* return n ? ( *n ? n : "/" ) : n; */
  return n;
}

/* *INDENT-OFF*  */
DUF_PATHINFO_FC_REF( const char, itemtruename )
DUF_PATHINFO_FC_UP_REF( const char, itemtruename )
/* *INDENT-ON*  */

const char *
duf_pathinfo_itemtruename_q( const duf_pathinfo_t * pi, const char *q )
{
  const char *p;

  p = duf_pathinfo_itemtruename( pi );
  return p ? p : q;
}

/************************************************************************/
struct stat *
duf_pathinfo_stat_d( const duf_pathinfo_t * pi, int d )
{
  struct stat *pst = NULL;

  /* if ( pdi->opendir ) */
  {
    if ( duf_pathinfo_ptr_d( pi, d )->lev_dh.rs > 0 && duf_pathinfo_ptr_d( pi, d )->lev_dh.source == DUF_DH_SOURCE_FS )
      pst = &duf_pathinfo_ptr_d( pi, d )->lev_dh.st;
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
