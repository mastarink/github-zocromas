#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"


/* ###################################################################### */
#include "duf_print.h"
/* ###################################################################### */


/***
   %h : depth
   %Q : seq
   %q : seq_node
   %# : seq_leaf
   %M : md5id
   %S : nsame
   %P : prefix
   %I : dirid
   %F : nfiles
   %D : ndirs
   %A : dataid
   %~ : suffix
   %O : inode
   %m : mode
   %n : nlink
   %u : user
   %g : group
   %z : filesize
   %t : mtime
   %r : realpath
   %R : relative realpath (relative to 'top level')
   %f : filename
   %@ : md5sum
   %X : exifid
   %T : exif datetime
   %E : mimeid
   %e : mime
   %N : nameid
   %p : move to position
   %s : << space >>

***/
static int
duf_sformat_id( const char **pfmt, char **ppbuffer, size_t position, size_t bfsz, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi,
                duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb )
{
  int ok = 0;
  char c;
  char *pe = NULL;
  long v = 0;
  size_t fbsz = 512;
  const char *fmt = *pfmt;
  char *pbuffer = *ppbuffer;
  char format[fbsz];

  v = strtol( fmt, &pe, 10 );
  fmt = pe;
  c = *fmt++;
  switch ( c )
  {
  case 'h':                    /* depth */
    if ( v )
      snprintf( format, fbsz, "%%%ldu", v );
    else
      snprintf( format, fbsz, "%%u" );
    snprintf( pbuffer, bfsz, format, duf_pdi_reldepth( pdi ) );
    ok++;
    break;
  case 'Q':                    /* seq */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq );
    ok++;
    break;
  case 'q':                    /* seq_node */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq_node );
    ok++;
    break;
  case '#':                    /* seq_leaf */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq_leaf );
    ok++;
    break;
  case 'M':                    /* md5id */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->md5id );
    ok++;
    break;
  case 'S':                    /* nsame */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->nsame );
    ok++;
    break;
  case '_':                    /* space */
    memset( pbuffer, '_', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    ok++;
    break;
  case 's':                    /* space */
    /* pbuffer += strlen( pbuffer ); */
    memset( pbuffer, ' ', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    ok++;
    break;
  case 'p':
    {
      long vp = 0;

      vp = v - position;
      memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? -vp : 1 ) );
      ok++;
    }
    break;
  case 'P':                    /* prefix */
    if ( prefix_scb )
    {
      ( prefix_scb ) ( pbuffer, bfsz, pdi );
      ok++;
    }
    break;
  case 'I':                    /* dirid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, duf_levinfo_nodedirid( pdi ) );
    ok++;
    break;
  case 'L':                    /* nfiles */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.files );
    ok++;
    break;
  case 'D':                    /* ndirs */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.dirs );
    ok++;
    break;
  case 'A':                    /* dataid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->dataid );
    ok++;
    break;
  case '~':                    /* suffix */
    break;
  case 'O':                    /* inode */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_ino );
    ok++;
    break;
  case 'm':                    /* mode */
    {
      char modebuf[] = "----------";
      char *pmode = modebuf;

      pmode++;
      if ( S_IRUSR & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWUSR & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXUSR & pfi->st.st_mode )
        *pmode = 'x';
      pmode++;
      if ( S_IRGRP & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWGRP & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXGRP & pfi->st.st_mode )
        *pmode = 'x';
      pmode++;
      if ( S_IROTH & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWOTH & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXOTH & pfi->st.st_mode )
        *pmode = 'x';
      snprintf( pbuffer, bfsz, "%s", modebuf );
      ok++;
    }
    break;
  case 'n':                    /* nlink */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_nlink );
    ok++;
    break;
  case 'u':                    /* user */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_uid );
    ok++;
    break;
  case 'g':                    /* group */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_gid );
    ok++;
    break;
  case 'Z':                    /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

      if ( v )
        snprintf( format, fbsz, "%%%ldllu.", v );
      else
        snprintf( format, fbsz, "%%llu." );

      snprintf( pbuffer, bfsz, format, sz );
      ok++;
    }
    break;
  case 'z':                    /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

      if ( sz < 1024 )
      {
        if ( v )
          snprintf( format, fbsz, "%%%ldllu.", v );
        else
          snprintf( format, fbsz, "%%llu." );
        snprintf( pbuffer, bfsz, format, sz );
        ok++;
      }
      else if ( sz < 1024 * 1024 )
      {
        if ( v )
          snprintf( format, fbsz, "%%%ldlluk", v );
        else
          snprintf( format, fbsz, "%%lluk" );
        snprintf( pbuffer, bfsz, format, sz / 1024 );
        ok++;
      }
      else if ( sz < 1024 * 1024 * 1024 )
      {
        if ( v )
          snprintf( format, fbsz, "%%%ldlluM", v );
        else
          snprintf( format, fbsz, "%%lluM" );
        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 );
        ok++;
      }
      else
      {
        if ( v )
          snprintf( format, fbsz, "%%%ldlluG", v );
        else
          snprintf( format, fbsz, "%%lluG" );
        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 / 1024 );
        ok++;
      }
    }
    break;
  case 't':                    /* mtime */
    {
      time_t mtimet;
      struct tm mtimetm, *pmtimetm;
      char mtimes[128];

      mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
      pmtimetm = localtime_r( &mtimet, &mtimetm );
      strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
      snprintf( pbuffer, bfsz, format, mtimes );
      ok++;
    }
    break;
  case 'r':                    /* realpath */
    {
      const char *real_path = NULL;

      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      real_path = duf_levinfo_path( pdi );
      snprintf( pbuffer, bfsz, format, real_path );
      ok++;
    }
    break;
  case 'R':                    /* relative realpath */
    {
      const char *rel_real_path = NULL;

      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      rel_real_path = duf_levinfo_relpath( pdi );
      snprintf( pbuffer, bfsz, format, rel_real_path );
      ok++;
    }
    break;    
  case 'f':                    /* filename */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, pfi->name );
      ok++;
    }
    pbuffer += strlen( pbuffer );
    break;
#if 0
  case 'F':                    /* itemname */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, duf_levinfo_itemname( pdi ) );
      ok++;
    }
    pbuffer += strlen( pbuffer );
    break;
#endif
  case '@':                    /* md5sum */
    if ( pfi->md5sum1 || pfi->md5sum2 )
      snprintf( pbuffer, bfsz, "%016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
    else
      snprintf( pbuffer, bfsz, "%-32s", "-" );
    ok++;
    break;
  case 'X':                    /* exifid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->exifid );
    ok++;
    break;
  case 'T':                    /* exif datatime */
    {
      time_t xtimet;
      struct tm xtimetm, *pxtimetm;
      char xtimes[128];

      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      if ( pfi->exifid )
      {
        xtimet = ( time_t ) pfi->exifdt;
        pxtimetm = localtime_r( &xtimet, &xtimetm );
        strftime( xtimes, sizeof( xtimes ), "%b %d %Y %H:%M:%S", pxtimetm );
        snprintf( pbuffer, bfsz, format, xtimes );
      }
      else
      {
        strcpy( pbuffer, "___ __ ____ __:__:__" );
      }
      ok++;
    }
    break;
  case 'E':                    /* mimeid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->mimeid );
    ok++;
    break;
  case 'e':                    /* mime */
    if ( v )
      snprintf( format, fbsz, "%%%lds", v );
    else
      snprintf( format, fbsz, "%%s" );
    snprintf( pbuffer, bfsz, format, pfi->mime );
    ok++;
    break;
  case 'N':                    /* nameid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->nameid );
    ok++;
    break;
  default:
    pbuffer += strlen( pbuffer );
    *pbuffer++ = '%';
    if ( v )
      snprintf( pbuffer, bfsz, "%ld", v );
    pbuffer += strlen( pbuffer );
    *pbuffer++ = c;
    *pbuffer = 0;
    break;
  }
  pbuffer += strlen( pbuffer );
  *pfmt = fmt;
  *ppbuffer = pbuffer;
  return ok;
}

int
duf_print_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb )
{
  int ok = 0;
  const char *fmt = format;
  char *buffer = NULL;
  char *pbuffer = NULL;
  size_t bfsz = 1024 * 16;

  pbuffer = buffer = mas_malloc( bfsz + 8 );
  memset( buffer, 0, bfsz );
  while ( fmt && *fmt && buffer + bfsz - pbuffer > 0 )
  {
    if ( *fmt == '%' )
    {
      fmt++;
      ok = duf_sformat_id( &fmt, &pbuffer, pbuffer - buffer, buffer + bfsz - pbuffer /* remaining bytes cnt */ , pdi, pfi, prefix_scb, suffix_scb );
    }
    else if ( *fmt == '\\' )
    {
      fmt++;
      if ( *fmt == 'n' )
        *pbuffer++ = '\n';
      else if ( *fmt == 'r' )
        *pbuffer++ = '\r';
      else if ( *fmt == 't' )
        *pbuffer++ = '\t';
      else
      {
        *pbuffer++ = '\\';
        *pbuffer++ = *fmt;
      }
      fmt++;
    }
    else
    {
      *pbuffer++ = *fmt++;
    }
  }
  DUF_WRITES( 0, buffer );
  mas_free( buffer );
  return ok;
}
