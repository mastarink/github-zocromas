#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <assert.h>


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


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"


/* #include "duf_sql_const.h" */
#include "duf_sql_defs.h"

#include "duf_sql_field.h"
/* #include "duf_sql_field1.h" */

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"



DUF_UNUSED static unsigned long long
duf_insert_model_uni( duf_depthinfo_t * pdi, const char *model, int need_id, int *pr )
{
  int r = 0;
  unsigned long long modelid = 0;

  DEBUG_START(  );

  if ( r >= 0 && model )
  {
    int changes = 0;

    DEBUG_START(  );
    if ( need_id )
    {
      const char *sql = "SELECT id AS modelid FROM " DUF_DBPREF "exif_model WHERE model=:model";

      DUF_SQL_START_STMT( pdi, select_model, sql, r, pstmt_select );
      DUF_TEST_R( r );
      DUF_SQL_BIND_S( model, model, r, pstmt_select );
      DUF_TEST_R( r );
      DUF_SQL_STEP( r, pstmt_select );
      /* DUF_TEST_R( r ); */
      if ( r == DUF_SQL_ROW )
      {
        DUF_TRACE( current, 0, "<selected>" );
        modelid = duf_sql_column_long_long( pstmt_select, 0 );
        r = 0;
      }
      if ( r == DUF_SQL_DONE )
        r = 0;
      DUF_TEST_R( r );
      DUF_SQL_END_STMT( r, pstmt_select );
    }

    if ( !modelid && !duf_config->cli.disable.flag.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "exif_model ( model ) VALUES ( :model )";

      DUF_SQL_START_STMT( pdi, insert_model, sql, r, pstmt_insert );
      DUF_TEST_R( r );
      DUF_TRACE( insert, 0, " S: %s ", sql );
      DUF_SQL_BIND_S( model, model, r, pstmt_insert );
      DUF_TEST_R( r );

      DUF_SQL_STEP( r, pstmt_insert );
      /* DUF_TEST_R( r ); */
      DUF_SQL_CHANGES( changes, r, pstmt_insert );
      /* DUF_TEST_R( r ); */
      /* DUF_ERROR( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        modelid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( exif, 0, " inserted now( SQLITE_OK ) modelid = %llu ", modelid );
      }
      DUF_SQL_END_STMT( r, pstmt_insert );
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( " Wrong data " );
    r = DUF_ERROR_DATA;
  }

  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( modelid );
  DUF_TEST_R( r );
  assert( modelid );
  return modelid;
}

DUF_UNUSED static unsigned long long
duf_insert_exif_uni( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, const char *model, time_t timeepoch, const char *stime_original,
                     int need_id, int *pr )
{
  int r = 0;
  unsigned long long modelid = 0;
  unsigned long long exifid = 0;

  DEBUG_START(  );
  modelid = duf_insert_model_uni( pdi, model, 1 /*need_id */ , &r );
  if ( r >= 0 && ( timeepoch || modelid ) )
  {

    DEBUG_START(  );
    if ( need_id )
    {
      const char *sql =
            "SELECT id AS exifid FROM " DUF_DBPREF "exif WHERE modelid=:modelid AND date_time=datetime(:timeepoch, 'unixepoch')";

      DUF_SQL_START_STMT( pdi, select_exif, sql, r, pstmt_select );
      DUF_TEST_R( r );
      DUF_SQL_BIND_LL( modelid, modelid, r, pstmt_select );
      DUF_TEST_R( r );
      DUF_SQL_BIND_LL( timeepoch, timeepoch, r, pstmt_select );
      DUF_TEST_R( r );
      DUF_SQL_STEP( r, pstmt_select );
      /* DUF_TEST_R( r ); */
      if ( r == DUF_SQL_ROW )
      {
        DUF_TRACE( current, 0, "<selected>" );
        exifid = duf_sql_column_long_long( pstmt_select, 0 );
        r = 0;
      }
      if ( r == DUF_SQL_DONE )
        r = 0;
      DUF_TEST_R( r );
      DUF_SQL_END_STMT( r, pstmt_select );
      /* if ( !exifid )                        */
      /*   DUF_ERROR( "exifid NOT SELECTED" ); */
    }

    if ( !exifid && !duf_config->cli.disable.flag.insert )
    {
      int changes = 0;
      const char *sql =
            "INSERT OR IGNORE INTO " DUF_DBPREF "exif ( modelid, date_time, broken_date ) "
            " VALUES ( :modelid, datetime(:timeepoch, 'unixepoch'), :origtime )";

      DUF_SQL_START_STMT( pdi, insert_exif, sql, r, pstmt_insert );
      DUF_TEST_R( r );
      DUF_TRACE( insert, 0, " S: %s ", sql );
      DUF_TEST_R( r );
      DUF_SQL_BIND_LL( modelid, modelid, r, pstmt_insert );
      DUF_TEST_R( r );
      DUF_SQL_BIND_LL( timeepoch, timeepoch, r, pstmt_insert );
      DUF_TEST_R( r );
      DUF_SQL_BIND_S( origtime, stime_original, r, pstmt_insert );
      DUF_TEST_R( r );
      /* {                                                                                               */
      /*   const char *real_path = NULL;                                                                 */
      /*   static int c = 0;                                                                             */
      /*   static int n = 0;                                                                             */
      /*                                                                                                 */
      /*   if ( changes )                                                                                */
      /*     c++;                                                                                        */
      /*   n++;                                                                                          */
      /*                                                                                                 */
      /*   DUF_SFIELD2( filename );                                                                      */
      /*   real_path = duf_levinfo_path( pdi );                                                          */
      /*                                                                                                 */
      /*   DUF_ERROR( "%d. nchanges:%d %llu:%lu  %s%s", n, c, modelid, timeepoch, real_path, filename ); */
      /* }                                                                                               */
      DUF_SQL_STEP( r, pstmt_insert );
      /* DUF_TEST_R(r); */
      DUF_SQL_CHANGES( changes, r, pstmt_insert );
      if ( need_id && changes )
      {
        exifid = duf_sql_last_insert_rowid(  );
        assert( exifid );
      }
      if ( !changes )
        DUF_ERROR( "exifid NOT INSERTED:%s - %llu, %lu, %s", sql, modelid, timeepoch, stime_original );
      {
        const char *real_path = NULL;

        DUF_SFIELD2( filename );
        real_path = duf_levinfo_path( pdi );
        DUF_TRACE( exif, 0, " inserted now( SQLITE_OK ) exifid=%llu; modelid=%llu; %lu ; changes:%d; %s%s", exifid, modelid,
                   ( long ) timeepoch, changes, real_path, filename );
      }

      DUF_SQL_END_STMT( r, pstmt_insert );
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( " Wrong data " );
    r = DUF_ERROR_DATA;
  }

  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( exifid );
  DUF_TEST_R( r );
  return exifid;
}

static time_t
duf_exif_get_time( ExifData * edata, int *pdate_changed, char *stime_original, size_t stime_original_size, int *pr )
{
  int r = 0;
  time_t timeepoch = 0;
  ExifEntry *entry = NULL;
  int changed = 0;

  if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) )
/*                 || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) */
       || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
  {
    /* Get the contents of the tag in human-readable form */
    if ( r >= 0 && !exif_entry_get_value( entry, stime_original, stime_original_size ) )
      r = DUF_ERROR_EXIF;
    {
      char *corrected_time;

      corrected_time = mas_strdup( stime_original );
      /* DUF_ERROR( "@@@@@@@@@@@@@@ %s", stime_original ); */
      DUF_TRACE( exif, 3, "stime_original:%s", stime_original );
      /* 2008:06:21 13:18:19 */
      if ( r >= 0 )
      {
        char *pq;

        while ( ( pq = strchr( corrected_time, '?' ) ) )
        {
          int n;

          n = pq - corrected_time;
          if ( n < 4 )
          {
            if ( n > 1 )
              *pq = '9';
          }
          else if ( n == 5 || n == 6 )
          {
            corrected_time[5] = '1';
            corrected_time[6] = '2';
          }
          else if ( n == 8 || n == 9 )
          {
            corrected_time[8] = '3';
            corrected_time[9] = '1';
          }
          else if ( n == 11 || n == 12 || n == 14 || n == 15 || n == 17 || n == 18 )
          {
            corrected_time[n] = '0';
          }
          changed = 1;
        }
      }
      if ( r >= 0 && !*corrected_time )
        r = DUF_ERROR_EXIF_NO_DATE;
      if ( r >= 0 && strchr( corrected_time, '?' ) )
      {
        DUF_ERROR( "broken date %s", corrected_time );
        r = DUF_ERROR_EXIF_BROKEN_DATE;
      }
      if ( r >= 0 || r == DUF_ERROR_EXIF_BROKEN_DATE )
      {

        {
          struct tm times = { 0 };

          strptime( corrected_time, "%Y:%m:%d %H:%M:%S", &times );

          /* timeepoch = mktime( &times ); */

          /* timeepoch = timelocal( &times ); */
          timeepoch = timegm( &times );

          /* strftime( buf1, sizeof( buf1 ), "%Y-%m-%d %H:%M:%S", &times ); */
          /* DUF_ERROR( "!!!!! %lu : %lu", timeepoch, time(NULL) ); */
          /* Don't bother printing it if it's entirely blank */
          /* trim_spaces( corrected_time ); */
        }
      }
      mas_free( corrected_time );
    }
    /* ??? exif_entry_free( entry ); */
  }
  else if ( 0 )
  {
    r = DUF_ERROR_EXIF_NO_DATE;
    if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_ERROR( "NO DATE - HAS +1" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_ERROR( "NO DATE - HAS +2" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_ERROR( "NO DATE - HAS +3" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_ERROR( "NO DATE - HAS +4" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_ERROR( "NO DATE - HAS +5" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_ERROR( "NO DATE - HAS +6" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
      DUF_ERROR( "NO DATE - HAS +7" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME ) ) )
      DUF_ERROR( "NO DATE - HAS +8" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME ) ) )
      DUF_ERROR( "NO DATE - HAS +9" );
    else
      DUF_ERROR( "NO DATE" );
    /* ??? exif_entry_free( entry ); */
  }

  if ( pdate_changed )
    *pdate_changed = changed;
  if ( pr )
    *pr = r;
  return timeepoch;
}

static int
duf_scan_dirent_exif_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_UFIELD2( dataid );

  DEBUG_START(  );
  {
    size_t bufsz = 1024;
    unsigned char *buffer;
    size_t sum = 0;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 2;
      ExifLoader *loader;

      loader = exif_loader_new(  );

      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( r >= 0 && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        DUF_TRACE( exif, 5, "read %d", rr );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          r = DUF_ERROR_READ;
        }
        DUF_TEST_R( r );
        if ( rr > 0 )
        {
          sum += rr;
          if ( !exif_loader_write( loader, buffer, rr ) )
          {
            r = DUF_ERROR_EXIF_END;
            break;
          }
        }
        if ( rr <= 0 )
        {
          r = DUF_ERROR_EOF;
          break;
        }

        DUF_TEST_R( r );
      }
      mas_free( buffer );
      if ( r == DUF_ERROR_EOF || r == DUF_ERROR_EXIF_END )
        r = 0;
      if ( r >= 0 )
      {
        ExifData *edata = NULL;

        /* ExifTag tag1 = EXIF_TAG_DATE_TIME;           */
        /* ExifTag tag2 = EXIF_TAG_DATE_TIME_DIGITIZED; */
        /* ExifTag tag3 = EXIF_TAG_DATE_TIME_ORIGINAL;  */

        edata = exif_loader_get_data( loader );
        exif_loader_unref( loader );
        if ( r >= 0 && edata )
        {
          /* ??? exif_entry_free( entry ); */
          char stime_original[1024] = { '\0' };
          time_t timeepoch = 0;
          int date_changed = 0;
          char *model = NULL;

          {
            ExifEntry *entry = NULL;
            char tmodel[1024] = "";

            entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_MODEL );
            if ( !entry )
              r = DUF_ERROR_EXIF_NO_MODEL;
            if ( r >= 0 && !exif_entry_get_value( entry, tmodel, sizeof( tmodel ) ) )
              r = DUF_ERROR_EXIF_NO_MODEL;
            mas_chomp( tmodel );
            model = mas_strdup( tmodel );
          }

          timeepoch = duf_exif_get_time( edata, &date_changed, stime_original, sizeof( stime_original ), &r );
          /* DUF_ERROR( "@@@@@@@@@@@@@@ %lu - %lu", sum, timeepoch ); */
          if ( timeepoch || *stime_original || model )
          {
            unsigned long long exifid = 0;
            const char *real_path = NULL;

            DUF_SFIELD2( filename );
            real_path = duf_levinfo_path( pdi );
            DUF_TRACE( exif, 1, "%s%s", real_path, filename );

            exifid = duf_insert_exif_uni( pstmt, pdi, model, timeepoch, stime_original, 1 /* need_id */ , &r );
            DUF_TRACE( exif, 0, "ID:%llu; (%d) read %lu m:%s t:%lu; %s%s", exifid, r, sum, model, timeepoch, real_path, filename );

            DUF_TRACE( exif, 2, "exifid:%llu; dataid:%llu; model:'%s'; datetime:%ld", exifid, dataid, model, ( long ) timeepoch );

            /* r = duf_sql( "INSERT INTO duf_exif (dataid, model, datetime, d, broken_date) "                                   */
            /*              " VALUES ( %llu, '%s', datetime('%lu', 'unixepoch'), '%lu', '%s' )", ( int * ) NULL, dataid, model, */
            /*              timeepoch, timeepoch, date_changed ? stime_original : "" );                                         */
            if ( r >= 0 && exifid && !duf_config->cli.disable.flag.update )
            {
              int changes = 0;

              if ( 1 )
              {
                const char *sql = " UPDATE " DUF_DBPREF " filedatas SET exifid = :exifid WHERE id = :dataid ";

                DUF_SQL_START_STMT( pdi, update_exif, sql, r, pstmt_update );
                DUF_TRACE( update, 0, " S: %s ", sql );
                DUF_SQL_BIND_LL( exifid, exifid, r, pstmt_update );
                DUF_SQL_BIND_LL( dataid, dataid, r, pstmt_update );
                DUF_SQL_STEP( r, pstmt_update );
                /* DUF_TEST_R(r); */
                DUF_SQL_CHANGES( changes, r, pstmt_update );
                DUF_SQL_END_STMT( r, pstmt_update );
              }
              else
              {
                r = duf_sql( " UPDATE " DUF_DBPREF " filedatas SET exifid = %llu WHERE id = %lld", &changes, exifid, dataid );
                duf_pdi_reg_changes( pdi, changes );
              }


              DUF_TEST_R( r );
            }
          }
          else
          {
            DUF_ERROR( "!!! (%d) read %lu m:%s t:%lu", r, sum, model, timeepoch );
          }
          if ( r == DUF_ERROR_EXIF_NO_MODEL )
            r = 0;
          mas_free( model );
          model = NULL;
          /* sleep(1); */


          /* ??? exif_entry_free( entry ); */
          DUF_TEST_R( r );

          /* edata = exif_data_new_from_file( filepath ); */
          if ( edata )
          {
            exif_data_free( edata );
            /* exif_loader_unref( edata ); */
          }
        }
      }
      DUF_TEST_R( r );
    }
    else
    {
      r = DUF_ERROR_MEMORY;
    }
    DUF_TEST_R( r );
  }


  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}





static const char *final_sql[] = {
  /* "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " (* *)                            */
  /*       " FROM " DUF_DBPREF "exif AS x " (* *)                                                  */
  /*       " JOIN " DUF_DBPREF "exif_model AS mo ON (x.modelid=mo.id) " (* *)                      */
  /*       " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.exifid=x.id) " (* *)                        */
  /*       " WHERE " DUF_DBPREF "exif.modelid=x.modelid AND exif.date_time=x.date_time" " )" (* *) */
  /*       ,                                                                                       */


  NULL,
};


duf_scan_callbacks_t duf_collect_exif_callbacks = {
  .title = "collect exif",
  .def_opendir = 1,
  .scan_mode_2 = 1,

  .leaf_scan_fd2 = duf_scan_dirent_exif_content2,

  /* filename for debug only */
  .leaf_fieldset = " fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize, fd.id as dataid " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn.id AS filenameid " /* */
        ,
  .leaf_selector2 =             /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " exif AS x ON( fd.exifid = x.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
        " WHERE "               /* */
        " ( fd.exifid IS NULL  OR x.modelid IS NULL ) AND" /* */
	" sz.size > 0 AND"
        " mi.mime='image/jpeg' AND" /* */
        " fn.Pathid = :dirid "  /* */
        ,
  .leaf_selector_total2 =       /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " exif AS x ON( fd.exifid = x.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
        " WHERE "               /* */
        " ( fd.exifid IS NULL  OR x.modelid IS NULL ) AND " /* */
        " mi.mime='image/jpeg' " /* */
        ,
  .node_fieldset = " pt.id AS dirid, pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        ,
  .node_selector2 =             /* */
        " FROM " DUF_DBPREF " paths AS pt " /* */
        " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt.id ) " /* */
        " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt.id ) " /* */
        " WHERE pt.parentid = :dirid " /* */
        ,
  .final_sql_argv = final_sql,
};
