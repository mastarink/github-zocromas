#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

/* #include <dirent.h> */
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"


#include "duf_path.h"

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

#include "duf_dbg.h"

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

  DUF_TRACE( scan, 2, "  L%u: scan 5       =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );


  DUF_PRINTF( 0, "%6llu:%6llu:%6llu [%10llu] file: %s", pdi->seq, pdi->items.files, pdi->items.total, filesize, filename );

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

  DUF_TRACE( scan, 2, "  L%u: scan 5       =>           - %s", duf_pdi_depth( pdi ), duf_levinfo_itemname( pdi ) );

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
        .truepath = 1,
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
      duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL );
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

  {
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
      duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL );
      DUF_PUTSL( 0 );
    }


    /* DUF_PRINTF( 0, "------------   =%-2llu    [%10llu]  md5: %s", nsame, maxsize, dirname ); */


    /* mas_free( path ); */
  }
  DEBUG_ENDR( r );
  return r;
}

/* #### duf_sql( "UPDATE duf_md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id ); */

  /* .node_selector =                                                                                             */
  /*       "SELECT duf_md5.id AS dirid, printf('%%016x%%016x',md5sum1,md5sum2) AS dirname "                      */
  /*       " ,0 AS ndirs" */
  /* " ,(SELECT count(*) FROM duf_filenames AS subfilenames "                               */
  /*       (* " LEFT "  toooooooo slow with LEFT *)                                                              */
  /*       "            JOIN duf_filedatas ON (duf_filedatas.id=subfilenames.dataid) "                           */
  /*       "                            WHERE duf_filedatas.md5id=duf_md5.id) AS nfiles "                        */
  /*       (* ", (SELECT count(*) FROM duf_filedatas "                                                        *) */
  /*       (* "           LEFT JOIN duf_filenames AS subfilenames ON (duf_filedatas.id=subfilenames.dataid) " *) */
  /*       (* "                             WHERE duf_filedatas.md5id=duf_md5.id) AS nfiles"                  *) */
  /*       " FROM duf_md5 " */
  /* " WHERE %llu<1 "                                                                     */
  /*       (* " AND nfiles>1 " *)                                                                                */
  /*       " ORDER BY md5sum1,md5sum2 ",                                                                         */


duf_scan_callbacks_t duf_print_md5_callbacks = {
  .title = __FILE__,
  /* .opendir = 1, */
  .scan_mode_step = 1,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .node_scan_before2 = scan_node_before2,
  .leaf_scan = scan_leaf,
  .leaf_scan2 = scan_leaf2,
  .fieldset = " md.id AS dirid, md.id AS md5id " /* */
        ", fn.Pathid as truedirid " /*	*/
	", fn.name AS filename, fd.size AS filesize " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", dupcnt AS nsame "    /* */
        ", printf('%016x%016x',md5sum1,md5sum2) AS dirname " /* */
        ", fn.Pathid AS hid "   /* */
        ", fn.id AS filenameid" /* */
        ", fd.id AS dataid " /* */
        ", fd.mode AS filemode" /* */
        ,
  .leaf_selector = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fd.id=fn.dataid) " /* */
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /* */
        " WHERE fd.md5id='%llu' ORDER BY fd.size " /* */
        ,
  .leaf_selector2 = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fd.id=fn.dataid) " /* */
        " LEFT JOIN duf_md5 AS md ON (md.id=fd.md5id)" /* */
        " WHERE "               /* */
        " (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) AND " /* */
        " (:minsame IS NULL OR md.dupcnt>=:minsame) AND (:maxsame IS NULL OR md.dupcnt<=:maxsame) AND " /* */
        " fd.md5id=:dirid "     /* */
        " ORDER BY fd.size "    /* */
        ,
  .node_selector = "SELECT md.id AS dirid, md.id AS md5id " /* */
        ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dirname" /* */
        ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dfname " /* */
        ", 0 AS ndirs "         /* */
        ", dupcnt AS nfiles "   /* */
        ", (SELECT size FROM duf_filedatas AS fd WHERE md.id=fd.md5id LIMIT 1) AS filesize " /* */
        ", 0 AS maxsize, 0 AS minsize" /* */
        " FROM duf_md5 AS md "  /* */
        "  WHERE %llu<1 "       /* */
        "  ORDER BY filesize, md.id " /* */
        ,
  .node_selector2 = "SELECT md.id AS dirid, md.id AS md5id " /* */
        ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dirname" /* */
        ", printf('%%016x%%016x',md.md5sum1,md.md5sum2) AS dfname " /* */
        ", md.md5sum1, md.md5sum2 " /* */
        ", 0 AS ndirs "         /* */
        ", dupcnt AS nfiles, dupcnt AS nsame " /* */
        /* ", (SELECT size FROM duf_filedatas AS fd WHERE md.id=fd.md5id LIMIT 1) AS filesize" (* *) */
        ", fd.size AS maxsize, fd.size AS minsize" /* */
        "  FROM duf_md5 AS md"  /* */
        "  LEFT JOIN duf_filedatas AS fd ON (md.id=fd.md5id) " /*	*/
	"  WHERE " /* */
        " (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) AND " /* */
        " (:minsame IS NULL OR md.dupcnt>=:minsame) AND (:maxsame IS NULL OR md.dupcnt<=:maxsame) AND " /* */
        " :dirid<1 "            /* */
        "  GROUP BY md.id " /*	*/
	"  ORDER BY fd.size, md.id " /* */
        ,

  /* .final_sql_argv = final_sql, */
};
