






#include <string.h>

#include "duf_maintenance.h"


#include "duf_pdi_ref.h"
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
#if 0
           "'filenames-leaf' AS fieldset_id, " /* Never used!? */
           "  fn.Pathid AS dirid " /* */
           ", 0 AS ndirs, 0 AS nfiles" /* */
           ", fn." DUF_SQL_FILENAMEFIELD " AS filename, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
           ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDFIELD " AS filenameid " /* */
           ", fn." DUF_SQL_IDFIELD " AS nameid " /* */
           ", fd." DUF_SQL_IDFIELD " AS filedataid " /* */
           ", fd." DUF_SQL_IDFIELD " AS dataid " /* */
           ", md.dup5cnt AS nsame " /* */
           ", fd.md5id AS md5id" /* */
           /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fd.exifid AS exifid, fd.mimeid AS mimeid " /* */
           ", fd.size AS filesize " /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON ( md." DUF_SQL_IDFIELD " = fd.md5id ) " /* */
           ,
           .matcher = " fn.Pathid = :parentdirID " /* */
           ,
           .filter = NULL       /* */
#else
           NULL
#endif
           },
  .node = {                     /* */
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           "'filenames-node' AS fieldset_id, " /* */
           " pt." DUF_SQL_IDFIELD " AS dirid" /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
#if 1
           .cte =               /* */
           "WITH RECURSIVE cte_paths(" DUF_SQL_IDFIELD ",parentid) AS " /* */
           " ( "                /* */
           "  SELECT paths." DUF_SQL_IDFIELD ",paths.parentid FROM paths " /* */
           "   WHERE parentid=:topDirID " /* */
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
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           ,
#endif
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL /*                  */ " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL /*  */ " AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL /* */ " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           ,
           .matcher = "pt.parentid = :parentdirID  AND ( :dirName IS NULL OR " DUF_SQL_DIRNAMEFIELD "=:dirName ) " /* */
           ,
           .filter = NULL       /* */
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

    const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILENAMES_FULL " (pathID, " DUF_SQL_FILENAMEFIELD ", dataid) VALUES (:pathID, :Name, :dataID)";

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
