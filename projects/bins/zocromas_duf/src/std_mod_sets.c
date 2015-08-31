#define DUF_SQL_PDI_STMT

/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"


#include "duf_sql_defs.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_selected.h"






duf_sql_set_t std_leaf_set = {
  .fieldset =                   /* */
        /* " 'std-leaf' AS fieldset_id, " (* *) */
        "  fn.Pathid AS dirid " /* */
        ", 0 as ndirs, 0 as nfiles" /* */
        ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
        ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
        ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
        ", fn." DUF_SQL_IDNAME " AS nameid " /* */
        ", md.dup5cnt AS nsame " /* */
        ", fd.md5id AS md5id "  /* */
        /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
        ", md.md5sum1, md.md5sum2 " /* */
        /* */
        ", fd." DUF_SQL_IDNAME " AS dataid " /* */
        ", mi.mime AS mime "    /* */
        ", xm.model as camera " /* */
        ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
        ", fd.exifid as exifid, fd.mimeid as mimeid " /* */
        ,
  .selector2 =                  /* */
        /* "SELECT %s " */
        " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS fns " /* */
        " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas  AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        " LEFT JOIN " DUF_DBPREF "mime       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif       AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif_model AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
        /* " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" (* *) */
        ,
  .matcher = " fn.Pathid=:parentdirID " /* */
        ,
  .filter = NULL                /* */
        /* " ORDER BY fn." DUF_SQL_IDNAME " " *//* */
        ,
#if 0
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF " md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
#endif
};

duf_sql_set_t std_node_set = {
  .fieldset =                   /* */
        /* "'std-node' AS fieldset_id, " (* *) */
        " pt." DUF_SQL_IDNAME " AS dirid" /* */
        ", pt." DUF_SQL_IDNAME " AS nameid " /* */
        ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
        ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
        " LEFT JOIN " DUF_DBPREF " paths AS pt ON( pts.parentid = pt.rowid ) " /* */
        " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        ,
  .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
        ,
  .filter = NULL                /* */
        ,
#if 0
  .selector_total2 =            /* */
        " /* std */ FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS p " /* */
#endif
};



duf_sql_set_t std_ns_leaf_set = {
  .fieldset =                   /* */
        /* " 'std-ns-leaf' AS fieldset_id, " (* *) */
        " fn.Pathid AS dirid "  /* */
        ", 0 as ndirs, 0 as nfiles" /* */
        ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
        ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
        ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
        ", fn." DUF_SQL_IDNAME " AS nameid " /* */
        ", md.dup5cnt AS nsame " /* */
        ", fd.md5id AS md5id "  /* */
        /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
        ", md.md5sum1, md.md5sum2 " /* */
	/* */
	", fd.exifid as exifid, fd.mimeid as mimeid " /* */
        ", fd.size AS filesize " /* */
        ", fd." DUF_SQL_IDNAME " AS dataid " /* */
        ", mi.mime AS mime "    /* */
        ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
        ", xm.model as camera"
        ,
  .selector2 =                  /* */
        /* "SELECT %s " */
        " FROM " DUF_SQL_TABLES_NAME_FULL " AS fns " /* */
        " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas  AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        " LEFT JOIN " DUF_DBPREF "mime       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif       AS x  ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif_model AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
        ,
  .matcher = " fn.Pathid=:parentdirID " /* */
        /* " ORDER BY fn." DUF_SQL_IDNAME " " *//* */
        ,
  .filter =                     /* */
        DUF_SQL_UFILTER_BINDINGS /* */
        ,
#if 0
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF " md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
#endif
};

duf_sql_set_t std_ns_node_set = {
  .fieldset =                   /* */
        /* "'std-ns-node' AS fieldset_id, " (* *) */
        " pt." DUF_SQL_IDNAME " AS dirid" /* */
        ", pt." DUF_SQL_IDNAME " AS nameid " /* */
        ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        ,
  .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
        ,
  .filter = NULL                /* */
        ,
#if 0
  .selector_total2 =            /* */
        " /* sns */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
};

#if 0
duf_sql_set_t std_leaf_set__ = {
  .fieldset = "fn.pathid AS dirid " /* */
        ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize" /* */
        ", 0 as ndirs, 0 as nfiles" /* */
        ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
        " , strftime('%s',fd.mtim) AS mtime " /* */
        ", dup5cnt AS nsame "   /* */
        ", fd." DUF_SQL_IDNAME " AS filenameid" /* */
        ", fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
        ", fd.md5id AS md5id"   /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid            =fd." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5       AS md ON ( md." DUF_SQL_IDNAME "=fd.md5id              ) " /* */
        ,
  .matcher = " fn.pathid = :parentdirID" /* */
        ,
  .filter = NULL                /* */
        ,
#  if 0
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
#  endif
};

duf_sql_set_t std_node_set__ = {
  .fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
        ,.selector2 =           /* */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        ,
  .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
        ,
  .filter = NULL                /* */
        ,
#  if 0
  .selector_total2 =            /* */
        " /* _ */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#  endif
};
#endif
