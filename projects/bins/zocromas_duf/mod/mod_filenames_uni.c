






#include <string.h>

#include "duf_maintenance.h"


#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"
/* #include "duf_sql.h" */
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"



#include "duf_filedata.h"

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_tables.h"


/* ########################################################################################## */

static int filenames_leaf2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
static int filenames_leaf2_deleted( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
static int filenames_de_file_before2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = { /* */
  .name = "final @ ...",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET dupdatacnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_FILEDATAS_FULL "." DUF_SQL_IDFIELD "=fd." DUF_SQL_IDFIELD ")" /* */
          ,
          NULL}
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_filenames_callbacks = {
  .title = "file names",
  .name = "filenames",
  .init_scan = NULL,
  .def_opendir = 1,

  /* .dirent_file_scan_before = filenames_entry_reg, */
  .dirent_file_scan_before2 = filenames_de_file_before2,

  .leaf_scan2 = filenames_leaf2,
  .leaf_scan2_deleted = filenames_leaf2_deleted,


/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 2,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 2,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .count_nodes = 1,
  .leaf = {                     /* */
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
           NULL},
  .node = {                     /* */
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           "'filenames-node' AS fieldset_id, " /* */
           " pt." DUF_SQL_IDFIELD " AS dirid" /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
#ifndef DUF_NO_NUMS
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
#endif
#ifndef DUF_NO_RNUMS
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs " /* */
           ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles " /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,

/* 
 * TODO 20160118.164635
 *  1. filedata need to put file counter into paths table
 *  2. filenames to use it as counter!
 *  currently filenames counts childs dir, therefore doesn't scan files is there are no child dirs
 * */   
#ifdef DUF_USE_CTE
           .cte =               /* */
           "W? ITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
           " ( "                /* */
           "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths " /* */
           "   WHERE parentid=:topDirID " /* */
#  ifndef DUF_NO_RNUMS
           /* " AND " DUF_SQL_RNUMDIRS( pt ) " > 0 AND (" DUF_SQL__RNUMFILES( pt ) ") > 0 " (* *) */
#  endif
           "  UNION "           /* */
           "   SELECT paths." DUF_SQL_IDFIELD ",paths.parentid " /* */
           "    FROM cte_paths " /* */
           "    JOIN paths ON(paths.parentid=cte_paths." DUF_SQL_IDFIELD ") " /* */
           " ) ",
/*
 WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS 
   (SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths 
      WHERE parentid=15 --------- matcher
     UNION 
     SELECT paths." DUF_SQL_IDFIELD ",paths.parentid 
        FROM cte_paths 
        JOIN paths ON(paths.parentid=cte_paths." DUF_SQL_IDFIELD ")
   )
   SELECT *
      FROM cte_paths AS pte
      LEFT JOIN paths AS pt ON(pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ")
      LEFT JOIN t_common_pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ")
      LEFT JOIN t_common_pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") */
           .selector2_cte =     /* */
           " FROM cte_paths " /*                                  */ " AS pte " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHS_FULL /*             */ " AS pt ON (pte." DUF_SQL_IDFIELD "=pt." DUF_SQL_IDFIELD ") " /* */
#  ifndef DUF_NO_NUMS
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#  endif
           ,
#endif
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL /*                  */ " AS pt " /* */
#ifndef DUF_NO_NUMS
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName ) " /* */
           ,
#ifndef DUF_NO_NUMS
	     /* .filter = "rndirs > 0 AND rnfiles > 0"       (* *) */
#else
           .filter = NULL       /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
static int DUF_UNUSED
filenames_leaf2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi DUF_UNUSED )
{
  DEBUG_STARTR( r );
  /* T( "@@[%d] %s%s", duf_pdi_depth( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) ); */
  DEBUG_ENDR( r );
}

static int DUF_UNUSED
filenames_leaf2_deleted( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( todo, 0, "@@@@@@@[%d] %s%s", duf_pdi_depth( pdi ), duf_levinfo_path( pdi ), duf_levinfo_itemtruename( pdi ) );
  /* TODO remove or mark name from DB */
  DEBUG_ENDR( r );
}

static int
filenames_de_file_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  const char *fname = duf_levinfo_itemtruename( pdi );

  unsigned long long dataid;

  DOPR( r, dataid = duf_pdistat2file_dataid_existed( pdi, /* duf_levinfo_stat( pdi ), */ &r ) );
  assert( dataid > 0 );
  if ( DUF_NOERROR( r ) && fname && duf_levinfo_dirid_up( pdi ) )
  {
    int changes = 0;

    const char *sql =
          "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILENAMES_FULL " (pathID, " DUF_SQL_FILENAMEFIELD ", dataid) VALUES (:pathID, :Name, :dataID)";

    DUF_SQL_START_STMT( pdi, insert_filename, sql, r, pstmt );
    DUF_TRACE( mod, 3, "S:%s", sql );
    DUF_SQL_BIND_LL( pathID, duf_levinfo_dirid_up( pdi ), r, pstmt );
    DUF_SQL_BIND_S( Name, fname, r, pstmt );
    DUF_SQL_BIND_LL( dataID, dataid, r, pstmt );
    DUF_SQL_STEPC( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( pdi, insert_filename, r, pstmt );
  }
  else
  {
    /* DUF_SHOW_ERROR( "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) ); */
    DUF_MAKE_ERRORM( r, DUF_ERROR_DATA, "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) );
    DUF_TEST_R( r );
  }
  /* DUF_TRACE( mod, 0, "%llu : %s @ %llu", dirid, fname, dirid ); */
  DEBUG_ENDR( r );
}
