#include "duf_maintenance.h"


#include "duf_sql_defs.h"
#include "sql_beginning_tables.h"
#include "sql_beginning_selected.h"
#include "sql_tables_global_defs.h"


/* ###################################################################### */
#include "std_mod_sets.h"
/* ###################################################################### */

duf_sql_set_t std_leaf_sets[] = { /* */
  {                             /* */
   .name = "std leaf one",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,             /* */
   .fieldset = "#std-leaf",     /* */
   .selector2 = "#std-leaf",    /* */
   .matcher = " fn.Pathid=:parentdirID " /* */
   ,
   .filter = NULL               /* */
   /* " ORDER BY fn." DUF_SQL_IDFIELD " " */
   }
  ,                             /* */
  {                             /* */
   .name = "std leaf no sel",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,             /* */
   .fieldset = "#std-ns-leaf",
   .selector2 =                 /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
   "#std-ns-leaf",
   .matcher = " fn.Pathid=:parentdirID " /*  +pu  */
   /* " ORDER BY fn." DUF_SQL_IDFIELD " " *//* */
   ,
   .filter =                    /* */
   DUF_SQL_UFILTER_BINDINGS     /* */
   /* " ORDER BY ...." */
   }
  ,                             /* */
  {                             /* */
   .name = "std leaf no sel no matcher",
   .type = DUF_NODE_LEAF,
   .expand_sql = 1,             /* */
   .fieldset = "#std-ns-leaf",
   .selector2 =                 /* ns: without selected table(s) : DUF_SQL_TABLES_FILENAMES_FULL, not DUF_SQL_SELECTED_TMP_FILENAMES_FULL */
   "#std-ns-leaf",
   .filter = DUF_SQL_UFILTER_BINDINGS, /* */
   /* " ORDER BY ...." */
   /* .order = "sha1id",           (* *) */
   /* .order = "nsame_sha1 DESC,sha1id",           (* *) */
   .order = "sh.dupsha1cnt DESC,sha1id",           /* */
   }
};

size_t std_leaf_nsets = sizeof( std_leaf_sets ) / sizeof( std_leaf_sets[0] );




duf_sql_set_t std_node_sets[] = { /* */
  {
   .name = "std node one",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
   "#std-node",
#ifdef DUF_USE_CTE
   /* never used?! */
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
#  ifndef DUF_NO_NUMS
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#  endif
   ,
#endif

   .selector2 =                 /* */
#if 1
   " FROM      " DUF_SQL_SELECTED_TMP_PATHS_FULL " AS pts " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL " AS pt ON( pts.parentid = pt." DUF_SQL_IDFIELD " ) " /* */
#  ifndef DUF_NO_NUMS
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_DIRS_FULL "  AS td ON ( td.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
   " LEFT JOIN " DUF_SQL_SELECTED_TMP_PATHTOT_FILES_FULL "  AS tf ON ( tf.Pathid = pt." DUF_SQL_IDFIELD " ) " /* */
#  endif
#else
   "#std-node"
#endif
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName ) " /* */
   ,
   .filter = NULL               /* */
   }
  ,
  {
   .name = "std node two",
   .type = DUF_NODE_NODE,
   .expand_sql = 1,             /* */
   .fieldset =                  /* */
   "#std-node",
#ifdef DUF_USE_CTE
   .cte =                       /* */
   "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
   " ( "                        /* */
   "  SELECT pt." DUF_SQL_IDFIELD ",pt.parentid FROM " DUF_SQL_TABLES_PATHS_FULL "  AS pt" /* */
   "   WHERE parentid=:topDirID " /* */
   "  UNION "                   /* */
   "   SELECT ptu." DUF_SQL_IDFIELD ",ptu.parentid " /* */
   "    FROM cte_paths "        /* */
   "    JOIN " DUF_SQL_TABLES_PATHS_FULL " AS ptu ON( ptu.parentid=cte_paths." DUF_SQL_IDFIELD " ) " /* */
   " ) ",
   .selector2_cte =             /* */
   " FROM cte_paths " /*                                     */ " AS pte " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*                */ " AS pt ON (pte." DUF_SQL_IDFIELD " = pt." DUF_SQL_IDFIELD ") " /* */
#  ifndef DUF_NO_NUMS
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
#  endif
   ,
#endif
   .selector2 =                 /* */
   " FROM      " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
#ifndef DUF_NO_NUMS
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid = pt." DUF_SQL_IDFIELD ") " /* */
#endif
   ,
   .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName ) " /* */
   ,
#ifdef DUF_NO_RNUMS
   .filter = NULL               /* */
#else
   .filter = DUF_SQL_RNUMDIRS( pt ) " > 0 AND (" DUF_SQL__RNUMFILES( pt ) ") > 0 " /* */
#endif
   }
};

size_t std_node_nsets = sizeof( std_node_sets ) / sizeof( std_node_sets[0] );
