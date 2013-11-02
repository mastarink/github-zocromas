#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-byte-order.h>
#include <libexif/exif-data-type.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-log.h>
#include <libexif/exif-tag.h>
#include <libexif/exif-content.h>
#include <libexif/exif-mnote-data.h>
#include <libexif/exif-mem.h>
#include <libexif/exif-log.h>
#include <libexif/exif-mem.h>



#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_path.h"
#include "duf_utils.h"


#include "duf_exif.h"

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *                int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
 * */
static int
duf_sql_copy_jpeg_by_date( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  char *fname;
  char *path = NULL;
  unsigned long long pathid;

  /* sqlite3_int64 dataid; */
  char *datetime;

  /* fprintf( stderr, ">>>>>> %10s %15s %5s %5s [%s]\n", presult[0], presult[ 1], presult[ 2], presult[ 3], */
  /*          presult[ 4] );                                                                                         */

  /* dataid = */ ( void ) strtoll( presult[0], NULL, 10 );
  pathid = strtoll( presult[3], NULL, 10 );
  path = duf_pathid_to_path( pathid );
  datetime = presult[4];
  fname = presult[5];
  if ( fname && path )
  {
    char *base = "/mnt/new_media/media/years";
    char *fpath;
    char *new_path;
    char *new_fpath;

    new_path = mas_strdup( base );
    {
      struct stat stf;

      if ( 0 != stat( new_path, &stf ) && errno == ENOENT )
      {
        fprintf( stderr, "%s\n", new_path );
        mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );
      }
    }
    new_path = mas_strcat_x( new_path, "/" );
    new_path = mas_strncat_x( new_path, datetime, 4 );
    /* fprintf( stderr, "%s: %s\n new_path => %s\n", datetime, fpath, new_path ); */
    {
      struct stat stf;

      if ( 0 != stat( new_path, &stf ) && errno == ENOENT )
      {
        fprintf( stderr, "%s\n", new_path );
        mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );
      }
    }
    new_path = mas_strcat_x( new_path, "/" );
    new_path = mas_strncat_x( new_path, datetime + 5, 2 );
    {
      struct stat stf;

      if ( 0 != stat( new_path, &stf ) && errno == ENOENT )
      {
        fprintf( stderr, "%s\n", new_path );
        mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );
      }
    }
    new_path = mas_strcat_x( new_path, "/" );
    new_fpath = mas_strdup( new_path );
    new_fpath = mas_strncat_x( new_fpath, datetime, 4 );
    new_fpath = mas_strncat_x( new_fpath, datetime + 5, 2 );
    new_fpath = mas_strncat_x( new_fpath, datetime + 8, 2 );
    new_fpath = mas_strcat_x( new_fpath, "-" );
    new_fpath = mas_strncat_x( new_fpath, datetime + 11, 2 );
    new_fpath = mas_strncat_x( new_fpath, datetime + 14, 2 );
    new_fpath = mas_strncat_x( new_fpath, datetime + 17, 2 );
    /* new_fpath = mas_strcat_x( new_fpath, ".jpg" ); */
    new_fpath = mas_strcat_x( new_fpath, "." );
    new_fpath = mas_strcat_x( new_fpath, fname );
    fpath = duf_join_path( path, fname );
    fprintf( stderr, "%u. %s\n  => %s\n", nrow, fpath, new_fpath );
    mas_free( new_fpath );
    mas_free( new_path );
    mas_free( fpath );
  }
  mas_free( path );
  return 0;
}

int
copy_jpeg_by_date( void )
{
  int r = 0;

  r = duf_sql_select( duf_sql_copy_jpeg_by_date, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, 0,
                      "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, "
                      " duf_filenames.pathid, duf_exif.datetime, duf_filenames.name " " FROM duf_filedatas "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_exif ON (duf_exif.dataid=duf_filedatas.id) " " WHERE duf_exif.datetime IS NOT NULL "
                      " AND ( duf_filenames.name LIKE '%%.JPG' OR duf_filenames.name LIKE '%%.JPEG' ) " " ORDER BY duf_exif.datetime" );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *          int fun( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
 * */
static int
duf_sql_update_exif( int nrow, int nrows, char *presult[], va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
{
  int r = 0;
  char *fname;
  char *path = NULL;
  unsigned long long pathid;
  unsigned long long dataid;

  /* fprintf( stderr, ">>>>>> %10s %15s %5s %5s [%s]\n", presult[0], presult[ 1], presult[ 2], presult[ 3], */
  /*          presult[ 4] );                                                                                         */

  dataid = strtoll( presult[0], NULL, 10 );
  pathid = strtoll( presult[3], NULL, 10 );
  path = duf_pathid_to_path( pathid );
  fname = presult[4];
  if ( fname && path )
  {
    char *fpath;

    fpath = duf_join_path( path, fname );
    /* fprintf( stderr, "%s\x1b[K\r", fpath ); */

    /* fprintf( stderr, "fname:[%s]\n", fpath ); */
    {
      ExifData *ed;

      /* ExifIfd ifd = EXIF_IFD_0;                    */
      ExifTag tag1 = EXIF_TAG_DATE_TIME;
      ExifTag tag2 = EXIF_TAG_DATE_TIME_DIGITIZED;
      ExifTag tag3 = EXIF_TAG_DATE_TIME_ORIGINAL;
      ExifEntry *entry;

      ed = exif_data_new_from_file( fpath );
      if ( ed )
      {
        if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], tag3 ) )
/*                   || ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], tag2 ) ) */
             || ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], tag1 ) ) )
        {
          char buf[1024];
          char *pq;
          int changed = 0;

          /* char buf1[1024]; */
          struct tm times;

          /* Get the contents of the tag in human-readable form */
          exif_entry_get_value( entry, buf, sizeof( buf ) );
          /* 2008:06:21 13:18:19 */
          while ( ( pq = strchr( buf, '?' ) ) )
          {
            int n;

            fprintf( stderr, "!!!! %d [%s] (%s)\n", n, pq, buf );
            n = pq - buf;
            if ( n < 4 )
            {
              if ( n > 1 )
                *pq = '9';
            }
            else if ( n == 5 || n == 6 )
            {
              buf[5] = '1';
              buf[6] = '2';
            }
            else if ( n == 8 || n == 9 )
            {
              buf[8] = '3';
              buf[9] = '1';
            }
            else if ( n == 11 || n == 12 || n == 14 || n == 15 || n == 17 || n == 18 )
            {
              buf[n] = '0';
            }
            changed = 1;
          }
          {
            strptime( buf, "%Y:%m:%d %H:%M:%S", &times );

            /* strftime( buf1, sizeof( buf1 ), "%Y-%m-%d %H:%M:%S", &times ); */
            /* fprintf( stderr, ">>>>> %lu : %lu\n", t, time(NULL) ); */
            /* Don't bother printing it if it's entirely blank */
            /* trim_spaces( buf ); */
            if ( *buf )
            {
              time_t t;

              t = mktime( &times );
              r = duf_sql
                    ( "INSERT INTO duf_exif (dataid, datetime, d, broken_date) values ( '%llu', datetime('%lu', 'unixepoch'), '%lu', '%u' )",
                      dataid, t, t, changed );
            }
          }
        }
        else if ( 0 )
        {
          if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], tag3 ) ) )
            fprintf( stderr, "NO DATE - HAS +1\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], tag3 ) ) )
            fprintf( stderr, "NO DATE - HAS +2\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], tag3 ) ) )
            fprintf( stderr, "NO DATE - HAS +3\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], tag2 ) ) )
            fprintf( stderr, "NO DATE - HAS +4\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], tag2 ) ) )
            fprintf( stderr, "NO DATE - HAS +5\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], tag2 ) ) )
            fprintf( stderr, "NO DATE - HAS +6\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], tag1 ) ) )
            fprintf( stderr, "NO DATE - HAS +7\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], tag1 ) ) )
            fprintf( stderr, "NO DATE - HAS +8\n" );
          else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], tag1 ) ) )
            fprintf( stderr, "NO DATE - HAS +9\n" );
          else
            fprintf( stderr, "NO DATE : %s\n", fpath );
        }
      }
    }
    mas_free( fpath );
  }
  mas_free( path );
  return r;
}

int
duf_update_exif( void )
{
  int r = 0;

  r = duf_sql_select( duf_sql_update_exif, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, 0,
                      "SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filenames.pathid, duf_filenames.name "
                      " FROM duf_filedatas " " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_exif ON (duf_exif.dataid=duf_filedatas.id) " " WHERE duf_exif.datetime IS NULL "
                      " AND ( duf_filenames.name LIKE '%%.JPG' OR duf_filenames.name LIKE '%%.JPEG' ) " );
  return r;
}
