/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */
#include "sql_beginning_selected.h"
#include "sql_tables_global_defs.h"

/* ###################################################################### */
#include "std_mod_sets.h"
/* ###################################################################### */

duf_sql_set_t std_leaf_sets[] = {                                    /* */
  {                                                                  /* */
   .name = "std-leaf-one",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,                                                  /* */
   .fieldset = "#std-leaf",                                          /* */
   .selector2 = "#std-leaf",                                         /* */
   .matcher = " fn.Pathid=:parentdirID "                             /* */
   ,
   .filter = NULL                                                    /* */
 /* " ORDER BY fn." DUF_SQL_IDFIELD " " */
   }
  ,                                                                  /* */
  {                                                                  /* */
   .name = "std-leaf-no-sel",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,                                                  /* */
   .fieldset = "#std-leaf",
   .selector2 = "#std-ns-leaf",                                      /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
   .matcher = " fn.Pathid=:parentdirID "                             /*  +pu  */
 /* " ORDER BY fn." DUF_SQL_IDFIELD " " *//* */
   ,
#if 0
   .filter = DUF_SQL_UFILTER_BINDINGS,                               /* */
#else
   .afilter = {
               DUF_SQL_UFILTER_BINDINGS,                             /* */
             /* for no-sel only: */
               "fn.Pathid IN ( "                                     /* */
               " WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ", parentid) AS ( " /* */
               "		SELECT pt." DUF_SQL_IDFIELD ",pt.parentid "        /* */
               "		FROM main.paths AS pt WHERE rowid=:topDirID "      /* */
               "		UNION "                                            /* */
               "			SELECT ptu." DUF_SQL_IDFIELD ", ptu.parentid "    /* */
               "			  FROM cte_paths "                                /* */
               "				JOIN main.paths AS ptu ON ( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " )) " /* */
               " SELECT " DUF_SQL_IDFIELD " FROM cte_paths"          /* */
               ")"                                                   /* */
               }
   ,                                                                 /* */
#endif
 /* " ORDER BY ...." */
   }
  ,                                                                  /* */
  {                                                                  /* */
   .name = "std-leaf-no-sel-no-matcher",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,                                                  /* */
   .fieldset = "#std-leaf",
   .selector2 = "#std-ns-leaf",                                      /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
#if 0
   .filter = DUF_SQL_UFILTER_BINDINGS,                               /* */
#else
   .afilter = {
               DUF_SQL_UFILTER_BINDINGS,                             /* */
             /* for no-sel only: */
               "fn.Pathid IN ( "                                     /* */
               " WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ", parentid) AS ( " /* */
               "		SELECT pt." DUF_SQL_IDFIELD ",pt.parentid "        /* */
               "		FROM main.paths AS pt WHERE rowid=:topDirID "      /* */
               "		UNION "                                            /* */
               "			SELECT ptu." DUF_SQL_IDFIELD ", ptu.parentid "    /* */
               "			  FROM cte_paths "                                /* */
               "				JOIN main.paths AS ptu ON ( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " )) " /* */
               " SELECT " DUF_SQL_IDFIELD " FROM cte_paths"          /* */
               ")"                                                   /* */
               }
   ,                                                                 /* */
#endif
 /* " ORDER BY ...." */
 /* .order = "sha1id",           (* *) */
 /* .order = "nsame_sha1 DESC,sha1id",           (* *) */
   .order = "sh.dupsha1cnt DESC,sha1id",                             /* */
   }
  ,
  {
   .name = "std-all-under",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,                                                  /* */
   .fieldset = "#std-leaf",
   .cte = "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
   " ( "                                                             /* */
   "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths "   /* */
   "   WHERE rowid=:topDirID "                                       /* */
   "  UNION "                                                        /* */
   "   SELECT paths." DUF_SQL_IDFIELD ", paths.parentid "            /* */
   "    FROM cte_paths "                                             /* */
   "    JOIN paths ON( paths.parentid=cte_paths." DUF_SQL_IDFIELD ") " /* */
   " ) ",
   .selector2_cte = "FROM filenames AS fn "                          /* */
   " JOIN cte_paths AS pte ON (fn.Pathid=pte." DUF_SQL_IDFIELD ") "  /* */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*               */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*                   */ " AS sz ON (sz.size=fd.size)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL /*                    */ " AS x  ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL /*              */ " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL /*                    */ " AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                     */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) ", /* */
#if 0
   .filter = DUF_SQL_UFILTER_BINDINGS,                               /* */
#else
   .afilter = {
               DUF_SQL_UFILTER_BINDINGS,                             /* */
             /* for no-sel only: -- One of:
              *  1. JOIN cte_paths
              *  2. fn.Pathid IN ( WITH...
              *  */
# if 0
               "fn.Pathid IN ( "                                     /* */
               " WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ", parentid) AS ( " /* */
               "		SELECT pt." DUF_SQL_IDFIELD ",pt.parentid "        /* */
               "		FROM main.paths AS pt WHERE rowid=:topDirID "      /* */
               "		UNION "                                            /* */
               "			SELECT ptu." DUF_SQL_IDFIELD ", ptu.parentid "    /* */
               "			  FROM cte_paths "                                /* */
               "				JOIN main.paths AS ptu ON ( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " )) " /* */
               " SELECT " DUF_SQL_IDFIELD " FROM cte_paths"          /* */
               ")"                                                   /* */
# endif
               }
   ,                                                                 /* */
#endif
   .order = "sh.dupsha1cnt ASC, sha1id",                             /* */
   .matcher = NULL,
 /* To select ALL files under path
    WITH RECURSIVE cte_paths(rowid,parentid) AS  (   SELECT pt.rowid,pt.parentid FROM main.paths  AS pt WHERE rowid=:PathID   UNION     SELECT ptustd.rowid,ptustd.parentid     FROM cte_paths     JOIN main.paths AS ptustd ON( ptustd.parentid=cte_paths.rowid                     )  ) SELECT fn.file_name FROM filenames AS fn  JOIN cte_paths AS pte ON(fn.Pathid=pte.rowid)
    * */
   }
  ,
  {                                                                  /* */
   .name = "std-leaf-no-sel-fd",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,                                                  /* */
   .fieldset = "#std-leaf",
   .selector2 = "#std-ns-fd-leaf",                                   /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
   .matcher = " fn.Pathid=:parentdirID "                             /*  +pu  */
 /* " ORDER BY fn." DUF_SQL_IDFIELD " " *//* */
   ,
#if 0
   .filter = DUF_SQL_UFILTER_BINDINGS,                               /* */
#else
   .afilter = {
               DUF_SQL_UFILTER_BINDINGS,                             /* */
             /* for no-sel only: */
               "fn.Pathid IN ( "                                     /* */
               " WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ", parentid) AS ( " /* */
               "		SELECT pt." DUF_SQL_IDFIELD ",pt.parentid "        /* */
               "		FROM main.paths AS pt WHERE rowid=:topDirID "      /* */
               "		UNION "                                            /* */
               "			SELECT ptu." DUF_SQL_IDFIELD ", ptu.parentid "    /* */
               "			  FROM cte_paths "                                /* */
               "				JOIN main.paths AS ptu ON ( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " )) " /* */
               " SELECT " DUF_SQL_IDFIELD " FROM cte_paths"          /* */
               ")"                                                   /* */
               }
   ,                                                                 /* */
#endif
 /* " ORDER BY ...." */
   }
  ,                                                                  /* */

};

int std_leaf_nsets = sizeof( std_leaf_sets ) / sizeof( std_leaf_sets[0] );

duf_sql_set_t std_node_sets[] = {                                    /* */
  {
   .name = "std-node-one",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,                                                  /* */
   .fieldset =                                                       /* */
   "#std-node",
#ifdef DUF_USE_CTE
 /* never used?! */
   .cte =                                                            /* */
   "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS "      /* */
   " ( "                                                             /* */
   "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths "   /* */
   "   WhERE parentid=:topDirID "                                    /* */
   "  UNION "                                                        /* */
   "   SELECT paths." DUF_SQL_IDFIELD ", paths.parentid "            /* */
   "    FROM cte_paths "                                             /* */
   "    JOIN paths ON( paths.parentid=cte_paths." DUF_SQL_IDFIELD ") " /* */
   " ) ",
   .selector2_cte =                                                  /* */
   " FROM cte_paths " /*                                  */ " AS pte " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*             */ " AS pt ON (pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ") " /* */
# ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# elif defined( DUF_DO_NUMS )
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
# endif
   ,
#endif

   .selector2 =                                                      /* */
#if 1
   " FROM      " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts "          /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON( pts.parentid = pt." DUF_SQL_IDFIELD " ) " /* */
# ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# elif defined( DUF_DO_NUMS )
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
# endif
#else
   "#std-node"
#endif
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName ) " /* */
   ,
   .filter = NULL                                                    /* */
   }
  ,
  {
   .name = "std-node-two",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,                                                  /* */
   .fieldset =                                                       /* */
   "#std-node",
#ifdef DUF_USE_CTE
   .cte =                                                            /* */
   "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS "      /* */
   " ( "                                                             /* */
   "  SELECT pt." DUF_SQL_IDFIELD ",pt.parentid FROM " DUF_SQL_TABLES_PATHS_FULL "  AS pt" /* */
   "   WHeRE parentid=:topDirID "                                    /* */
 /* TODO !!!!! "       OR " DUF_SQL_IDFIELD "=:topDirID   " (* 20160118.162736 : 
  *  counting: need count self additionally to childs, see mod_filenames_uni.c (20160118.164635) *) */
   "  UNION "                                                        /* */
   "   SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid "                 /* */
   "    FROM cte_paths "                                             /* */
   "    JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " /* */
   " ) ",
   .selector2_cte =                                                  /* */
   " FROM cte_paths " /*                                     */ " AS pte " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*                */ " AS pt ON (pte." DUF_SQL_IDFIELD " = pt." DUF_SQL_IDFIELD ") " /* */
# ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
# elif defined( DUF_DO_NUMS )
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
# endif
   ,
#endif
   .selector2 =                                                      /* */
   " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt "                 /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
#endif
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName ) " /* */
   ,
#ifdef DUF_NO_RNUMS
   .filter = NULL                                                    /* */
#else
   .filter = DUF_SQL_RNUMDIRS( pt ) " > 0 AND (" DUF_SQL__RNUMFILES( pt ) ") > 0 " /* */
#endif
   }
};

int std_node_nsets = sizeof( std_node_sets ) / sizeof( std_node_sets[0] );
