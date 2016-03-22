/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/*  #include "duf_tracen_defs.h"  (*  T; TT; TR ♠  *) */
/*  #include "duf_errorn_defs.h"  (*  DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠  *) */

/*  #include "duf_start_end.h"  (*  DUF_STARTR ; DUF_ENDR ♠  *) */
/* #include "duf_dodefs.h" (* DOR ♠ *) */

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ########################################################################################## */
#include "mod_exif.h"
/* ########################################################################################## */

/*
TODO
  1. good-date : 
     choose from 
       a. Date and Time (Original)  EXIF_TAG_DATE_TIME_ORIGINAL
       b. Date and Time (Digitized) EXIF_TAG_DATE_TIME_DIGITIZED
  2. Make difference by EXIF_TAG_SOFTWARE
       a. digiKam-2.4.1
       b. ....
*/

/* ########################################################################################## */
static int duf_dirent_contnt2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */
#define FILTER_DATA "(fd.noexif IS NULL AND fd.noexif IS NULL)"

static duf_sql_sequence_t final_sql = {
  .name = "final-exif",
  .done = 0,.sql = {
                    "UPDATE " DUF_SQL_TABLES_EXIF_FULL " SET dupexifcnt=(SELECT COUNT(*) " /* */
                    " FROM " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd " /* */
                    " JOIN " DUF_SQL_TABLES_EXIF_FULL " AS x ON (fd.exifid=x." DUF_SQL_IDFIELD ") " /* */
                    " WHERE exif." DUF_SQL_IDFIELD "=x." DUF_SQL_IDFIELD " AND fixed IS NULL ) WHERE fixed IS NULL" /* */
                    , NULL}
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_exif_callbacks = {
  .title = "collect exif",                                           /* */
  .name = "exif",                                                    /* */
  .def_opendir = 1,                                                  /* */
  .leaf_scan_fd2 = duf_dirent_contnt2,                               /* */
/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
/* filename for debug only */
  .leaf = {
           .name = "exif-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#exif",
           .selector2 =
#if 1
           "#std-ns-fd-leaf"                                         /* */
#else
           " FROM " /* */ DUF_SQL_TABLES_FILENAMES_FULL /*    */ " AS fn " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_FILEDATAS_FULL /*    */ " AS fd ON ( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_MD5_FULL /*          */ " AS md ON ( fd.md5id  = md." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_MIME_FULL /*         */ " AS mi ON ( fd.mimeid = mi." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_EXIF_FULL /*         */ " AS x  ON ( fd.exifid =  x." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_EXIF_MODEL_FULL /*   */ " AS xm ON ( x.modelid = xm." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN  " DUF_SQL_TABLES_SIZES_FULL /*        */ " AS sz ON ( sz.size   = fd.size)" /* */
#endif
           ,
           .matcher = " fn.Pathid = :parentdirID "                   /* */
           ,                                                         /* */
           .filter =                                                 /* */
           " sz.size > 0 " /*                                                                       */ " AND " /* */
           " ( " FILTER_DATA /* " OR x.modelid IS NULL " */ "  ) " /*                               */ " AND" /* */
           " ( mi.mime   IS NULL OR mi.mime='image/jpeg' ) " /*                                     */ " AND" /* */
           " ( :fFast    IS NULL OR sz.size IS NULL  OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                                                     /* */
           ,                                                         /* */
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD},       /* */
  .node = {
           .name = "exif-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'exif-node' AS fieldset_id, " (* *) */
           "pt." DUF_SQL_IDFIELD " AS dirid"                         /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs "                 /* */
           ", (" DUF_SQL__RNUMFILES( pt ) " WHERE " FILTER_DATA ") AS rnfiles " /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
           ", STRFTIME( '%s', pt.mtim ) AS mtime "                   /* */
           ,                                                         /* */
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = "pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " AND (tf.numfiles>0 OR td.numdirs>0 ) "                  /* */
#endif
           ,                                                         /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           .filter = " rnfiles > 0 "                                 /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static
SRP( MOD, unsigned long long, modelid, 0, insert_model_uni, duf_depthinfo_t * pdi, const char *model, int need_id )
{
/* int lr = 0; */

/* DUF_STARTULL( modelid ); */

  if ( model && *model && !duf_get_config_flag_disable_insert(  ) )
  {
    int changes = 0;

    if ( need_id )
    {
      const char *sql = "SELECT " DUF_SQL_IDFIELD " AS modelid FROM " DUF_SQL_TABLES_EXIF_MODEL_FULL " WHERE model=:Model";

      DUF_SQL_SE_START_STMT( pdi, select_model, sql, pstmt );
/* DUF_TEST_R( lr ); */
      DUF_SQL_SE_BIND_S( Model, model, pstmt );
/* DUF_TEST_R( lr ); */
      DUF_SQL_SE_STEP( pstmt );
    /* DUF_TEST_R( lr ); */
      if ( QISERR1_N( SQL_ROW ) )
      {
        MAST_TRACE( select, 0, "<selected>" );
#if 0
        modelid = duf_sql_column_long_long( pstmt, 0 );
#else
        modelid = DUF_GET_UFIELD2( modelid );
#endif
      /* lr = 0; */
      }
    /* if ( QISERR1_N(SQL_DONE ) ) */
    /*   lr = 0;                                 */
    /* DUF_TEST_R( lr ); */
      DUF_SQL_SE_END_STMT( pdi, select_model, pstmt );
    }

    if ( !modelid )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_EXIF_MODEL_FULL " ( model ) VALUES ( :Model )";

      DUF_SQL_SE_START_STMT( pdi, insert_model, sql, pstmt_insert );
/* DUF_TEST_R( lr ); */
      MAST_TRACE( insert, 0, " S: %s ", sql );
      DUF_SQL_SE_BIND_S( Model, model, pstmt_insert );
/* DUF_TEST_R( lr ); */

      DUF_SQL_SE_STEP( pstmt_insert );
    /* DUF_TEST_R( lr ); */
      DUF_SQL_SE_CHANGES( changes, pstmt_insert );
    /* DUF_TEST_R( lr ); */
    /* DUF_SHOW_ERRORO( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        modelid = duf_sql_last_insert_rowid(  );
        MAST_TRACE( exif, 0, " inserted now( SQLITE_OK ) modelid = %llu ", modelid );
      }
      DUF_SQL_SE_END_STMT( pdi, insert_model, pstmt_insert );
    }
/* DUF_TEST_R( lr ); */
  }
  else
  {
  /* No model is not soooo bad! */
  /* DUF_SHOW_ERRORO( " Wrong data " ); */
  /* ERRMAKE(DATA);          */
  }
/* DUF_TEST_R( lr ); */

/* if ( pr ) */
/* *pr = lr; */
/* DUF_TEST_R( lr ); */
/* assert( modelid ); */
/* DUF_ENDULL( modelid ); */
  ERP( MOD, unsigned long long, modelid, 0, insert_model_uni, duf_depthinfo_t * pdi, const char *model, int need_id );
}

static
SRP( MOD, unsigned long long, exifid, 0, insert_exif_uni, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi, const char *model, time_t timeepoch,
     int dtfixed, const char *stime_original, int need_id )
{
/* int lr = 0; */
  unsigned long long modelid = 0;

/* DUF_STARTULL( exifid ); */
  modelid = duf_insert_model_uni( pdi, model, 1 /*need_id */ , QPERRIND );
  if ( QNOERR && ( timeepoch || modelid || dtfixed || stime_original ) && !duf_get_config_flag_disable_insert(  ) )
  {
    if ( need_id )
    {
      const char *sql = NULL;

      sql = "SELECT " DUF_SQL_IDFIELD " AS exifid FROM " DUF_SQL_TABLES_EXIF_FULL
              " WHERE ( :modelID IS NULL OR modelid=:modelID ) AND date_time=datetime(:timeEpoch, 'unixepoch')";

      DUF_SQL_SE_START_STMT( pdi, select_exif, sql, pstmt );
/* DUF_TEST_R( lr ); */
      if ( modelid )
      {
      /* DUF_SQL_SE_BIND_LL( modelID, modelid,  pstmt ); */
        DUF_SQL_SE_BIND_LL_NZ_OPT( modelID, modelid, pstmt );
      /* DUF_TEST_R( lr ); */
      }
      DUF_SQL_SE_BIND_LL( timeEpoch, timeepoch, pstmt );
    /* DUF_TEST_R( lr ); */
      DUF_SQL_SE_STEP( pstmt );
    /* DUF_TEST_R( lr ); */
      if ( QISERR1_N( SQL_ROW ) )
      {
        MAST_TRACE( select, 0, "<selected>" );
#if 0
        exifid = duf_sql_column_long_long( pstmt, 0 );
#else
        exifid = DUF_GET_UFIELD2( exifid );
#endif
      /* lr = 0; */
      }
    /* DUF_CLEAR_ERROR( lr, DUF_SQL_DONE ); */
    /* DUF_TEST_R( lr ); */
      DUF_SQL_SE_END_STMT( pdi, select_exif, pstmt );
    /* if ( !exifid )                        */
    /*   DUF_SHOW_ERRORO( "exifid NOT SELECTED" ); */
    }

    if ( !exifid )
    {
      int changes = 0;
      const char *sql =
              "INSERT OR IGNORE INTO " DUF_SQL_TABLES_EXIF_FULL " ( modelid, date_time, broken_date, fixed ) "
              " VALUES ( :modelID, datetime(:timeEpoch, 'unixepoch'), :origTime, :dtFixed )";

      DUF_SQL_SE_START_STMT( pdi, insert_exif, sql, pstmt_insert );
/* DUF_TEST_R( lr ); */
      MAST_TRACE( insert, 0, " S: %s ", sql );
/* DUF_TEST_R( lr ); */
      if ( modelid )
      {
      /* DUF_SQL_SE_BIND_LL( modelID, modelid,  pstmt_insert ); */
        DUF_SQL_SE_BIND_LL_NZ_OPT( modelID, modelid, pstmt_insert );
/* DUF_TEST_R( lr ); */
      }
      DUF_SQL_SE_BIND_LL_NZ_OPT( timeEpoch, timeepoch, pstmt_insert );
/* DUF_TEST_R( lr ); */
      DUF_SQL_SE_BIND_LL_NZ_OPT( dtFixed, dtfixed, pstmt_insert );
/* DUF_TEST_R( lr ); */
      DUF_SQL_SE_BIND_S( origTime, stime_original, pstmt_insert );
/* DUF_TEST_R( lr ); */
    /* {                                                                                               */
    /*   const char *real_path = NULL;                                                                 */
    /*   static int c = 0;                                                                             */
    /*   static int n = 0;                                                                             */
    /*                                                                                                 */
    /*   if ( changes )                                                                                */
    /*     c++;                                                                                        */
    /*   n++;                                                                                          */
    /*                                                                                                 */
    /*   DUF_SFIELD2( fname );                                                                      */
    /*   real_path = duf_levinfo_path( pdi );                                                          */
    /*                                                                                                 */
    /*   DUF_SHOW_ERRORO( "%d. nchanges:%d %llu:%lu  %s%s", n, c, modelid, timeepoch, real_path, filename ); */
    /* }                                                                                               */
      DUF_SQL_SE_STEPC( pstmt_insert );
    /* DUF_TEST_R(lr); */
      DUF_SQL_SE_CHANGES( changes, pstmt_insert );
      if ( need_id && changes )
      {
        exifid = duf_sql_last_insert_rowid(  );
        assert( exifid );
      }
      if ( !changes )
      {
        QT( "exifid NOT INSERTED:%s - %llu, %lu, %s", sql, modelid, timeepoch, stime_original );
      /* TODO DUF_MAKE_ERROR */
      }

      {
#ifdef MAS_TRACING
        DUF_SFIELD2( fname );
#endif
        MAST_TRACE( exif, 1, " inserted now( SQLITE_OK ) exifid=%llu; modelid=%llu; %lu ; changes:%d; %s%s", exifid, modelid,
                    ( long ) timeepoch, changes, duf_levinfo_path( pdi ), fname );
      }

      DUF_SQL_SE_END_STMT( pdi, insert_exif, pstmt_insert );
    }
/* DUF_TEST_R( lr ); */
  }
  else
  {
  /* DUF_SHOW_ERRORO( " Wrong data " ); */
    ERRMAKE( DATA );
  }

/* DUF_TEST_R( lr ); */
/* if ( pr ) */
/* *pr = lr; */
/* DUF_ENDULL( exifid ); */
  ERP( MOD, unsigned long long, exifid, 0, insert_exif_uni, duf_stmnt_t * pstmt MAS_UNUSED, duf_depthinfo_t * pdi, const char *model,
       time_t timeepoch, int dtfixed, const char *stime_original, int need_id );
}

static time_t
duf_exif_get_time( ExifData * edata, int *pdate_changed, char *stime_original, size_t stime_original_size, int *pr )
{
  int lr = 0;
  time_t timeepoch = 0;
  ExifEntry *entry = NULL;
  int date_changed = 0;
  int date_is_broken = 0;

#if 0
  if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) )
/*                 || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) */
       || ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
#else
  if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_ORIGINAL ) ) || ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME ) ) )
#endif
  {
    const char *ptime_original = NULL;

    memset( stime_original, stime_original_size, 0 );
  /* Get the contents of the tag in human-readable form */
    if ( lr >= 0 )
      ptime_original = exif_entry_get_value( entry, stime_original, stime_original_size );
  /* ERRMAKE(EXIF ); */
    if ( ptime_original )
    {
      char *corrected_time = NULL;

      for ( unsigned i = 0; i < strlen( stime_original ); i++ )
      {
        char c;

        c = stime_original[i];
        if ( ( c < ' ' || c > 'z' ) && c != '?' && c != ':' && c != ' ' )
        {
          MAST_TRACE( exif, 0, ">>>>>>>>>>>>>> %s <<<<<<<<<<<<<", stime_original );
        /* ERRMAKE(EXIF_BROKEN_DATE ); */
          date_is_broken = 1;
          break;
        }
      }
      if ( lr >= 0 )
        corrected_time = mas_strdup( stime_original );
    /* DUF_SHOW_ERRORO( "@@@@@@@@@@@@@@ %s", stime_original ); */
      MAST_TRACE( exif, 3, "stime_original:%s", stime_original );
    /* 2008:06:21 13:18:19 */
    /* 0123:56:89 12:45:78 */
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
          date_changed++;
        }
      }
    /* if ( lr >= 0 && !*corrected_time )              */
    /*   ERRMAKE(EXIF_NO_DATE ); */
      else if ( lr >= 0 && strchr( corrected_time, '?' ) )
      {
      /* DUF_SHOW_ERRORO( "broken date %s", corrected_time ); */
      /* ERRMAKE_M(EXIF_BROKEN_DATE, "broken date %s", corrected_time ); */
        date_is_broken = 1;
      }
      if ( lr >= 0 || date_is_broken /* lr == DUF_ERROR_EXIF_BROKEN_DATE */  )
      {
        if ( corrected_time && *corrected_time )
        {
          struct tm times = { 0 };

          strptime( corrected_time, "%Y:%m:%d %H:%M:%S", &times );

        /* timeepoch = mktime( &times ); */

        /* timeepoch = timelocal( &times ); */
          timeepoch = timegm( &times );

        /* strftime( buf1, sizeof( buf1 ), "%Y-%m-%d %H:%M:%S", &times ); */
        /* DUF_SHOW_ERRORO( "!!!!! %lu : %lu", timeepoch, time(NULL) ); */
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
# define exif_data_get_entry(d, t) ( \
    exif_content_get_entry( d->ifd[EXIF_IFD_0], t ) ? \
            exif_content_get_entry( d->ifd[EXIF_IFD_0], t ) : \
            exif_content_get_entry( d->ifd[EXIF_IFD_1], t ) ? \
            exif_content_get_entry( d->ifd[EXIF_IFD_1], t ) : \
            exif_content_get_entry( d->ifd[EXIF_IFD_EXIF], t ) ? \
            exif_content_get_entry( d->ifd[EXIF_IFD_EXIF], t ) : \
            exif_content_get_entry( d->ifd[EXIF_IFD_GPS], t ) ? \
            exif_content_get_entry( d->ifd[EXIF_IFD_GPS], t ) : \
            exif_content_get_entry( d->ifd[EXIF_IFD_INTEROPERABILITY], t ) ? exif_content_get_entry( d->ifd[EXIF_IFD_INTEROPERABILITY], t ) : NULL )
#endif
  /* ERRMAKE(EXIF_NO_DATE ); */
#if 0
    if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
    {
      DUF_SHOW_ERRORO( "NO DATE - HAS +1" );
    }
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
    {
      DUF_SHOW_ERRORO( "NO DATE - HAS +2" );
    }
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
    {
      DUF_SHOW_ERRORO( "NO DATE - HAS +3" );
    }
#else
    if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_ORIGINAL ) ) )
    {
      QTT( "NO DATE - HAS +123" );
    }
#endif
#if 0
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +4" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +5" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +6" );
#else
    else if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME_DIGITIZED ) ) )
    {
      QTT( "NO DATE - HAS +456" );
    }
#endif

#if 0
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +7" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_1], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +8" );
    else if ( ( entry = exif_content_get_entry( edata->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME ) ) )
      DUF_SHOW_ERRORO( "NO DATE - HAS +9" );
#else
    else if ( ( entry = exif_data_get_entry( edata, EXIF_TAG_DATE_TIME ) ) )
    {
      QTT( "NO DATE - HAS +789" );
    }
#endif
    else
    {
      QTT( "NO DATE" );
    }
  /* ??? exif_entry_free( entry ); */
  }

  if ( pdate_changed )
    *pdate_changed = date_changed;
  if ( pr )
    *pr = lr;
  return timeepoch;
}

static
SR( MOD, dirent_contnt2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi , duf_sccb_handle_t *sccbh MAS_UNUSED)
{
/*   DUF_STARTR( r ) */ ;

  DUF_UFIELD2( dataid );

  {
    size_t bufsz = 1024;
    unsigned char *buffer;
    size_t sum = 0;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 2 /* FIXME ??? */ ;
      ExifLoader *loader;

      loader = exif_loader_new(  );

    /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
    /* exif_loader_write_file */
      while ( QNOERR && cnt++ < maxcnt )
      {
        int rr;

        rr = read( duf_levinfo_dfd( pdi ), buffer, bufsz );
        MAST_TRACE( exif, 5, "read %d", rr );
        if ( rr < 0 )
        {
        /* DUF_ERRSYS( "read file" ); */
          MASE_ERRSYS( "read file" );
          ERRMAKE( READ );
        }
/* DUF_TEST_R( r ); */

        if ( rr > 0 )
        {
          int rx = 0;

          sum += rr;
          rx = exif_loader_write( loader, buffer, rr );
          if ( rx != 1 )
            break;
        }
        if ( rr <= 0 )
          break;

/* DUF_TEST_R( r ); */
      }
#if 0
      DUF_CLEAR_ERROR( r, DUF_ERROR_EOF, DUF_ERROR_EXIF_END );
#endif
      if ( QNOERR )
      {
        ExifData *edata = NULL;

      /* ExifTag tag1 = EXIF_TAG_DATE_TIME;           */
      /* ExifTag tag2 = EXIF_TAG_DATE_TIME_DIGITIZED; */
      /* ExifTag tag3 = EXIF_TAG_DATE_TIME_ORIGINAL;  */

        edata = exif_loader_get_data( loader );
        exif_loader_unref( loader );
        if ( QNOERR /* && edata *//* && edata->size */  )
        {
        /* ??? exif_entry_free( entry ); */
          int date_changed = 0;

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
            time_t timeepoch = 0;

            char stime_original[1024] = {
              '\0'
            };
            char *model = NULL;

            {
              ExifEntry *entry = NULL;

              if ( edata )
              {

#if 0
                entry = exif_content_get_entry( edata->ifd[EXIF_IFD_0], EXIF_TAG_MODEL );
#else
                entry = exif_data_get_entry( edata, EXIF_TAG_MODEL );
#endif
                if ( entry )
                {
                  char tmodel[1024] = "";
                  const char *pmodel = NULL;

                  pmodel = exif_entry_get_value( entry, tmodel, sizeof( tmodel ) );
                  if ( pmodel )
                  {
                    model = mas_strndup( tmodel, mas_chomplen( tmodel ) );
                  }
                }
                timeepoch = duf_exif_get_time( edata, &date_changed, stime_original, sizeof( stime_original ), QPERRIND );
              }
            }
          /* DUF_SHOW_ERRORO( "@@@@@@@@@@@@@@ %lu - %lu", sum, timeepoch ); */
            if (  /* DUF_CLEARED_ERROR( r, DUF_ERROR_EXIF_BROKEN_DATE ) && */ QNOERR /* && ( timeepoch || *stime_original || model ) ::20151024.125417:: insert null's */
                 && !duf_get_config_flag_disable_update(  ) )
            {
              unsigned long long exifid = 0;
              unsigned noexif = 0;

#ifdef MAS_TRACING
              const char *real_path = NULL;

              DUF_SFIELD2( fname );
              real_path = duf_levinfo_path( pdi );
              MAST_TRACE( exif, 2, "%s%s", real_path, fname );
#endif
              noexif = !( timeepoch || *stime_original || model );

              if ( !noexif )
                exifid = duf_insert_exif_uni( pstmt, pdi, model, timeepoch, date_changed, stime_original, 1 /* need_id */ , QPERRIND );
              MAST_TRACE( exif, 1, "ID:%llu; (%d) read %lu m:%s t:%lu; %s%s", exifid, QERRIND, sum, model, timeepoch, real_path, fname );

              MAST_TRACE( exif, 3, "exifid:%llu; dataid:%llu; model:'%s'; datetime:%ld", exifid, dataid, model, ( long ) timeepoch );

              if ( QNOERR /* && exifid */  )
              {
                int changes = 0;
                const char *sql =
                        " UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET noexif = :noexif, exifid = :exifID WHERE " DUF_SQL_IDFIELD " = :dataID ";

                DUF_SQL_SE_START_STMT( pdi, update_exif, sql, pstmt_update );
                MAST_TRACE( mod, 3, " S: %s ", sql );
              /* DUF_SQL_SE_BIND_LL( exifID, exifid,  pstmt_update ); */
                DUF_SQL_SE_BIND_LL_NZ_OPT( exifID, exifid, pstmt_update );
              /* DUF_SQL_SE_BIND_LL( noexif, noexif,  pstmt_update ); */
                DUF_SQL_SE_BIND_LL_NZ_OPT( noexif, noexif, pstmt_update );
                DUF_SQL_SE_BIND_LL( dataID, dataid, pstmt_update );
                DUF_SQL_SE_STEPC( pstmt_update );
              /* DUF_TEST_R(r); */
                DUF_SQL_SE_CHANGES( changes, pstmt_update );
                DUF_SQL_SE_END_STMT( pdi, update_exif, pstmt_update );
              }
            }
            else
            {
              MAST_TRACE( exif, 3, "Nothing got for EXIF : (%d)", QERRIND );
              assert( 0 );
            }
            mas_free( model );
            model = NULL;
          }

        /* sleep(1); */

        /* ??? exif_entry_free( entry ); */
/* DUF_TEST_R( r ); */

        /* edata = exif_data_new_from_file( filepath ); */
          if ( edata )
          {
            exif_data_free( edata );
          /* exif_loader_unref( edata ); */
            edata = NULL;
          }
        }
      }
      mas_free( buffer );
      buffer = NULL;
/* DUF_TEST_R( r ); */
    }
    else
    {
      ERRMAKE( MEMORY );
    }
/* DUF_TEST_R( r ); */
  }
/* DUF_CLEAR_ERROR( r, DUF_ERROR_EXIF_NO_DATE, DUF_ERROR_EXIF_NO_MODEL ); */

  pdi->total_files++;
/*  DUF_ENDR( r );*/
  ER( MOD, dirent_contnt2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi , duf_sccb_handle_t *sccbh MAS_UNUSED);
}
