#define DUF_SQL_PDI_STMT

/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"


#include "duf_sql_defs.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_selected.h"


duf_sql_set_t std_leaf_set = {
  .fieldset =                   /* */
        "  fn.Pathid AS dirid " /* */
        ", fn.name AS filename, fd.size AS filesize, fd.exifid as exifid, fd.mimeid as mimeid " /* */
        ", fd.size AS filesize " /* */
        ", uid, gid, nlink, inode " /* */
        ", STRFTIME( '%s', mtim ) AS mtime " /* */
        ", fd." DUF_SQL_IDNAME " AS dataid " /* */
        ", fd.mode AS filemode " /* */
        ", md.md5sum1, md.md5sum2 " /* */
        ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
        ", md.dup5cnt AS nsame " /* */
        ", mi.mime AS mime "    /* */
        ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
        /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
        ", fd.md5id AS md5id "  /* */
        ,
  .selector2 =                  /* */
        /* "SELECT %s " */
        " FROM " DUF_SQL_SELECTED_NAME_FULL " AS fns " /* */
        " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas  AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        " LEFT JOIN " DUF_DBPREF "mime       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif       AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif_model AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
        "    WHERE "            /* */
        " fn.Pathid=:parentdirID " /* */
        /* " ORDER BY fn." DUF_SQL_IDNAME " " *//* */
        ,
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF " md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
};

duf_sql_set_t std_node_set = {
  .fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_SQL_SELECTED_PATHS_FULL " AS pts " /* */
        " LEFT JOIN " DUF_DBPREF " paths AS pt ON( pts.parentid = pt.rowid ) " /* */
        " LEFT JOIN " DUF_SQL_SELECTED_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_SQL_SELECTED_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
        ,
  .selector_total2 =            /* */
        " /* std */ FROM " DUF_SQL_SELECTED_PATHS_FULL " AS p " /* */
};



duf_sql_set_t std_ns_leaf_set = {
  .fieldset =                   /* */
        "  fn.Pathid AS dirid " /* */
        ", fn.name AS filename, fd.size AS filesize, fd.exifid as exifid, fd.mimeid as mimeid " /* */
        ", fd.size AS filesize " /* */
        ", uid, gid, nlink, inode " /* */
        ", STRFTIME( '%s', mtim ) AS mtime " /* */
        ", fd." DUF_SQL_IDNAME " AS dataid " /* */
        ", fd.mode AS filemode " /* */
        ", md.md5sum1, md.md5sum2 " /* */
        ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
        ", md.dup5cnt AS nsame " /* */
        ", mi.mime AS mime "    /* */
        ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
        /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
        ", fd.md5id AS md5id "  /* */
        ,
  .selector2 =                  /* */
        /* "SELECT %s " */
        " FROM " DUF_SQL_TABLES_NAME_FULL " AS fns " /* */
        " JOIN " DUF_DBPREF "filenames AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas  AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
        " LEFT JOIN " DUF_DBPREF "mime       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif       AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
        " LEFT JOIN " DUF_DBPREF "exif_model AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
        "    WHERE "            /* */
        " fn.Pathid=:parentdirID " /* */
        " AND "                 /* */
        DUF_SQL_UFILTER_BINDINGS
        /* " ORDER BY fn." DUF_SQL_IDNAME " " *//* */
        ,
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF " filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF " md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
};

duf_sql_set_t std_ns_node_set = {
  .fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
        " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
        ,
  .selector_total2 =            /* */
        " /* sns */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
};


duf_sql_set_t std_leaf_set__ = {
  .fieldset = "fn.pathid AS dirid " /* */
        ", fn.name AS filename, fd.size AS filesize" /* */
        ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
        ", dup5cnt AS nsame "   /* */
        ", fd." DUF_SQL_IDNAME " AS filenameid" /* */
        ", fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
        ", fd.md5id AS md5id"   /* */
        ,
  .selector2 =                  /* */
        " FROM      " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid            =fd." DUF_SQL_IDNAME " ) " /* */
        " LEFT JOIN " DUF_DBPREF "md5       AS md ON ( md." DUF_SQL_IDNAME "=fd.md5id              ) " /* */
        "    WHERE "            /* */
        " fn.pathid = :parentdirID" /* */
        ,
  .selector_total2 =            /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
};

duf_sql_set_t std_node_set__ = {
  .fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
        ,.selector2 =           /* */
        " FROM " DUF_DBPREF "paths AS pt " /* */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
        " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
        ,
  .selector_total2 =            /* */
        " /* _ */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
};