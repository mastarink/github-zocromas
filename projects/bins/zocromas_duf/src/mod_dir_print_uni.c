#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>
#include <time.h>

#include <assert.h>
#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_levinfo.h"

/* #include "duf_sql_def.h" */
#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_path.h"


#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_dir_print_uni.h" */
/* ###################################################################### */




/* callback of  duf_scan_hook_file_t */
static int
scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_unused */  )
{
  int r = 0;

  DEBUG_START(  );

  /* SQL at duf_scan_files_by_dirid */
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */
  DUF_UFIELD( filemode );
  /* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); */
  DUF_UFIELD( md5sum1 );
  /* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); */
  DUF_UFIELD( md5sum2 );
  /* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); */
  DUF_UFIELD( mtime );
  DUF_UFIELD( uid );
  DUF_UFIELD( gid );
  DUF_UFIELD( nlink );
  DUF_UFIELD( inode );
  DUF_UFIELD( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  /* if (nsame<2) return r; */

  /* DUF_PRINTF(0, ".> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* DUF_PRINTF(0, ".-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  {
    duf_fileinfo_t fi;

    duf_format_t format = {
      .filename = 1,
      .seq = 1,
      .dirid = 1,
      .inode = 1,
      .mode = 1,
      .nlink = 1,
      .uid = 1,
      .gid = 1,
      .filesize = 1,
      .md5 = 1,
      .mtime = 1,
      .nsame = 1,
    };
    fi.nsame = nsame;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
    fi.st.st_uid = ( uid_t ) uid;
    fi.st.st_gid = ( gid_t ) gid;
    fi.st.st_nlink = ( nlink_t ) nlink;
    fi.st.st_size = ( off_t ) filesize;
    fi.name = filename;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;
    duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL );
  }
  DUF_PUTSL( 0 );

  DEBUG_ENDR( r );
  return r;
}

static int
scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  /* SQL at duf_scan_files_by_dirid */
  DUF_SFIELD2( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD2( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */
  DUF_UFIELD2( filemode );
  /* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); */
  DUF_UFIELD2( md5id );
  DUF_UFIELD2( dataid );
  DUF_UFIELD2( md5sum1 );
  /* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); */
  DUF_UFIELD2( md5sum2 );
  /* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); */
  DUF_UFIELD2( mtime );
  DUF_UFIELD2( uid );
  DUF_UFIELD2( gid );
  DUF_UFIELD2( nlink );
  DUF_UFIELD2( inode );
  DUF_UFIELD2( nsame );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  /* if (nsame<2) return r; */

  /* DUF_PRINTF(0, ".> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* DUF_PRINTF(0, ".-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  {
    duf_fileinfo_t fi = { 0 };

    duf_format_t format = {
      .filename = 1,
      .seq = 1,
      .dirid = 0,
      .inode = 1,
      .mode = 1,
      .nlink = 1,
      .uid = 1,
      .gid = 1,
      .filesize = 1,
      .md5 = 0,
      .md5id = 1,
      .mtime = 1,
      .nsame = 1,
      .dataid = 1,
    };
    fi.nsame = nsame;
    fi.st.st_mode = ( mode_t ) filemode;
    fi.st.st_ino = ( ino_t ) inode;
    fi.st.st_mtim.tv_sec = mtime;
    fi.st.st_mtim.tv_nsec = 0;
    fi.st.st_uid = ( uid_t ) uid;
    fi.st.st_gid = ( gid_t ) gid;
    fi.st.st_nlink = ( nlink_t ) nlink;
    fi.st.st_size = ( off_t ) filesize;
    fi.name = filename;
    fi.md5id = md5id;
    fi.dataid = dataid;
    fi.md5sum1 = md5sum1;
    fi.md5sum2 = md5sum2;
    duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL );
  }
  DUF_PUTSL( 0 );

  DEBUG_ENDR( r );
  return r;
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
static int
scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DEBUG_START(  );

  {
    const char *real_path = NULL;

    if ( !real_path )
      real_path = duf_levinfo_path( pdi );

    DUF_UFIELD( nfiles );
    DUF_UFIELD( minsize );
    DUF_UFIELD( maxsize );

    if ( nfiles )
    {
      /* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good */
      if ( duf_config->cli.format.seq )
        DUF_PRINTF( 0, ".d%7llu ", pdi->seq );
      if ( duf_config->cli.format.dirid )
        DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );
      DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" );
    }
    /* mas_free( path ); */
  }

  DEBUG_ENDR( r );

  return r;
}

static int
scan_node_before2( duf_sqlite_stmt_t * pstmt, unsigned long long pathid_unused, duf_depthinfo_t * pdi )
{
  int r = 0;

  DEBUG_START(  );

  /* DUF_UFIELD2( nfiles ); */
  /* DUF_UFIELD2( minsize ); */
  /* DUF_UFIELD2( maxsize ); */
  /* {                                                                                                                      */
  /*   const char *real_path = NULL;                                                                                        */
  /*                                                                                                                        */
  /*   if ( !real_path )                                                                                                    */
  /*     real_path = duf_levinfo_path( pdi );                                                                               */
  /*                                                                                                                        */
  /*   if ( nfiles )                                                                                                        */
  /*   {                                                                                                                    */
  /*     (* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, ".%s\n", path  ); is not good *)               */
  /*     if ( duf_config->cli.format.seq )                                                                                  */
  /*       DUF_PRINTF( 0, ".d%7llu ", pdi->seq );                                                                           */
  /*     if ( duf_config->cli.format.dirid )                                                                                */
  /*       DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );                                                    */
  /*     DUF_PRINTF( 0, ". (%5llu) %llu-%llu %s\n", nfiles, minsize, maxsize, real_path ? real_path : "-=No real_path=-" ); */
  /*   }                                                                                                                    */
  /*   (* mas_free( path ); *)                                                                                              */
  /* }                                                                                                                      */
  if ( pdi->items.files > 0 )
  {
    duf_fileinfo_t fi = { 0 };

    duf_format_t format = {
      .filename = 0,
      .realpath = 1,
      .seq = 1,
      .dirid = 1,
      .inode = 0,
      .mode = 0,
      .nlink = 0,
      .uid = 0,
      .gid = 0,
      .filesize = 0,
      .md5 = 0,
      .md5id = 0,
      .dataid = 0,
      .mtime = 0,
      .nsame = 0,
      .prefix = 1,
    };
    /* fi.nsame = nsame; */
    /* fi.st.st_mode = ( mode_t ) filemode; */
    /* fi.st.st_ino = ( ino_t ) inode;      */
    /* fi.st.st_mtim.tv_sec = mtime;        */
    /* fi.st.st_mtim.tv_nsec = 0;           */
    /* fi.st.st_uid = ( uid_t ) uid;        */
    /* fi.st.st_gid = ( gid_t ) gid;        */
    /* fi.st.st_nlink = ( nlink_t ) nlink;  */
    /* fi.st.st_size = ( off_t ) filesize; */
    /* fi.name = duf_levinfo_itemname( pdi ); */
    /* fi.md5id = md5id; */
    /* fi.md5sum1 = md5sum1; */
    /* fi.md5sum2 = md5sum2; */
    if ( duf_print_file_info( pdi, &fi, &format, ( duf_pdi_cb_t ) NULL ) > 0 )
      DUF_PUTSL( 0 );
    else
      DUF_PUTS( 0, "????????????" );
  }

  DEBUG_ENDR( r );

  return r;
}

duf_scan_callbacks_t duf_print_dir_callbacks = {
  .title = __FILE__ ".dir",
  .scan_mode_2 = 1,
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .node_scan_before2 = scan_node_before2,
  .leaf_scan = scan_leaf,
  .leaf_scan2 = scan_leaf2,
  .fieldset =                   /* */
        "fn.Pathid AS dirid "   /* */
        ", fn.name AS filename, fd.size AS filesize " /* */
        ", fd.size AS filesize " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.id AS dataid " /* */
        ", fd.mode AS filemode " /* */
        ", fn.id AS filenameid" /* */
        ", md.dupcnt AS nsame" /* */
        ", md.id AS md5id, md.md5sum1, md.md5sum2 " /* */
        ,
  /* " , DATETIME(mtim, 'unixepoch') AS mtimef " */
  /* ", strftime('%Y-%m-%d %H:%M:%S',mtim,'unixepoch') AS mtimef " */
  /* ", case cast (strftime('%w', mtim,'unixepoch') AS integer) "                                                                   */
  /* " when 0 then 'Sun' when 1 then 'Mon' when 2 then 'Tue' when 3 then 'Wed' "                                                    */
  /* " when 4 then 'Thu' when 5 then 'Fri' else 'Sat' end AS dowmtime, " "case cast (strftime('%m', mtim,'unixepoch') AS integer) " */
  /* " when 1 then 'Jan' when 2 then 'Feb' when 3 then 'Mar' when 4 then 'Apr' when 5 then 'May' when 6 then "                      */
  /* " 'Jun' when 7 then 'Jul' when 8 then 'Aug' when 9 then 'Sep' when 10 then 'Oct' when 11 then 'Nov' when 12 then 'Dec' "       */
  /* " else 'Wow' end AS monthmtime "                                                                                               */
  .leaf_selector =              /* */
        "SELECT %s "            /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN duf_md5  AS md ON (md.id=fd.md5id)" /*	*/
	"    WHERE " /* */
        /* "           fd.size >= %llu AND fd.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " fn.Pathid='%llu' ",
  .leaf_selector2 =             /* */
        "SELECT %s "            /* */
        " FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON (fn.dataid=fd.id) " /* */
        " LEFT JOIN duf_md5  AS md ON (md.id=fd.md5id) " /* */
        "    WHERE "            /* */
        " (:minsize IS NULL OR fd.size>=:minsize) AND (:maxsize IS NULL OR fd.size<=:maxsize) AND " /* */
        " (:minsame IS NULL OR md.dupcnt>=:minsame) AND (:maxsame IS NULL OR md.dupcnt<=:maxsame) AND " /* */
        " fn.Pathid=:dirid "    /* */
        ,
  .node_selector =
        "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /*	*/
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /*	*/
        /* " ,(SELECT count(*) FROM duf_paths AS subpaths WHERE subpaths.parentid=pt.id) AS ndirs "       */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "          WHERE sfn.Pathid=pt.id "                                                            */
        /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.Pathid=pt.id) AS maxsize "                                               */
        " FROM      duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs  AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid='%llu' ",
  .node_selector2 = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM      duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid=:dirid ",
  /* .final_sql_argv = final_sql, */
};
