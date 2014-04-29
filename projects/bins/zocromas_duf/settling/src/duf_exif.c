#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <time.h>
/* #include <unistd.h> */

#include <errno.h>

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


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"

#include "duf_file.h"
#include "duf_path.h"


/* ###################################################################### */
#include "duf_exif.h"
/* ###################################################################### */


/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *                int fun(  const char *const *presult, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, 
 *                         void *str_cb_udata )
 * */
/* static int                                                                                                                                   */
/* duf_sel_cb_copy_jpeg_by_date( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata, */
/*                               duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )                             */
/* {                                                                                                                                            */
/*   int r = 0;                                                                                                                                 */
/*   const char *filename;                                                                                                                      */
/*   char *path = NULL;                                                                                                                         */
/*   unsigned long long pathid;                                                                                                                 */
/*                                                                                                                                              */
/*   (* sqlite3_int64 dataid; *)                                                                                                                */
/*   const char *datetime;                                                                                                                      */
/*                                                                                                                                              */
/*   (* dataid = *) ( void ) strtoll( precord->presult[0], NULL, 10 );                                                                          */
/*   pathid = strtoll( precord->presult[3], NULL, 10 );                                                                                         */
/*   path = duf_pathid_to_path_s( pathid, pdi, &r );                                                                                            */
/*                                                                                                                                              */
/*   fprintf( stderr, "PRESULT %s\n", __func__ );                                                                                               */
/*   datetime = precord->presult[4];                                                                                                            */
/*   filename = precord->presult[5];                                                                                                            */
/*                                                                                                                                              */
/*   if ( filename && path )                                                                                                                    */
/*   {                                                                                                                                          */
/*     char *base = "/mnt/new_media/media/years";                                                                                               */
/*     char *filepath;                                                                                                                          */
/*     char *new_path;                                                                                                                          */
/*     char *new_fpath;                                                                                                                         */
/*                                                                                                                                              */
/*     new_path = mas_strdup( base );                                                                                                           */
/*     {                                                                                                                                        */
/*       struct stat stf;                                                                                                                       */
/*                                                                                                                                              */
/*       if ( 0 != stat( new_path, &stf ) && errno == ENOENT )                                                                                  */
/*       {                                                                                                                                      */
/*         fprintf( stderr, "%s\n", new_path );                                                                                                 */
/*         mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );                                                                  */
/*       }                                                                                                                                      */
/*     }                                                                                                                                        */
/*     new_path = mas_strcat_x( new_path, "/" );                                                                                                */
/*     new_path = mas_strncat_x( new_path, datetime, 4 );                                                                                       */
/*     (* fprintf( stderr, "%s: %s\n new_path => %s\n", datetime, filepath, new_path ); *)                                                      */
/*     {                                                                                                                                        */
/*       struct stat stf;                                                                                                                       */
/*                                                                                                                                              */
/*       if ( 0 != stat( new_path, &stf ) && errno == ENOENT )                                                                                  */
/*       {                                                                                                                                      */
/*         fprintf( stderr, "%s\n", new_path );                                                                                                 */
/*         mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );                                                                  */
/*       }                                                                                                                                      */
/*     }                                                                                                                                        */
/*     new_path = mas_strcat_x( new_path, "/" );                                                                                                */
/*     new_path = mas_strncat_x( new_path, datetime + 5, 2 );                                                                                   */
/*     {                                                                                                                                        */
/*       struct stat stf;                                                                                                                       */
/*                                                                                                                                              */
/*       if ( 0 != stat( new_path, &stf ) && errno == ENOENT )                                                                                  */
/*       {                                                                                                                                      */
/*         fprintf( stderr, "%s\n", new_path );                                                                                                 */
/*         mkdir( new_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP );                                                                  */
/*       }                                                                                                                                      */
/*     }                                                                                                                                        */
/*     new_path = mas_strcat_x( new_path, "/" );                                                                                                */
/*     new_fpath = mas_strdup( new_path );                                                                                                      */
/*     new_fpath = mas_strncat_x( new_fpath, datetime, 4 );                                                                                     */
/*     new_fpath = mas_strncat_x( new_fpath, datetime + 5, 2 );                                                                                 */
/*     new_fpath = mas_strncat_x( new_fpath, datetime + 8, 2 );                                                                                 */
/*     new_fpath = mas_strcat_x( new_fpath, "-" );                                                                                              */
/*     new_fpath = mas_strncat_x( new_fpath, datetime + 11, 2 );                                                                                */
/*     new_fpath = mas_strncat_x( new_fpath, datetime + 14, 2 );                                                                                */
/*     new_fpath = mas_strncat_x( new_fpath, datetime + 17, 2 );                                                                                */
/*     (* new_fpath = mas_strcat_x( new_fpath, ".jpg" ); *)                                                                                     */
/*     new_fpath = mas_strcat_x( new_fpath, "." );                                                                                              */
/*     new_fpath = mas_strcat_x( new_fpath, filename );                                                                                         */
/*     filepath = duf_join_path( path, filename );                                                                                              */
/*     (* fprintf( stderr, "%u. %s\n  => %s\n", precord->nrow, filepath, new_fpath ); *)                                                        */
/*     mas_free( new_fpath );                                                                                                                   */
/*     mas_free( new_path );                                                                                                                    */
/*     mas_free( filepath );                                                                                                                    */
/*   }                                                                                                                                          */
/*   mas_free( path );                                                                                                                          */
/*   return r;                                                                                                                                  */
/* }                                                                                                                                            */

/* int                                                                                                                                      */
/* copy_jpeg_by_date( void )                                                                                                                */
/* {                                                                                                                                        */
/*   int r = 0;                                                                                                                             */
/*                                                                                                                                          */
/*   r = duf_sql_select( duf_sel_cb_copy_jpeg_by_date, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,          */
/*                       ( duf_scan_callbacks_t * ) NULL (*  sccb *) , ( const duf_dirhandle_t * ) NULL,                                    */
/*                       "SELECT duf_filedatas.id AS dataid, duf_filedatas.dev, duf_filedatas.inode, "                                      */
/*                       " duf_filenames.pathid, duf_exif.datetime, duf_filenames.name AS filename " " FROM duf_filedatas "                 */
/*                       " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "                                             */
/*                       " LEFT JOIN duf_exif ON (duf_exif.dataid=duf_filedatas.id) " " WHERE duf_exif.datetime IS NOT NULL "               */
/*                       " AND ( duf_filenames.name LIKE '%%.JpG' OR duf_filenames.name LIKE '%%.jPeG' ) " " ORDER BY duf_exif.datetime" ); */
/*   return r;                                                                                                                              */
/* }                                                                                                                                        */

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *          int fun(  const char *const *presult, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb,
 *                   void *str_cb_udata )
 * */
static int
duf_sel_cb_update_exif( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                        duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  int r = 0;

  /* const char *filename; */
  /* char *path = NULL; */
  /* unsigned long long pathid; */
  unsigned long long filenameid;
  unsigned long long dataid;
  char *filepath;

  /* pathid = strtoll( precord->presult[0], NULL, 10 ); */
  filenameid = strtoll( precord->presult[1], NULL, 10 );
  /* path = duf_pathid_to_path_dh( pathid, ( const duf_dirhandle_t * ) NULL ); */
  /* filename = precord->presult[2]; */
  dataid = strtoll( precord->presult[3], NULL, 10 );

  filepath = filenameid_to_filepath( filenameid, pdi, &r );

  /* fprintf( stderr, "!!!!!! %10s %15s %5s %5s [%s]\n", precord->presult[0], precord->presult[1], precord->presult[2], precord->presult[3], precord->presult[4] ); */

  /* if ( filename && path ) */
  if ( filepath )
  {

    /* filepath = duf_join_path( path, filename ); */

    /* fprintf( stderr, "++++++ %s\n", filepath ); */
    fprintf( stderr, "%lld  : %s\x1b[K\r", filenameid, filepath );
    /* fprintf( stderr, "filename:[%s]\n", filepath ); */
    {
      ExifData *ed;

      /* ExifTag tag1 = EXIF_TAG_DATE_TIME;           */
      /* ExifTag tag2 = EXIF_TAG_DATE_TIME_DIGITIZED; */
      /* ExifTag tag3 = EXIF_TAG_DATE_TIME_ORIGINAL;  */


      ed = exif_data_new_from_file( filepath );
      if ( ed )
      {
        {
          ExifEntry *entry;

          {
            char model[1024];

            entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], EXIF_TAG_MODEL );
            exif_entry_get_value( entry, model, sizeof( model ) );
            /* ??? exif_entry_free( entry ); */

            if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) )
/*                   || ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) */
                 || ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
            {
              char stime_original[1024];
              char *stime;
              char *pq;
              int changed = 0;
              struct tm times;

              stime = mas_strdup( stime_original );
              /* Get the contents of the tag in human-readable form */
              exif_entry_get_value( entry, stime_original, sizeof( stime_original ) );
              /* 2008:06:21 13:18:19 */
              while ( ( pq = strchr( stime, '?' ) ) )
              {
                int n;

                n = pq - stime;
                if ( n < 4 )
                {
                  if ( n > 1 )
                    *pq = '9';
                }
                else if ( n == 5 || n == 6 )
                {
                  stime[5] = '1';
                  stime[6] = '2';
                }
                else if ( n == 8 || n == 9 )
                {
                  stime[8] = '3';
                  stime[9] = '1';
                }
                else if ( n == 11 || n == 12 || n == 14 || n == 15 || n == 17 || n == 18 )
                {
                  stime[n] = '0';
                }
                changed = 1;
              }
              if ( strchr( stime, '?' ) )
                fprintf( stderr, "broken date %s\n", stime );
              {
                strptime( stime, "%Y:%m:%d %H:%M:%S", &times );
                /* strftime( buf1, sizeof( buf1 ), "%Y-%m-%d %H:%M:%S", &times ); */
                /* fprintf( stderr, "!!!!! %lu : %lu\n", t, time(NULL) ); */
                /* Don't bother printing it if it's entirely blank */
                /* trim_spaces( stime ); */
                if ( *stime )
                {
                  time_t t;

                  /* t = mktime( &times ); */
                  t = timelocal( &times );
                  /* fprintf( stderr, "%s\n", stime ); */
                  r = duf_sql( "INSERT INTO duf_exif (dataid, model, datetime, d, broken_date) "
                               " VALUES ( '%llu', '%s', datetime('%lu', 'unixepoch'), '%lu', '%s' )", ( int * ) NULL, dataid, model, t, t,
                               changed ? stime_original : "" );
                  /* sleep(1); */
                }
              }
              mas_free( stime );
              /* ??? exif_entry_free( entry ); */
            }
            else if ( 0 )
            {
              if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
                fprintf( stderr, "NO DATE - HAS +1\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
                fprintf( stderr, "NO DATE - HAS +2\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
                fprintf( stderr, "NO DATE - HAS +3\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
                fprintf( stderr, "NO DATE - HAS +4\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
                fprintf( stderr, "NO DATE - HAS +5\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
                fprintf( stderr, "NO DATE - HAS +6\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
                fprintf( stderr, "NO DATE - HAS +7\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME ) ) )
                fprintf( stderr, "NO DATE - HAS +8\n" );
              else if ( ( entry = exif_content_get_entry( ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME ) ) )
                fprintf( stderr, "NO DATE - HAS +9\n" );
              else
                fprintf( stderr, "NO DATE : %s\n", filepath );
              /* ??? exif_entry_free( entry ); */
            }
          }
        }
        exif_data_free( ed );
        /* exif_loader_unref( ed ); */
      }
    }
  }
  /* mas_free( path ); */
  mas_free( filepath );
  return r;
}

int
duf_update_exif( unsigned long long pathid )
{
  int r = 0;

  if ( pathid )
    r = duf_sql_select( duf_sel_cb_update_exif, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filenames.pathid, duf_filenames.id AS filenameid, duf_filenames.name AS filename, "
                        " duf_filedatas.id AS dataid, duf_filedatas.dev, duf_filedatas.inode " " FROM duf_filedatas "
                        " LEFT JOIN duf_exif ON (duf_exif.dataid=duf_filedatas.id) "
                        " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) " " WHERE duf_exif.datetime IS NULL "
                        " AND duf_filedatas.filetype='jpeg' " " AND pathid='%lld' ", pathid );
/* " AND ( duf_filenames.name LIKE '%%.JPG' OR duf_filenames.name LIKE '%%.JPEG' )" */
  else
    r = duf_sql_select( duf_sel_cb_update_exif, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filenames.pathid, duf_filenames.id AS filenameid, duf_filenames.name AS filename, "
                        " duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode " " FROM duf_filedatas "
                        " LEFT JOIN duf_exif ON (duf_exif.dataid=duf_filedatas.id) "
                        " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) " " WHERE duf_exif.datetime IS NULL "
                        " AND duf_filedatas.filetype='jpeg' " );
/* " AND ( duf_filenames.name LIKE '%%.JPG' OR duf_filenames.name LIKE '%%.JPEG' )" */
  return r;
}

int
duf_print_exif_same( unsigned long long dupcnt_min, unsigned long long limit )
{
  int r = 0;

  return r;
}
