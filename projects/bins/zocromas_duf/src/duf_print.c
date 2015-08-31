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

void
duf_convert_fmt( char *format, size_t fbsz, const char *fmt, const char *tail )
{
  if ( fbsz > 0 )
  {
    *format++ = '%';
    fbsz--;
  }
  DUF_TRACE( temp, 5, ">%ld>> '%s' (%c:%d:%d) :: '%s'", fbsz, fmt, *fmt, ( ( *fmt == '+' ) || ( *fmt == '-' ) ) ? 1 : 0, *fmt - '-', tail );
  if ( fbsz > 0 && ( *fmt == '+' || *fmt == '-' ) )
  {
    DUF_TRACE( temp, 5, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  while ( fbsz > 0 && *fmt >= '0' && *fmt <= '9' )
  {
    DUF_TRACE( temp, 5, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  strncpy( format, tail, fbsz );
}

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
   %tm : mtime
   %ta : mtime
   %tc : mtime
   %tx : exif datetime
   %r : realpath
   %R : relative realpath (relative to 'top level')
   %f : filename
   %@ : md5sum
   %X : exifid
   %E : mimeid
   %e : mime
   %N : nameid
   %a : camera
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
  const char *fmt0;

  fmt0 = fmt;
  v = strtol( fmt, &pe, 10 );
  fmt = pe;
  c = *fmt++;
  switch ( c )
  {
  case 'h':                    /* depth */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "u" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldu", v );
    else
      snprintf( format, fbsz, "%%u" );
#endif

    snprintf( pbuffer, bfsz, format, duf_pdi_reldepth( pdi ) );
    ok++;
    break;
  case 'Q':                    /* seq */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq );
    ok++;
    break;
  case 'q':                    /* seq_node */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq_node );
    ok++;
    break;
  case '#':                    /* seq_leaf */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq_leaf );
    ok++;
    break;
  case 'M':                    /* md5id */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    snprintf( pbuffer, bfsz, format, pfi->md5id );
    ok++;
    break;
  case 'S':                    /* nsame */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

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
      /* memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? -vp : 1 ) ); */
      memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? 1 : 1 ) );
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
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, duf_levinfo_nodedirid( pdi ) );
    ok++;
    break;
  case 'L':                    /* nfiles */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.files );
    ok++;
    break;
  case 'D':                    /* ndirs */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.dirs );
    ok++;
    break;
  case 'A':                    /* dataid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->dataid );
    ok++;
    break;
  case '~':                    /* suffix */
    break;
  case 'O':                    /* inode */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

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
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_nlink );
    ok++;
    break;
  case 'u':                    /* user */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_uid );
    ok++;
    break;
  case 'g':                    /* group */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_gid );
    ok++;
    break;
  case 'Z':                    /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "llu." );
#else
      if ( v )
        snprintf( format, fbsz, "%%%ldllu.", v );
      else
        snprintf( format, fbsz, "%%llu." );
#endif


      snprintf( pbuffer, bfsz, format, sz );
      ok++;
    }
    break;
  case 'z':                    /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

      if ( sz < 1024 )
      {
#if 1
        duf_convert_fmt( format, fbsz, fmt0, "llu." );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldllu.", v );
        else
          snprintf( format, fbsz, "%%llu." );
#endif

        snprintf( pbuffer, bfsz, format, sz );
        ok++;
      }
      else if ( sz < 1024 * 1024 )
      {
#if 1
        duf_convert_fmt( format, fbsz, fmt0, "lluk" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluk", v );
        else
          snprintf( format, fbsz, "%%lluk" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 );
        ok++;
      }
      else if ( sz < 1024 * 1024 * 1024 )
      {
#if 1
        duf_convert_fmt( format, fbsz, fmt0, "lluM" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluM", v );
        else
          snprintf( format, fbsz, "%%lluM" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 );
        ok++;
      }
      else
      {
#if 1
        duf_convert_fmt( format, fbsz, fmt0, "lluG" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluG", v );
        else
          snprintf( format, fbsz, "%%lluG" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 / 1024 );
        ok++;
      }
    }
    break;
  case 'T':                    /* time */
  case 't':                    /* time */
    {
      time_t timet;
      struct tm time_tm, *ptime_tm;
      char stime[128];
      char c2 = 0;

      timet = ( time_t ) 0;
      c2 = *fmt++;
      switch ( c2 )
      {
      case 'm':
        timet = ( time_t ) pfi->st.st_mtim.tv_sec;
        break;
      case 'a':
        timet = ( time_t ) pfi->st.st_atim.tv_sec;
        break;
      case 'c':
        timet = ( time_t ) pfi->st.st_atim.tv_sec;
        break;
      case 'x':
        if ( pfi->exifid )
          timet = ( time_t ) pfi->exifdt;
        break;
      default:
        timet = ( time_t ) pfi->st.st_mtim.tv_sec;
        fmt--;
      }
      ptime_tm = localtime_r( &timet, &time_tm );
      switch ( c )
      {
      case 't':
        strftime( stime, sizeof( stime ), "%b %d %Y %H:%M:%S", ptime_tm );
        break;
      case 'T':
        {
          char *f = NULL;

          if ( *fmt == '{' )
          {
            const char *efmt;

            fmt++;
            efmt = strchr( fmt, '}' );
            if ( efmt )
            {
              f = mas_strndup( fmt, efmt - fmt );
              /* DUF_TRACE( temp, 0, "=== '%s'", f ); */
              fmt = efmt;
              fmt++;
            }
          }
          if ( !f )
            f = mas_strdup( "%Y/%B/%d" );
          if ( f )
            strftime( stime, sizeof( stime ), f, ptime_tm );
          mas_free( f );
        }
        break;
      }
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, stime );
      ok++;
    }
    break;
  case 'r':                    /* realpath */
    {
      const char *real_path = NULL;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      real_path = duf_levinfo_path( pdi );
      snprintf( pbuffer, bfsz, format, real_path );
      ok++;
    }
    break;
  case 'R':                    /* relative realpath */
    {
      const char *rel_real_path = NULL;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif


      rel_real_path = duf_levinfo_relpath( pdi );
      snprintf( pbuffer, bfsz, format, rel_real_path );
      ok++;
    }
    break;
  case 'f':                    /* filename */
    {
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif


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
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->exifid );
    ok++;
    break;
  case 'a':                    /* camera */
    if ( pfi->camera )
    {
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, pfi->camera );
    }
    ok++;
    break;
  case 'E':                    /* mimeid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->mimeid );
    ok++;
    break;
  case 'e':                    /* mime */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%lds", v );
    else
      snprintf( format, fbsz, "%%s" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->mime );
    ok++;
    break;
  case 'N':                    /* nameid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

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

char *
duf_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb, int *pr )
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
  if ( pr )
    *pr = ok;
  return buffer;
}

int
duf_print_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb )
{
  int ok = 0;
  char *buffer;

  buffer = duf_sformat_file_info( pdi, pfi, format, prefix_scb, suffix_scb, &ok );
  DUF_WRITES( 0, buffer );
  mas_free( buffer );
  return ok;
}
