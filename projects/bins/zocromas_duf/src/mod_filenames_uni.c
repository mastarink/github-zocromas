#define DUF_SQL_PDI_STMT







#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"



#include "duf_filedata.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */

static int
filenames_insert_filename_uni( duf_depthinfo_t * pdi, const char *fname, unsigned long long dataid )
{
  DEBUG_STARTR( r );
  if ( fname && duf_levinfo_dirid_up( pdi ) )
  {
    int changes = 0;

    const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "filenames (Pathid, name, dataid) VALUES (:pathID, :Name, :dataID)";

    DUF_SQL_START_STMT( pdi, insert_filename, sql, r, pstmt );
    DUF_TRACE( mod, 3, "S:%s", sql );
    DUF_SQL_BIND_LL( pathID, duf_levinfo_dirid_up( pdi ), r, pstmt );
    DUF_SQL_BIND_S( Name, fname, r, pstmt );
    DUF_SQL_BIND_LL( dataID, dataid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( insert_filename, r, pstmt );
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_DATA );
    DUF_TEST_R( r );
  }
  /* DUF_TRACE( mod, 0, "%llu : %s @ %llu", dirid, fname, dirid ); */
  DEBUG_ENDR( r );
}

static int
register_direntry( const char *fname, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  if ( pst_file )
  {
    unsigned long long dataid = 0;

    DOPR( r, dataid = duf_stat2file_dataid_existed( pdi, pst_file, &r ) );
    DOR( r, filenames_insert_filename_uni( pdi, fname, dataid ) );
  }
  DEBUG_ENDR( r );
}






static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
          "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn " /* */
          " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " GROUP BY fn.Pathid" /* */
          ,
          "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
          " minsize=(SELECT min(size) AS minsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", maxsize=(SELECT max(size) AS maxsize " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
          ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
          " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)" /* */
          ,
#endif

          /* "DELETE FROM " DUF_DBPREF "sizes",                                 */
          /* "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " (* *) */
          /*       "SELECT size, COUNT(*) " (* *)                               */
          /*       " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size" (* *) */
          /*       ,                                                            */

          /* "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " (* *)                        */
          /*       "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " (* *)                    */
          /*       " FROM " DUF_DBPREF "paths " (* *)                                                           */
          /*       " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " (* *) */
          /*       " GROUP BY parents." DUF_SQL_IDNAME "" (* *)                                                 */
          /*       ,                                                                                            */
          /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                                                     */
          /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                                                */
          /*       " FROM " DUF_DBPREF "paths AS p " (* *)                                                      */
          /*       " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *)                                */
          /*       ,                                                                                            */



          /* "DELETE FROM " DUF_DBPREF "keydata", */
          /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
          /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
          /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
          /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
          /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
          /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */


          NULL,
          }
};


duf_scan_callbacks_t duf_filenames_callbacks = {
  .title = "file names",
  .name = "filenames",
  .init_scan = NULL,
  .def_opendir = 1,

  /* .dirent_file_scan_before = filenames_entry_reg, */
  .dirent_file_scan_before2 = register_direntry,


  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_argv=NULL recommended in many cases) */
  .count_nodes = 1,
  .leaf = {.fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize " /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", md.dup5cnt AS nsame" /* */
           ", md.md5sum1, md.md5sum2 " ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           " WHERE "            /* */
           " fn.Pathid = :parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "paths AS p " /* */
           },
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
           },
  .final_sql_argv = &final_sql,
};
