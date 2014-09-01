#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"



#include "duf_service.h"
#include "duf_config_ref.h"




#include "duf_sql_defs.h"
#include "duf_sql_field.h"
/* #include "duf_sql_field1.h" */
#include "duf_sql_field2.h"


/* ###################################################################### */
/* #include "duf_print_md5_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */

  DEBUG_START(  );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --print --md5  --files   -FF
   *                   ^^^^^^^        ^^^^^^^
   * */

  /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5       =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) ); */


  DUF_PRINTF( 0, "%6llu:%6llu:%6llu [%10llu] file: %s", pdi->seq, pdi->items.files, pdi->items.total, filesize, filename );
  /* DUF_PRINTF( 0, "%6llu:%6llu:%6llu [%10llu] file: %s", pdi->seq, duf_levinfo_items_files( pdi ), pdi->items.total, filesize, filename ); */

  DEBUG_ENDR( r );
  return r;
}

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_SFIELD2( filename );
  /* DUF_UFIELD2( filesize ); */

  DEBUG_START(  );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --print --md5  --files   -FF
   *                   ^^^^^^^        ^^^^^^^
   * */

  /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5       =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) ); */

  {

    DUF_UFIELD2( truedirid );
    /* DUF_UFIELD2( filesize ); */
    DUF_UFIELD2( filemode );
    /* DUF_UFIELD2( md5id ); */
    /* DUF_UFIELD2( md5sum1 ); */
    /* DUF_UFIELD2( md5sum2 ); */
    DUF_UFIELD2( mtime );
    DUF_UFIELD2( uid );
    DUF_UFIELD2( gid );
    DUF_UFIELD2( nlink );
    DUF_UFIELD2( inode );
    /* DUF_UFIELD2( nsame ); */

    {
      duf_fileinfo_t fi = { 0 };

      duf_format_t format = {
        .filename = 1,
        .seq = 1,
        .dirid = 0,
        .inode = 0,
        .mode = 1,
        .nlink = 1,
        .uid = 1,
        .gid = 1,
        .filesize = 0,
        .md5 = 0,
        .md5id = 0,
        .mtime = 1,
        .nsame = 0,
        .prefix = 0,
        .suffix = 0,
      };
      /* fi.nsame = nsame; */
      fi.st.st_mode = ( mode_t ) filemode;
      fi.st.st_ino = ( ino_t ) inode;
      fi.st.st_mtim.tv_sec = mtime;
      fi.st.st_mtim.tv_nsec = 0;
      fi.st.st_uid = ( uid_t ) uid;
      fi.st.st_gid = ( gid_t ) gid;
      fi.st.st_nlink = ( nlink_t ) nlink;
      /* fi.st.st_size = ( off_t ) maxsize; */
      fi.name = filename;
      fi.truedirid = truedirid;
      /* fi.md5id = md5id; */
      /* fi.md5sum1 = md5sum1; */
      /* fi.md5sum2 = md5sum2; */
      duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );
      DUF_PUTSL( 0 );
    }
    /* DUF_PRINTF( 0, "%6llu:%6llu:%6llu %s%s", pdi->seq, pdi->items.files, pdi->items.total, true_path, filename ); */
  }
  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* __attribute__ ( ( unused ) ) */
static int
scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( dirname );

  DEBUG_START(  );

  {
    /* char *path = duf_pathid_to_path_s( pathid_unused ); */


    DUF_PRINTF( 0, "------------------------------------- md5: %s", dirname );


    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DUF_SFIELD2( dirname );
  DUF_UFIELD2( maxsize );
  DUF_UFIELD2( nsame );
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( md5sum1 );
  DUF_UFIELD2( md5sum2 );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  DEBUG_START(  );
  if ( pdi->items.files > 0 )
  {
    duf_fileinfo_t fi = { 0 };

    duf_format_t format = {
      .filename = 0,
      .seq = 1,
      .dirid = 0,
      .inode = 0,
      .mode = 1,
      .nlink = 1,
      .uid = 1,
      .gid = 1,
      .filesize = 1,
      .md5 = 1,
      .md5id = 1,
      .mtime = 1,
      .nsame = 1,
      .prefix = 1,
      .suffix = 0,
    };
    fi.nsame = nsame;
    /* fi.st.st_mode = ( mode_t ) filemode; */
    /* fi.st.st_ino = ( ino_t ) inode;      */
    /* fi.st.st_mtim.tv_sec = mtime;        */
    /* fi.st.st_mtim.tv_nsec = 0;           */
    /* fi.st.st_uid = ( uid_t ) uid;        */
    /* fi.st.st_gid = ( gid_t ) gid;        */
    /* fi.st.st_nlink = ( nlink_t ) nlink;  */
    fi.st.st_size = ( off_t ) maxsize;
    fi.name = dirname;
    fi.md5id = md5id;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;
    duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL, ( duf_pdi_cb_t ) NULL );
    DUF_PUTS( 0, " >--------------------------" );


    /* DUF_PRINTF( 0, "------------   =%-2llu    [%10llu]  md5: %s", nsame, maxsize, dirname ); */


    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

/* #### duf_sql( "UPDATE " DUF_DBPREF "md5 SET dup5cnt='%llu' WHERE " DUF_SQL_IDNAME "='%llu'", cnt, md5id ); */

  /* .node.selector =                                                                                             */
  /*       "SELECT " DUF_DBPREF "md5." DUF_SQL_IDNAME " AS dirid, printf('%%016x%%016x',md5sum1,md5sum2) AS dirname "                      */
  /*       " ,0 AS ndirs" */
  /* " ,(SELECT count(*) FROM " DUF_DBPREF "filenames AS subfilenames "                               */
  /*       (* " LEFT "  toooooooo slow with LEFT *)                                                              */
  /*       "            JOIN " DUF_DBPREF "filedatas ON (" DUF_DBPREF "filedatas." DUF_SQL_IDNAME "=subfilenames.dataid) "                           */
  /*       "                            WHERE " DUF_DBPREF "filedatas.md5id=" DUF_DBPREF "md5." DUF_SQL_IDNAME ") AS nfiles "                        */
  /*       (* ", (SELECT count(*) FROM " DUF_DBPREF "filedatas "                                                        *) */
  /*       (* "           LEFT JOIN " DUF_DBPREF "filenames AS subfilenames ON (" DUF_DBPREF "filedatas." DUF_SQL_IDNAME "=subfilenames.dataid) " *) */
  /*       (* "                             WHERE " DUF_DBPREF "filedatas.md5id=" DUF_DBPREF "md5." DUF_SQL_IDNAME ") AS nfiles"                  *) */
  /*       " FROM " DUF_DBPREF "md5 " */
  /* " WHERE %llu<1 "                                                                     */
  /*       (* " AND nfiles>1 " *)                                                                                */
  /*       " ORDER BY md5sum1,md5sum2 ",                                                                         */


duf_scan_callbacks_t duf_print_md5_callbacks = {
  .title = __FILE__,
  /* .def_opendir = 1, */
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .node_scan_before2 = scan_node_before2,
  .leaf_scan = scan_leaf,
  .leaf_scan2 = scan_leaf2,
  .leaf = {.fieldset = "md." DUF_SQL_IDNAME " AS dirid, md." DUF_SQL_IDNAME " AS md5id " /* */
           ", fn.Pathid as truedirid " /* */
           ", fn.name AS filename, fd.size AS filesize " /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", dup5cnt AS nsame " /* */
           ", printf('%016x%016x',md5sum1,md5sum2) AS dirname " /* */
           ", fn.Pathid AS hid " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid" /* */
           ", fd." DUF_SQL_IDNAME " AS dataid " /* */
           ", fd.mode AS filemode" /* */
           ", fd.md5id AS md5id" /* */
           ,
           /* .selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " (* *)       */
           /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fd." DUF_SQL_IDNAME "=fn.dataid) " (* *) */
           /*       " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" (* *)         */
           /*       " WHERE fd.md5id='%llu' ORDER BY fd.size " (* *)                       */
           /*       ,                                                                      */
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fd." DUF_SQL_IDNAME "=fn.dataid) " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " WHERE "            /* */
           " fd.md5id=:dirID "  /* */
           " ORDER BY fd.size " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           }
  ,
  .node = {.fieldset = "md." DUF_SQL_IDNAME " AS dirid, md." DUF_SQL_IDNAME " AS md5id " /* */
           ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dirname" /* */
           ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dfname " /* */
           ", 0 AS ndirs "      /* */
           ", dup5cnt AS nfiles, dup5cnt AS nsame " /* */
           /* ", (SELECT size FROM " DUF_DBPREF "filedatas AS fd WHERE md." DUF_SQL_IDNAME "=fd.md5id LIMIT 1) AS filesize " (* *) */
           ", fd.size AS maxsize, fd.size AS minsize" /* */
           ,
           /* .selector = "SELECT md." DUF_SQL_IDNAME " AS dirid, md." DUF_SQL_IDNAME " AS md5id " (* *)                                            */
           /*       ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dirname" (* *)                                    */
           /*       ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dfname " (* *)                                    */
           /*       ", 0 AS ndirs "         (* *)                                                                        */
           /*       ", dup5cnt AS nfiles "   (* *)                                                                       */
           /*       ", (SELECT size FROM " DUF_DBPREF "filedatas AS fd WHERE md." DUF_SQL_IDNAME "=fd.md5id LIMIT 1) AS filesize " (* *) */
           /*       ", 0 AS maxsize, 0 AS minsize" (* *)                                                                 */
           /*       " FROM " DUF_DBPREF "md5 AS md " (* *)                                                               */
           /*       "  WHERE %llu<1 "       (* *)                                                                        */
           /*       "  ORDER BY filesize, md." DUF_SQL_IDNAME " " (* *)                                                                  */
           /*       ,                                                                                                    */
           .selector2 =         /* */
           /* "SELECT md." DUF_SQL_IDNAME " AS dirid, md." DUF_SQL_IDNAME " AS md5id "                    */
           /* ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dirname" */
           /* ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dfname " */
           /* ", md.md5sum1, md.md5sum2 "                                 */
           /* ", 0 AS ndirs "                                             */
           /* ", dup5cnt AS nfiles, dup5cnt AS nsame "                      */
           /* ", fd.size AS maxsize, fd.size AS minsize"                  */
           "  FROM " DUF_DBPREF "md5 AS md" /* */
           "  LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
           "  WHERE "           /* */
           " (:minSize IS NULL OR fd.size>=:minSize) AND (:maxSize IS NULL OR fd.size<=:maxSize) AND " /* */
           " (:minSame IS NULL OR md.dup5cnt>=:minSame) AND (:maxSame IS NULL OR md.dup5cnt<=:maxSame) AND " /* */
           " :dirID<1 "         /* */
           "  GROUP BY md." DUF_SQL_IDNAME " " /* */
           "  ORDER BY fd.size, md." DUF_SQL_IDNAME " " /* */
           }

  /* , .final_sql_argv = final_sql, */
};
