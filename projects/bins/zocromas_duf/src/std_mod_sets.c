#include "duf_maintenance.h"


#include "duf_sql_defs.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_selected.h"

/* ###################################################################### */
#include "std_mod_sets.h"
/* ###################################################################### */

duf_sql_set_t std_leaf_sets[] = { /* */
  {                             /* */
   .name = "std leaf one",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
#if 0
   /* " 'std-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn." DUF_SQL_FILENAMEFIELD " AS filename, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid " /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid " /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
#else
   "#std-leaf"
#endif
   ,
   .selector2 =                 /* */
#if 0
   " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL " AS fns " /* */
   " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns.nameid=fn." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "        AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL "       AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "       AS x ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
#else
   "#std-leaf"
#endif
   ,
   .matcher = " fn.Pathid=:parentdirID " /* */
   /* */
   ,
   .filter =                    /* */
   NULL                         /* */
   /* " ORDER BY fn." DUF_SQL_IDFIELD " " */
   }
  ,                             /* */
  {                             /* */
   .name = "std leaf no sel",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
#if 0
   /* " 'std-ns-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn." DUF_SQL_FILENAMEFIELD " AS filename, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid " /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid " /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
#else
   "#std-ns-leaf"
#endif
   ,
   .selector2 =                 /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
#if 0
   " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
   /* " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDFIELD "=fn." DUF_SQL_IDFIELD ") " (* *) */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL "        AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL "       AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL "       AS x  ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
#else
   "#std-ns-leaf"
#endif
   ,
   .matcher = " fn.Pathid=:parentdirID " /*  +pu  */
   /* " ORDER BY fn." DUF_SQL_IDFIELD " " *//* */
   ,
   .filter =                    /* */
   DUF_SQL_UFILTER_BINDINGS     /* */
   /* " ORDER BY ...." */
   }
};

size_t std_leaf_nsets = sizeof( std_leaf_sets ) / sizeof( std_leaf_sets[0] );




duf_sql_set_t std_node_sets[] = { /* */
  {
   .name = "std node one",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
#if 0
   /* "'std-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid" /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
#else
   "#std-node"
#endif
   ,
#if 1
   .cte =                       /* */
   "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
   " ( "                        /* */
   "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths " /* */
   "   WHERE parentid=:topDirID " /* */
   "  UNION "                   /* */
   "   SELECT paths." DUF_SQL_IDFIELD ",paths.parentid " /* */
   "    FROM cte_paths "        /* */
   "    JOIN paths ON(paths.parentid=cte_paths." DUF_SQL_IDFIELD ") " /* */
   " ) ",
   .selector2_cte =             /* */
   " FROM cte_paths " /*                                  */ " AS pte " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*             */ " AS pt ON (pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
   ,
#endif

   .selector2 =                 /* */
#if 1
   " FROM      " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON( pts.parentid = pt." DUF_SQL_IDFIELD " ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
#else
   "#std-node"
#endif
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR " DUF_SQL_DIRNAMEFIELD "=:dirName ) " /* */
   ,
   .filter = NULL               /* */
   }
  ,
  {
   .name = "std node two",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
#if 0
   /* "'std-ns-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid" /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
#else
   "#std-node"
#endif
   ,
   .selector2 =                 /* */
   " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR " DUF_SQL_DIRNAMEFIELD "=:dirName ) " /* */
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
