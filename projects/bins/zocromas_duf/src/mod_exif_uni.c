#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "mod_exif.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"




#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ########################################################################################## */
static int dirent_contnt2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {
  .done = 0,.sql = {
                    "UPDATE " DUF_DBPREF "exif SET dupexifcnt=(SELECT COUNT(*) " /* */
                    " FROM " DUF_DBPREF "filedatas AS fd " /* */
                    " JOIN " DUF_DBPREF "exif AS x ON (fd.exifid=x." DUF_SQL_IDNAME ") " /* */
                    " WHERE exif." DUF_SQL_IDNAME "=x." DUF_SQL_IDNAME " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
                    , NULL}
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_collect_exif_callbacks = {
  .title = "collect exif",      /* */
  .name = "exif",               /* */
  .def_opendir = 1,             /* */
  .leaf_scan_fd2 = dirent_contnt2, /* */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  /* filename for debug only */
  .leaf = {
           .fieldset = " fn.Pathid AS dirid, fn.name AS filename, fn.name AS dfname, fd.size AS filesize, fd." DUF_SQL_IDNAME " as dataid " /* */
           " , 0 as ndirs, 0 as nfiles" /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           "  "                 /* */
           " , fd.mode AS filemode " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fn." DUF_SQL_IDNAME " AS nameid " /* */
           " , fd.md5id AS md5id" /* */
           ,                    /* */
           .selector2 =         /* */
           " FROM " DUF_DBPREF " filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " exif AS x ON( fd.exifid = x." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
           ,
           .matcher = " fn.Pathid = :parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.exifid IS NULL  OR x.modelid IS NULL ) AND" /* */
           " sz.size > 0                                 AND" /* */
           " mi.mime='image/jpeg'                        AND" /* */
           " 1 "                /* */
           ,                    /* */
           .count_aggregate = "distinct fd." DUF_SQL_IDNAME
#if 0
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF " filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " exif AS x ON( fd.exifid = x." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
           ,                    /* */
#endif
           },                   /* */
  .node = {
           .fieldset = " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
           ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,                    /* */
           .selector2 =         /* */
           " FROM " DUF_DBPREF " paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
#endif
           ,
           .matcher = "pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           ,                    /* */
           .filter = NULL       /* */
#if 0
           ,.selector_total2 =  /* */
           " /* exif */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */



DUF_UNUSED static unsigned long long
duf_insert_model_uni( duf_depthinfo_t * pdi, const char *model, int need_id, int *pr )
{
  int lr = 0;

  DEBUG_STARTULL( modelid );

  if ( model && *model )
  {
    int changes = 0;

    if ( need_id )
    {
      const char *sql = "SELECT " DUF_SQL_IDNAME " AS modelid FROM " DUF_DBPREF "exif_model WHERE model=:Model";

      DUF_SQL_START_STMT( pdi, select_model, sql, lr, pstmt_select );
      DUF_TEST_R( lr );
      DUF_SQL_BIND_S( Model, model, lr, pstmt_select );
      DUF_TEST_R( lr );
      DUF_SQL_STEP( lr, pstmt_select );
      /* DUF_TEST_R( lr ); */
      if ( lr == DUF_SQL_ROW )
      {
        DUF_TRACE( select, 0, "<selected>" );
        modelid = duf_sql_column_long_long( pstmt_select, 0 );
        lr = 0;
      }
      if ( lr == DUF_SQL_DONE )
        lr = 0;
      DUF_TEST_R( lr );
      DUF_SQL_END_STMT( select_model, lr, pstmt_select );
    }

    if ( !modelid && !duf_config->cli.disable.flag.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "exif_model ( model ) VALUES ( :Model )";

      DUF_SQL_START_STMT( pdi, insert_model, sql, lr, pstmt_insert );
      DUF_TEST_R( lr );
      DUF_TRACE( insert, 0, " S: %s ", sql );
      DUF_SQL_BIND_S( Model, model, lr, pstmt_insert );
      DUF_TEST_R( lr );

      DUF_SQL_STEP( lr, pstmt_insert );
      /* DUF_TEST_R( lr ); */
      DUF_SQL_CHANGES( changes, lr, pstmt_insert );
      /* DUF_TEST_R( lr ); */
      /* DUF_SHOW_ERROR( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        modelid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( exif, 0, " inserted now( SQLITE_OK ) modelid = %llu ", modelid );
      }
      DUF_SQL_END_STMT( insert_model, lr, pstmt_insert );
    }
    DUF_TEST_R( lr );
  }
  else
  {
    /* No model is not soooo bad! */
    /* DUF_SHOW_ERROR( " Wrong data " ); */
    /* DUF_MAKE_ERROR(lr, DUF_ERROR_DATA);          */
  }
  DUF_TEST_R( lr );

  if ( pr )
    *pr = lr;
  DUF_TEST_R( lr );
  /* assert( modelid ); */
  DEBUG_ENDULL( modelid );
}

DUF_UNUSED static unsigned long long
duf_insert_exif_uni( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, const char *model, time_t timeepoch, int dtfixed, const char *stime_original,
                     int need_id, int *pr )
{
  int lr = 0;
  unsigned long long modelid = 0;

  DEBUG_STARTULL( exifid );
  modelid = duf_insert_model_uni( pdi, model, 1 /*need_id */ , &lr );
  if ( lr >= 0 && ( timeepoch || modelid || dtfixed || stime_original ) )
  {

    if ( need_id )
    {
      const char *sql =
            "SELECT " DUF_SQL_IDNAME " AS exifid FROM " DUF_DBPREF "exif WHERE ( :modelID IS NULL OR modelid=:modelID ) "
            " AND date_time=datetime(:timeEpoch, 'unixepoch')";

      DUF_SQL_START_STMT( pdi, select_exif, sql, lr, pstmt_select );
      DUF_TEST_R( lr );
      if ( modelid )
      {
        DUF_SQL_BIND_LL( modelID, modelid, lr, pstmt_select );
        DUF_TEST_R( lr );
      }
      DUF_SQL_BIND_LL( timeEpoch, timeepoch, lr, pstmt_select );
      DUF_TEST_R( lr );
      DUF_SQL_STEP( lr, pstmt_select );
      /* DUF_TEST_R( lr ); */
      if ( lr == DUF_SQL_ROW )
      {
        DUF_TRACE( select, 0, "<selected>" );
        exifid = duf_sql_column_long_long( pstmt_select, 0 );
        lr = 0;
      }
      if ( lr == DUF_SQL_DONE )
        lr = 0;
      DUF_TEST_R( lr );
      DUF_SQL_END_STMT( select_exif, lr, pstmt_select );
      /* if ( !exifid )                        */
      /*   DUF_SHOW_ERROR( "exifid NOT SELECTED" ); */
    }

    if ( !exifid && !duf_config->cli.disable.flag.insert )
    {
      int changes = 0;
      const char *sql =
            "INSERT OR IGNORE INTO " DUF_DBPREF "exif ( modelid, date_time, broken_date, fixed ) "
            " VALUES ( :modelID, datetime(:timeEpoch, 'unixepoch'), :origTime, :dtFixed )";

      DUF_SQL_START_STMT( pdi, insert_exif, sql, lr, pstmt_insert );
      DUF_TEST_R( lr );
      DUF_TRACE( insert, 0, " S: %s ", sql );
      DUF_TEST_R( lr );
      if ( modelid )
      {
        DUF_SQL_BIND_LL( modelID, modelid, lr, pstmt_insert );
        DUF_TEST_R( lr );
      }
      DUF_SQL_BIND_LL_NZ_OPT( timeEpoch, timeepoch, lr, pstmt_insert );
      DUF_TEST_R( lr );
      DUF_SQL_BIND_LL_NZ_OPT( dtFixed, dtfixed, lr, pstmt_insert );
      DUF_TEST_R( lr );
      DUF_SQL_BIND_S( origTime, stime_original, lr, pstmt_insert );
      DUF_TEST_R( lr );
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
      /*   DUF_SHOW_ERROR( "%d. nchanges:%d %llu:%lu  %s%s", n, c, modelid, timeepoch, real_path, filename ); */
      /* }                                                                                               */
      DUF_SQL_STEP( lr, pstmt_insert );
      /* DUF_TEST_R(lr); */
      DUF_SQL_CHANGES( changes, lr, pstmt_insert );
      if ( need_id && changes )
      {
        exifid = duf_sql_last_insert_rowid(  );
        assert( exifid );
      }
      if ( !changes )
        DUF_SHOW_ERROR( "exifid NOT INSERTED:%s - %llu, %lu, %s", sql, modelid, timeepoch, stime_original );
      {
#ifdef MAS_TRACING
        DUF_SFIELD2( filename );
#endif
        DUF_TRACE( exif, 1, " inserted now( SQLITE_OK ) exifid=%llu; modelid=%llu; %lu ; changes:%d; %s%s", exifid, modelid,
                   ( long ) timeepoch, changes, duf_levinfo_path( pdi ), filename );
      }

      DUF_SQL_END_STMT( insert_exif, lr, pstmt_insert );
    }
    DUF_TEST_R( lr );
  }
  else
  {
    DUF_SHOW_ERROR( " Wrong data " );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
  }

  DUF_TEST_R( lr );
  if ( pr )
    *pr = lr;
  DEBUG_ENDULL( exifid );
}

static time_t
duf_exif_get_time( ExifData * edata, int *pdate_changed, char *stime_original, size_t stime_original_size, int *pr )
{
  int lr = 0;
  time_t timeepoch = 0;
  ExifEntry *entry = NULL;
  int changed = 0;

#if 0
  if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) )
/*                 || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) */
       || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
#else
  if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_ORIGINAL ) ) || ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME ) ) )
#endif
  {
    memset( stime_original, stime_original_size, 0 );
    /* Get the contents of the tag in human-readable form */
    if ( lr >= 0 && !exif_entry_get_value( entry, stime_original, stime_original_size ) )
      DUF_MAKE_ERROR( lr, DUF_ERROR_EXIF );
    {
      char *corrected_time = NULL;

      for ( int i = 0; i < strlen( stime_original ); i++ )
      {
        char c;

        c = stime_original[i];
        if ( ( c < ' ' || c > 'z' ) && c != '?' && c != ':' && c != ' ' )
        {
          DUF_TRACE( exif, 0, ">>>>>>>>>>>>>> %s <<<<<<<<<<<<<", stime_original );
          DUF_MAKE_ERROR( lr, DUF_ERROR_EXIF_BROKEN_DATE );
          break;
        }
      }
      if ( lr >= 0 )
        corrected_time = mas_strdup( stime_original );
      /* DUF_SHOW_ERROR( "@@@@@@@@@@@@@@ %s", stime_original ); */
      DUF_TRACE( exif, 3, "stime_original:%s", stime_original );
      /* 2008:06:21 13:18:19 */
      if ( lr >= 0 )
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
          changed++;
        }
      }
      if ( lr >= 0 && !*corrected_time )
        DUF_MAKE_ERROR( lr, DUF_ERROR_EXIF_NO_DATE );
      else if ( lr >= 0 && strchr( corrected_time, '?' ) )
      {
        DUF_SHOW_ERROR( "broken date %s", corrected_time );
        DUF_MAKE_ERROR( lr, DUF_ERROR_EXIF_BROKEN_DATE );
      }
      if ( lr >= 0 || lr == DUF_ERROR_EXIF_BROKEN_DATE )
      {
        if ( corrected_time && *corrected_time )
        {
          struct tm times = { 0 };

          strptime( corrected_time, "%Y:%m:%d %H:%M:%S", &times );

          /* timeepoch = mktime( &times ); */

          /* timeepoch = timelocal( &times ); */
          timeepoch = timegm( &times );

          /* strftime( buf1, sizeof( buf1 ), "%Y-%m-%d %H:%M:%S", &times ); */
          /* DUF_SHOW_ERROR( "!!!!! %lu : %lu", timeepoch, time(NULL) ); */
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
#if 0
#  define exif_data_get_entry(d, t) (
    exif_content_get_entry( d->ifd[EXIF_IFD_0], t ) ?
          exif_content_get_entry( d->ifd[EXIF_IFD_0], t ) :
          exif_content_get_entry( d->ifd[EXIF_IFD_1], t ) ?
          exif_content_get_entry( d->ifd[EXIF_IFD_1], t ) :
          exif_content_get_entry( d->ifd[EXIF_IFD_EXIF], t ) ?
          exif_content_get_entry( d->ifd[EXIF_IFD_EXIF], t ) :
          exif_content_get_entry( d->ifd[EXIF_IFD_GPS], t ) ?
          exif_content_get_entry( d->ifd[EXIF_IFD_GPS], t ) :
          exif_content_get_entry( d->ifd[EXIF_IFD_INTEROPERABILITY], t ) ? exif_content_get_entry( d->ifd[EXIF_IFD_INTEROPERABILITY], t ) : NULL )
#endif
          DUF_MAKE_ERROR( lr, DUF_ERROR_EXIF_NO_DATE );
#if 0
    if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +1" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +2" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +3" );
#else
    if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +123" );
#endif
#if 0
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +4" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +5" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +6" );
#else
    else if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +456" );
#endif

#if 0
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +7" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +8" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +9" );
#else
    else if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERROR( "NO DATE - HAS +789" );
#endif
    else
      DUF_SHOW_ERROR( "NO DATE" );
    /* ??? exif_entry_free( entry ); */
  }

  if ( pdate_changed )
    *pdate_changed = changed;
  if ( pr )
    *pr = lr;
  return timeepoch;
}

static int dirent_contnt2( duf_sqlite_stmt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );



  DUF_UFIELD2( dataid );

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
      /* exif_loader_write_file */
      while ( r >= 0 && cnt++ < maxcnt )
      {
        int rr;

        rr = read( duf_levinfo_dfd( pdi ), buffer, bufsz );
        DUF_TRACE( exif, 5, "read %d", rr );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          DUF_MAKE_ERROR( r, DUF_ERROR_READ );
        }
        DUF_TEST_R( r );

        if ( rr > 0 )
        {
          sum += rr;
          if ( !exif_loader_write( loader, buffer, rr ) )
          {
            DUF_MAKE_ERROR( r, DUF_ERROR_EXIF_END );
            break;
          }
        }
        if ( rr <= 0 )
        {
          DUF_MAKE_ERROR( r, DUF_ERROR_EOF );
          break;
        }

        DUF_TEST_R( r );
      }
      mas_free( buffer );
      DUF_CLEAR_ERROR( r, DUF_ERROR_EOF, DUF_ERROR_EXIF_END );
      if ( r >= 0 )
      {
        ExifData *edata = NULL;

        /* ExifTag tag1 = EXIF_TAG_DATE_TIME;           */
        /* ExifTag tag2 = EXIF_TAG_DATE_TIME_DIGITIZED; */
        /* ExifTag tag3 = EXIF_TAG_DATE_TIME_ORIGINAL;  */

        edata = exif_loader_get_data( loader );
        exif_loader_unref( loader );
        if ( r >= 0 && edata /* && edata->size */  )
        {
          /* ??? exif_entry_free( entry ); */
          char stime_original[1024] =
          {
          '\0'};
          time_t timeepoch = 0;
          int date_changed = 0;
          char *model = NULL;

          /* Other tags
           *
           * === EXIF_IFD_0,EXIF_IFD_1,EXIF_IFD_EXIF ===
           *    EXIF_TAG_INTEROPERABILITY_INDEX
           *    EXIF_TAG_INTEROPERABILITY_VERSION
           *    EXIF_TAG_NEW_SUBFILE_TYPE
           *  ! EXIF_TAG_IMAGE_WIDTH
           *  ! EXIF_TAG_IMAGE_LENGTH
           *    EXIF_TAG_BITS_PER_SAMPLE
           *  ! EXIF_TAG_COMPRESSION
           *    EXIF_TAG_PHOTOMETRIC_INTERPRETATION
           *    EXIF_TAG_FILL_ORDER
           *    EXIF_TAG_DOCUMENT_NAME
           *    EXIF_TAG_IMAGE_DESCRIPTION
           *  ! EXIF_TAG_MAKE       /// LG Electronics
           *    EXIF_TAG_MODEL
           *    EXIF_TAG_STRIP_OFFSETS
           *  ! EXIF_TAG_ORIENTATION
           *    EXIF_TAG_SAMPLES_PER_PIXEL
           *    EXIF_TAG_ROWS_PER_STRIP
           *    EXIF_TAG_STRIP_BYTE_COUNTS
           *  ! EXIF_TAG_X_RESOLUTION
           *  ! EXIF_TAG_Y_RESOLUTION
           *    EXIF_TAG_PLANAR_CONFIGURATION
           *  ! EXIF_TAG_RESOLUTION_UNIT
           *    EXIF_TAG_TRANSFER_FUNCTION
           *    EXIF_TAG_SOFTWARE
           * !! EXIF_TAG_DATE_TIME
           *    EXIF_TAG_ARTIST
           *    EXIF_TAG_WHITE_POINT
           *    EXIF_TAG_PRIMARY_CHROMATICITIES
           *    EXIF_TAG_SUB_IFDS
           *    EXIF_TAG_TRANSFER_RANGE
           *    EXIF_TAG_JPEG_PROC
           *    EXIF_TAG_JPEG_INTERCHANGE_FORMAT
           *    EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH
           *    EXIF_TAG_YCBCR_COEFFICIENTS
           *    EXIF_TAG_YCBCR_SUB_SAMPLING
           *    EXIF_TAG_YCBCR_POSITIONING
           *    EXIF_TAG_REFERENCE_BLACK_WHITE
           *    EXIF_TAG_XML_PACKET
           *    EXIF_TAG_RELATED_IMAGE_FILE_FORMAT
           *    EXIF_TAG_RELATED_IMAGE_WIDTH
           *    EXIF_TAG_RELATED_IMAGE_LENGTH
           *    EXIF_TAG_CFA_REPEAT_PATTERN_DIM
           *    EXIF_TAG_CFA_PATTERN
           *    EXIF_TAG_BATTERY_LEVEL
           *    EXIF_TAG_COPYRIGHT
           *  ! EXIF_TAG_EXPOSURE_TIME     ==? EXIF_TAG_SHUTTER_SPEED_VALUE
           *    EXIF_TAG_FNUMBER
           *    EXIF_TAG_IPTC_NAA
           *    EXIF_TAG_IMAGE_RESOURCES
           *    EXIF_TAG_EXIF_IFD_POINTER
           *    EXIF_TAG_INTER_COLOR_PROFILE
           *    EXIF_TAG_EXPOSURE_PROGRAM
           *    EXIF_TAG_SPECTRAL_SENSITIVITY
           *    EXIF_TAG_GPS_INFO_IFD_POINTER
           *  ! EXIF_TAG_ISO_SPEED_RATINGS
           *    EXIF_TAG_OECF
           *    EXIF_TAG_TIME_ZONE_OFFSET
           *    EXIF_TAG_EXIF_VERSION
           * !! EXIF_TAG_DATE_TIME_ORIGINAL
           * !! EXIF_TAG_DATE_TIME_DIGITIZED
           *    EXIF_TAG_COMPONENTS_CONFIGURATION
           *    EXIF_TAG_COMPRESSED_BITS_PER_PIXEL
           *  ! EXIF_TAG_SHUTTER_SPEED_VALUE    ==? EXIF_TAG_EXPOSURE_TIME
           *  ! EXIF_TAG_APERTURE_VALUE     /// Fixed for camera?
           *    EXIF_TAG_BRIGHTNESS_VALUE
           *    EXIF_TAG_EXPOSURE_BIAS_VALUE
           *    EXIF_TAG_MAX_APERTURE_VALUE
           *    EXIF_TAG_SUBJECT_DISTANCE
           *    EXIF_TAG_METERING_MODE
           *    EXIF_TAG_LIGHT_SOURCE
           *  ! EXIF_TAG_FLASH      /// Fired / No flash
           *  ! EXIF_TAG_FOCAL_LENGTH     /// Fixed for camera?
           *    EXIF_TAG_SUBJECT_AREA
           *    EXIF_TAG_TIFF_EP_STANDARD_ID
           *    EXIF_TAG_MAKER_NOTE
           *    EXIF_TAG_USER_COMMENT
           *    EXIF_TAG_SUB_SEC_TIME
           *    EXIF_TAG_SUB_SEC_TIME_ORIGINAL
           *    EXIF_TAG_SUB_SEC_TIME_DIGITIZED
           *    EXIF_TAG_XP_TITLE
           *    EXIF_TAG_XP_COMMENT
           *    EXIF_TAG_XP_AUTHOR
           *    EXIF_TAG_XP_KEYWORDS
           *    EXIF_TAG_XP_SUBJECT
           *    EXIF_TAG_FLASH_PIX_VERSION
           *    EXIF_TAG_COLOR_SPACE
           *    EXIF_TAG_PIXEL_X_DIMENSION
           *    EXIF_TAG_PIXEL_Y_DIMENSION
           *    EXIF_TAG_RELATED_SOUND_FILE
           *    EXIF_TAG_INTEROPERABILITY_IFD_POINTER
           *    EXIF_TAG_FLASH_ENERGY
           *    EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE
           *    EXIF_TAG_FOCAL_PLANE_X_RESOLUTION
           *    EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION
           *    EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT
           *    EXIF_TAG_SUBJECT_LOCATION
           *    EXIF_TAG_EXPOSURE_INDEX
           *    EXIF_TAG_SENSING_METHOD
           *    EXIF_TAG_FILE_SOURCE
           *    EXIF_TAG_SCENE_TYPE
           *    EXIF_TAG_NEW_CFA_PATTERN
           *    EXIF_TAG_CUSTOM_RENDERED
           *    EXIF_TAG_EXPOSURE_MODE
           *    EXIF_TAG_WHITE_BALANCE
           *  ! EXIF_TAG_DIGITAL_ZOOM_RATIO
           *    EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM
           *    EXIF_TAG_SCENE_CAPTURE_TYPE
           *    EXIF_TAG_GAIN_CONTROL
           *    EXIF_TAG_CONTRAST
           *    EXIF_TAG_SATURATION
           *    EXIF_TAG_SHARPNESS
           *    EXIF_TAG_DEVICE_SETTING_DESCRIPTION
           *    EXIF_TAG_SUBJECT_DISTANCE_RANGE
           *    EXIF_TAG_IMAGE_UNIQUE_ID
           *    EXIF_TAG_GAMMA
           *    EXIF_TAG_PRINT_IMAGE_MATCHING
           *    EXIF_TAG_PADDING
           * ==== EXIF_IFD_GPS ===
           * GPS tags overlap with above ones, so use (?)
           *                    entry = exif_content_get_entry( edata->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE )
           *         NOT macro: entry = exif_data_get_entry( edata, EXIF_TAG_GPS_LATITUDE )
           * -------------------------------
           *    EXIF_TAG_GPS_VERSION_ID
           *    EXIF_TAG_GPS_LATITUDE_REF   === INTEROPERABILITY_INDEX   /// North/South
           *  ! EXIF_TAG_GPS_LATITUDE       === INTEROPERABILITY_VERSION
           *    EXIF_TAG_GPS_LONGITUDE_REF      /// East/West
           *  ! EXIF_TAG_GPS_LONGITUDE
           *    EXIF_TAG_GPS_ALTITUDE_REF      /// Above Sea Level
           *  ! EXIF_TAG_GPS_ALTITUDE
           *    EXIF_TAG_GPS_TIME_STAMP
           *  ! EXIF_TAG_GPS_SATELLITES
           *    EXIF_TAG_GPS_STATUS
           *    EXIF_TAG_GPS_MEASURE_MODE
           *    EXIF_TAG_GPS_DOP
           *    EXIF_TAG_GPS_SPEED_REF
           *  ! EXIF_TAG_GPS_SPEED
           *    EXIF_TAG_GPS_TRACK_REF
           *    EXIF_TAG_GPS_TRACK
           *    EXIF_TAG_GPS_IMG_DIRECTION_REF      /// Magnetic North
           *  ! EXIF_TAG_GPS_IMG_DIRECTION      /// 333
           *    EXIF_TAG_GPS_MAP_DATUM
           *    EXIF_TAG_GPS_DEST_LATITUDE_REF
           *    EXIF_TAG_GPS_DEST_LATITUDE
           *    EXIF_TAG_GPS_DEST_LONGITUDE_REF
           *    EXIF_TAG_GPS_DEST_LONGITUDE
           *    EXIF_TAG_GPS_DEST_BEARING_REF
           *    EXIF_TAG_GPS_DEST_BEARING
           *    EXIF_TAG_GPS_DEST_DISTANCE_REF
           *    EXIF_TAG_GPS_DEST_DISTANCE
           *    EXIF_TAG_GPS_PROCESSING_METHOD
           *    EXIF_TAG_GPS_AREA_INFORMATION
           *    EXIF_TAG_GPS_DATE_STAMP
           *    EXIF_TAG_GPS_DIFFERENTIAL
           */
          {
            ExifEntry *entry = NULL;
            char tmodel[1024] = "";

#if 0
            entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_MODEL );
#else
            entry = exif_data_get_entry( edata, EXIF_TAG_MODEL );
#endif
            if ( !entry )
              DUF_MAKE_ERROR( r, DUF_ERROR_EXIF_NO_MODEL );
            if ( r >= 0 && !exif_entry_get_value( entry, tmodel, sizeof( tmodel ) ) )
              DUF_MAKE_ERROR( r, DUF_ERROR_EXIF_NO_MODEL );
            if ( *tmodel )
            {
              mas_chomp( tmodel );
              model = mas_strdup( tmodel );
            }
          }

          timeepoch = duf_exif_get_time( edata, &date_changed, stime_original, sizeof( stime_original ), &r );
          /* DUF_SHOW_ERROR( "@@@@@@@@@@@@@@ %lu - %lu", sum, timeepoch ); */
          if ( DUF_CLEARED_ERROR( r, DUF_ERROR_EXIF_BROKEN_DATE ) && ( timeepoch || *stime_original || model ) )
          {
            unsigned long long exifid = 0;

#ifdef MAS_TRACING
            const char *real_path = NULL;
#endif

#ifdef MAS_TRACING
            DUF_SFIELD2( filename );
            real_path = duf_levinfo_path( pdi );
#endif
            DUF_TRACE( exif, 2, "%s%s", real_path, filename );

            exifid = duf_insert_exif_uni( pstmt, pdi, model, timeepoch, date_changed, stime_original, 1 /* need_id */ , &r );
            DUF_TRACE( exif, 1, "ID:%llu; (%d) read %lu m:%s t:%lu; %s%s", exifid, r, sum, model, timeepoch, real_path, filename );

            DUF_TRACE( exif, 3, "exifid:%llu; dataid:%llu; model:'%s'; datetime:%ld", exifid, dataid, model, ( long ) timeepoch );

            if ( r >= 0 && exifid && !duf_config->cli.disable.flag.update )
            {
              int changes = 0;

              if ( 1 )
              {
                const char *sql = " UPDATE " DUF_DBPREF " filedatas SET exifid = :exifID WHERE " DUF_SQL_IDNAME " = :dataID ";

                DUF_SQL_START_STMT( pdi, update_exif, sql, r, pstmt_update );
                DUF_TRACE( mod, 3, " S: %s ", sql );
                DUF_SQL_BIND_LL( exifID, exifid, r, pstmt_update );
                DUF_SQL_BIND_LL( dataID, dataid, r, pstmt_update );
                DUF_SQL_STEP( r, pstmt_update );
                /* DUF_TEST_R(r); */
                DUF_SQL_CHANGES( changes, r, pstmt_update );
                DUF_SQL_END_STMT( update_exif, r, pstmt_update );
              }
              else
              {
                DOR( r, duf_sql( " UPDATE " DUF_DBPREF " filedatas SET exifid = %llu WHERE " DUF_SQL_IDNAME " = %lld", &changes, exifid, dataid ) );
                duf_pdi_reg_changes( pdi, changes );
              }


              DUF_TEST_R( r );
            }
          }
          else
          {
            DUF_TRACE( exif, 3, "Nothing got for EXIF : (%d)", r );
          }
          DUF_CLEAR_ERROR( r, DUF_ERROR_EXIF_NO_MODEL );

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
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
    DUF_TEST_R( r );
  }
  DUF_CLEAR_ERROR( r, DUF_ERROR_EXIF_NO_DATE );
  DEBUG_ENDR( r );
}
