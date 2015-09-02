#define DUF_SQL_PDI_STMT

/* #include <assert.h> */

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"


#include "duf_sql_defs.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_selected.h"



duf_sql_set_t std_leaf_sets[] = { /* */
  {                             /* */
   .fieldset =                  /* */
   " 'std-leaf' AS fieldset_id, " /* */
   "  fn.Pathid AS dirid "      /* */
   ", 0 as ndirs, 0 as nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode AS filemode "     /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", md.dup5cnt AS nsame "     /* */
   ", fd.md5id AS md5id "       /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   ", mi.mime AS mime "         /* */
   ", xm.model as camera "      /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", fd.exifid as exifid, fd.mimeid as mimeid " /* */
   ", xm.model as camera",
   .selector2 =                 /* */
   " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS fns " /* */
   " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL "       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "       AS x ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size)" /* */
   ,
   .matcher = " fn.Pathid=:parentdirID " /* */
   /* */
   ,
   .filter =                    /* */
   NULL                         /* */
   /* " ORDER BY fn." DUF_SQL_IDNAME " " */
   }
  , {                           /* */
     .fieldset =                /* */
     " 'std-ns-leaf' AS fieldset_id, " /* */
     " fn.Pathid AS dirid "     /* */
     ", 0 as ndirs, 0 as nfiles" /* */
     ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
     ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
     ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
     ", fd.mode AS filemode "   /* */
     ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
     ", fn." DUF_SQL_IDNAME " AS nameid " /* */
     ", md.dup5cnt AS nsame "   /* */
     ", fd.md5id AS md5id "     /* */
     /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
     ", md.md5sum1, md.md5sum2 " /* */
     ", fd.exifid as exifid, fd.mimeid as mimeid " /* */
     ", fd.size AS filesize "   /* */
     /* */
     ", fd." DUF_SQL_IDNAME " AS dataid " /* */
     ", mi.mime AS mime "       /* */
     ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
     ", xm.model as camera",
     .selector2 =               /* */
     " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
     /* " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDNAME "=fn." DUF_SQL_IDNAME ") " (* *) */
     " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
     " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "        AS md ON (md." DUF_SQL_IDNAME "=fd.md5id) " /* */
     " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL "       AS mi ON (mi." DUF_SQL_IDNAME "=fd.mimeid) " /* */
     " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "       AS x  ON (x." DUF_SQL_IDNAME "=fd.exifid) " /* */
     " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDNAME ") " /* */
     " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " as sz ON (sz.size=fd.size)" /* */
     ,
     .matcher = " fn.Pathid=:parentdirID " /* */
     /* " ORDER BY fn." DUF_SQL_IDNAME " " *//* */
     ,
     .filter =                  /* */
     DUF_SQL_UFILTER_BINDINGS   /* */
     /* " ORDER BY ...." */
     }
};

size_t std_leaf_nsets = sizeof( std_leaf_sets ) / sizeof( std_leaf_sets[0] );




duf_sql_set_t std_node_sets[] = { /* */
  {
   .fieldset =                  /* */
   "'std-node' AS fieldset_id, " /* */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   ,
   .selector2 =                 /* */
   " FROM      " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON( pts.parentid = pt.rowid ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
   ,
   .filter = NULL               /* */
   ,
#if 0
   .selector_total2 =           /* */
   " /* std */ FROM " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS p " /* */
#endif
   }
  ,
  {
   .fieldset =                  /* */
   "'std-ns-node' AS fieldset_id, " /* */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   ,
   .selector2 =                 /* */
   " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
   ,
   .filter = NULL               /* */
   ,
#if 0
   .selector_total2 =           /* */
   " /* sns */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
   }
};

size_t std_node_nsets = sizeof( std_node_sets ) / sizeof( std_node_sets[0] );
